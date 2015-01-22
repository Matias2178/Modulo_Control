//******************************************************************************
//SCOMM.DEF
//DEFINICIONES DE DRIVER DE COMUNICACION SERIE SINCRONICA (MASTER) POR 1 HILO
//							By L.A.S.
//******************************************************************************
//******************************************************************************
//DEFINICION DE TIPOS DE VARIABLES
//******************************************************************************
//	typedef unsigned char BYTE;
//	typedef unsigned int WORD;
//	typedef unsigned long DWORD;
	
//	#include	"VarType.h"
//******************************************************************************
//CONSTANTES MODIFICABLES
//******************************************************************************
#define kSW2_CLK				 1000		//(7372800/2)/2000
#define kSW2_CLK_MIN			(kSW2_CLK/2)
#define kSW2_CLK_MED			(kSW2_CLK+kSW2_CLK_MIN)
#define kSW2_CLK_MAX			(kSW2_CLK*4)
#define kSW2_TFTRXBIT			(65536-(kSW2_CLK_MAX * 2))
#define kSW2_TFTRXSTART			5123	//10246		//(65536-(kSW2_CLK*30))		//15 mS
#define kSW2_TSTARTTX			(65536-(kSW2_CLK*3))

#define kSW2_TRSTCOMPRX			22
#define kSW2_TWERRTX			(kSW2_CLK*20)

#define SW2_sizebuf				46

#define	SW2_stsWait				0b00000011
#define	SW2_stsErr				0b00001100

#define	SW2_cmdWr				0b00001000
#define	SW2_cmdRd				0b00000000

//******************************************************************************
//DEFINICION DE ESTRUCTURAS
//******************************************************************************
union _USw2masterSts
{
	unsigned char Value;
	struct
	{
		unsigned char Rx:1;			//RECEPCION DE DATOS , MODO RX
		unsigned char Tx:1;			//ENVIO DE DATOS , MODO TX
		unsigned char RxErr:1;		//ERROR EN RECEPCION DE DATOS
		unsigned char TxErr:1;		//ERROR EN TRANSMISION DE DATOS
		unsigned char RxEnd:1;		//FIN DE MODO RX
		unsigned char TxEnd:1;		//FIN DE MODO TX
		unsigned char TxCmd:1;		//COMANDO ENVIADO EN MODO TRANSMISION
		unsigned char ByteVal:1;		//VALOR DE BYTE
	} B;
};

union _USw2Cmd
{
	unsigned int Value;
	struct
	{
		unsigned char :4;
		unsigned char Addr:3;	//DIRECCION
		unsigned char Wr:1;		//WR=1 RD=0
		unsigned char Id:8;		//ID
	} B;
	struct
	{
		unsigned char :4;	
		unsigned char Cmd:4;
		unsigned char Id:8;		//ID
	} V;
};

struct _TSw2master
{
	union _USw2masterSts Sts;		//ESTADO
	union _USw2Cmd Cmd;				//COMANDO
	unsigned int timStore;
	unsigned char txInit;			//CONTADOR DE CLOCK DE ENABLE
	unsigned int data;				//DATO
	unsigned char bcount;			//CONTADOR DE BITS
	unsigned char bufIndex;			//INDICE DE BUFFER
	unsigned char bufTxCount;		//CANTIDAD DE DATOS EN EL BUFFER TX
	unsigned char bufRxCount;		//CANTIDAD DE DATOS EN EL BUFFER RX
	unsigned char buf[SW2_sizebuf];	//BUFFER
};

struct _TSw2powerSts
{
unsigned char Value;
	struct
	{
		unsigned char On:1;
		unsigned char Active:1;
		unsigned char Ready:1;
		unsigned char Ok:1;
		unsigned char Err:1;
		unsigned char :3;
	} B;
};

struct _TSw2power
{
	struct _TSw2powerSts Sts;		//ESTADO
	unsigned char tmrRst;
	unsigned char tmrReady;
};

union _USw2PortSts
{
	unsigned char Value;
	struct
	{
		unsigned char fPend:1;		//COMANDO PENDIENTE
		unsigned char fReady:1;		//LISTO
		unsigned char fOk:1;			//COMANDO EJECUTADO OK
		unsigned char fErr:1;		//COMANDO ERROR
		unsigned char :4;
	} B;
};

struct _TSw2Port
{
	union _USw2PortSts Sts;
	union _USw2Cmd Cmd;
	unsigned char RxCount;			//CANTIDAD DE DATOS A RECIBIR
	unsigned char TxCount;			//CANTIDAD DE DATOS A TRANSMITIR
	unsigned char Data[SW2_sizebuf];	
};

//******************************************************************************
//DECLARACION DE VARIABLES GLOBALES
//******************************************************************************
///----extern volatile near struct _TSW2power SW2Power;
extern struct _TSw2master SW2;
extern struct _TSw2Port SW2PortSys;
extern struct _TSw2Port SW2PortUser;

//******************************************************************************
//DECLARACION DE FUNCIONES DEL MODULO
//******************************************************************************
extern void SW2_Init(void);
extern void SW2_PortSysStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen);
extern void SW2_PortSysWr(unsigned char Data);
extern void SW2_PortSysWrBuf(void *pTxData,unsigned char TxLen);
extern void SW2_PortSysSend(void);
extern void SW2_PortUserStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen);
extern void SW2_PortUserWr(unsigned char Data);
extern void SW2_PortUserWrBuf(void *pTxData,unsigned char TxLen);
extern char SW2_PortUserSend(char fWait);
extern char Sw2_RdPV(unsigned char Id,unsigned char Addr,unsigned char LenPV);
extern char Sw2_RdReg(unsigned char Id,unsigned char RegId,unsigned long *Value);
extern char Sw2_WrReg(unsigned char Id,unsigned char RegId,unsigned long *Value);
extern void SW2_Tick0010(void);
extern void SW2_PortSysWr2(unsigned char Data);
extern void SW2_PortUserWr2(unsigned char Data);


