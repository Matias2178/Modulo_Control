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
#define kSW1_CLK				 1000		//(7372800/2)/2000
#define kSW1_CLK_MIN			(kSW1_CLK/2)
#define kSW1_CLK_MED			(kSW1_CLK+kSW1_CLK_MIN)
#define kSW1_CLK_MAX			(kSW1_CLK*4)
#define kSW1_TFTRXBIT			(65536-(kSW1_CLK_MAX * 2))
#define kSW1_TFTRXSTART			5123	//10246		//(65536-(kSW1_CLK*30))		//15 mS
#define kSW1_TSTARTTX			(65536-(kSW1_CLK*3))

#define kSW1_TRSTCOMPRX			22
#define kSW1_TWERRTX			(kSW1_CLK*20)

#define SW1_sizebuf				46

#define	SW1_stsWait				0b00000011
#define	SW1_stsErr				0b00001100

#define	SW1_cmdWr				0b00001000
#define	SW1_cmdRd				0b00000000

//******************************************************************************
//DEFINICION DE ESTRUCTURAS
//******************************************************************************
union _USw1masterSts
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

union _USw1Cmd
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

struct _TSw1master
{
	union _USw1masterSts Sts;		//ESTADO
	union _USw1Cmd Cmd;				//COMANDO
	unsigned int timStore;
	unsigned char txInit;			//CONTADOR DE CLOCK DE ENABLE
	unsigned int data;				//DATO
	unsigned char bcount;			//CONTADOR DE BITS
	unsigned char bufIndex;			//INDICE DE BUFFER
	unsigned char bufTxCount;		//CANTIDAD DE DATOS EN EL BUFFER TX
	unsigned char bufRxCount;		//CANTIDAD DE DATOS EN EL BUFFER RX
	unsigned char buf[SW1_sizebuf];	//BUFFER
};

struct _TSw1powerSts
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

struct _TSw1power
{
	struct _TSw1powerSts Sts;		//ESTADO
	unsigned char tmrRst;
	unsigned char tmrReady;
};

union _USw1PortSts
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

struct _TSw1Port
{
	union _USw1PortSts Sts;
	union _USw1Cmd Cmd;
	unsigned char RxCount;			//CANTIDAD DE DATOS A RECIBIR
	unsigned char TxCount;			//CANTIDAD DE DATOS A TRANSMITIR
	unsigned char Data[SW1_sizebuf];	
};

//******************************************************************************
//DECLARACION DE VARIABLES GLOBALES
//******************************************************************************
///----extern volatile near struct _TSw1power SW1Power;
extern struct _TSw1master SW1;
extern struct _TSw1Port SW1PortSys;
extern struct _TSw1Port SW1PortUser;

//******************************************************************************
//DECLARACION DE FUNCIONES DEL MODULO
//******************************************************************************
extern void SW1_Init(void);
extern void SW1_PortSysStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen);
extern void SW1_PortSysWr(unsigned char Data);
extern void SW1_PortSysWrBuf(void *pTxData,unsigned char TxLen);
extern void SW1_PortSysSend(void);
extern void SW1_PortUserStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen);
extern void SW1_PortUserWr(unsigned char Data);
extern void SW1_PortUserWrBuf(void *pTxData,unsigned char TxLen);
extern char SW1_PortUserSend(char fWait);
extern char Sw1_RdPV(unsigned char Id,unsigned char Addr,unsigned char LenPV);
extern char Sw1_RdReg(unsigned char Id,unsigned char RegId,unsigned long *Value);
extern char Sw1_WrReg(unsigned char Id,unsigned char RegId,unsigned long *Value);
extern void SW1_Tick0010(void);
extern void SW1_PortSysWr2(unsigned char Data);
extern void SW1_PortUserWr2(unsigned char Data);


