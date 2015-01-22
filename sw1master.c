//******************************************************************************
//SW1_M.C
//DRIVER DE COMUNICACION SERIE SINCRONICA (MASTER) POR 1 HILO PARA 16CXX-12CXX
//							By L.A.S.
//
//ENABLE |??|____________|??|
//
//DATO 0 |??|__|??|	
//
//DATO 1 |??|____|??|
//
//PRESENCIA |?|_____|??
//			|?|___|_|??
//******************************************************************************

//RUTINAS DEL BUS SW1 MODO MASTER
	#include 	<p24Fxxxx.h>
	#include 	<string.h>
	#include 	"ES_Conf.h"
	#include 	"sw1master.h"
	#include 	"Constantes.h"
	#include	"ExeTask.h"


//VARIABLES INTERNAS USADAS
//volatile near 
struct _TSw1power SW1Power;
struct _TSw1master SW1;
struct _TSw1Port SW1PortSys;
struct _TSw1Port SW1PortUser;

//VARIABLES USADAS
unsigned char SW1_ActPort;

//RUTINAS DE USO INTERNO
void SW1_InitSend(void);

//******************************************************************************
//******************************************************************************
//	              RUTINAS DE USUARIO
//******************************************************************************
//******************************************************************************
void SW1_Init(void)
{
	OUT_SW1_BUS=0;
	SW1.Sts.Value=0;
	SW1.bufIndex=0;
	SW1.bufTxCount=0;
	SW1.bufRxCount=0;
	SW1.bcount=0;
	SW1_ActPort=0;
	
	T3CON=0b00010000;	//8 BIT R/W
						//PRESCALER = 2
						//TMR STOP

	TMR3=0;
	IEC0bits.T3IE=0;
	IEC1bits.INT1IE=0;
	INTCON2bits.INT1EP = 1; //INTEDG0=0;	//1>0
	IFS0bits.T3IF=0;
	IFS1bits.INT1IF=0;
	IPC2bits.T3IP=1;		//TMR3 Overflow Interrupt High Priority
}
//-----------------------------------------------------------------------------
void SW1_PortSysStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen)
{
	if (!SW1PortSys.Sts.B.fPend)
		{
		SW1PortSys.Sts.Value=0;
		SW1PortSys.Cmd.Value=0;
		SW1PortSys.Cmd.V.Id=Id;
		SW1PortSys.Cmd.V.Cmd=Cmd & 15;
		SW1PortSys.RxCount=RxLen;
		if (SW1PortSys.Cmd.B.Wr) SW1PortSys.RxCount=1;
		SW1PortSys.TxCount=0;
		}
}
//-----------------------------------------------------------------------------
void SW1_PortSysWr(unsigned char Data)
{
	if (!SW1PortSys.Sts.Value)
		{
		if (SW1PortSys.TxCount <= SW1_sizebuf)
			{		
			SW1PortSys.Data[SW1PortSys.TxCount]=Data;
			SW1PortSys.TxCount++;
			}
		}
}	
//-----------------------------------------------------------------------------
void SW1_PortSysWrBuf(void *pTxData,unsigned char TxLen)
{
unsigned char *pUChar=(unsigned char *)pTxData;

	if (!SW1PortSys.Sts.Value)
		{
		if ((SW1PortSys.TxCount + TxLen) <= SW1_sizebuf)
			{
			while(TxLen)
				{								
				SW1PortSys.Data[SW1PortSys.TxCount]=*pUChar;
				SW1PortSys.TxCount++;
				pUChar++;
				TxLen--;
				}
			}
		}
}
//-----------------------------------------------------------------------------
void SW1_PortSysSend(void)
{
	if (!SW1PortSys.Sts.Value)
		{
		SW1PortSys.Sts.B.fPend=true;
		}
}
//-----------------------------------------------------------------------------
void SW1_PortUserStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen)
{
	if (!SW1PortUser.Sts.B.fPend)
		{
		SW1PortUser.Sts.Value=0;
		SW1PortUser.Cmd.Value=0;
		SW1PortUser.Cmd.V.Id=Id;
		SW1PortUser.Cmd.V.Cmd=Cmd & 15;
		SW1PortUser.RxCount=RxLen;
		if (SW1PortUser.Cmd.B.Wr) SW1PortUser.RxCount=1;
		SW1PortUser.TxCount=0;
		}
}
//-----------------------------------------------------------------------------
void SW1_PortUserWr(unsigned char Data)
{
	if (!SW1PortUser.Sts.Value)
		{
		if (SW1PortUser.TxCount <= SW1_sizebuf)
			{		
			SW1PortUser.Data[SW1PortUser.TxCount]=Data;
			SW1PortUser.TxCount++;
			}
		}
}
//-----------------------------------------------------------------------------
void SW1_PortSysWr2(unsigned char Data)
//void SW1_PortSysWr2(unsigned int Data)
{
int i;											//agregado MM
	for(i=0;i<2;i++)							//agregado MM
	{											//agregado MM
		if (!SW1PortSys.Sts.Value)
			{
			if (SW1PortSys.TxCount <= SW1_sizebuf)
				{		
				SW1PortSys.Data[SW1PortSys.TxCount]=Data;
				SW1PortSys.TxCount++;
				}	
			}
	}
}						
//-----------------------------------------------------------------------------
void SW1_PortUserWrBuf(void *pTxData,unsigned char TxLen)
{
unsigned char *pUChar=(unsigned char *)pTxData;

	if (!SW1PortUser.Sts.Value)
		{
		if ((SW1PortUser.TxCount + TxLen) <= SW1_sizebuf)
			{
			while(TxLen)
				{								
				SW1PortUser.Data[SW1PortUser.TxCount]=*pUChar;
				SW1PortUser.TxCount++;
				pUChar++;
				TxLen--;
				}
			}
		}
}
//-----------------------------------------------------------------------------
char SW1_PortUserSend(char fWait)
{
	if (!SW1PortUser.Sts.Value)
		{
		SW1PortUser.Sts.B.fPend=true;
		if (!fWait) return(true);
		while(SW1PortUser.Sts.B.fPend) ExeTask();
		if (SW1PortUser.Sts.B.fOk) return(true);
		}
	return(false);
}
//-----------------------------------------------------------------------------
char Sw1_RdPV(unsigned char Id,unsigned char Addr,unsigned char LenPV)
{
	SW1_PortUserStart(Id,Addr | SW1_cmdRd,LenPV);
	return(SW1_PortUserSend(true));
}
//-----------------------------------------------------------------------------
char Sw1_RdReg(unsigned char Id,unsigned char RegId,unsigned long *Value)
{
	SW1_PortUserStart(Id,0x06 | SW1_cmdRd,4);
	SW1_PortUserWr(RegId);		//REG.ID
	if (SW1_PortUserSend(true))
		{
		*Value=*(unsigned long *)SW1PortUser.Data;
		return(true);	
		}
	return(false);
}
//-----------------------------------------------------------------------------
char Sw1_WrReg(unsigned char Id,unsigned char RegId,unsigned long *Value)
{
	SW1_PortUserStart(Id,0x06 | SW1_cmdWr,0);
	SW1_PortUserWr(RegId);	//REG.ID
	SW1_PortUserWrBuf(Value,sizeof(unsigned long));
	return(SW1_PortUserSend(true));
}
//-----------------------------------------------------------------------------
void SW1_Tick0010(void)
{
unsigned char UChar00;

	if (SW1.Sts.Value & SW1_stsWait) return;

	if (SW1_ActPort & 0x80)
		{
		//RESPUESTA PENDIENTE
		if (SW1_ActPort & 0x01)
			{
			//USUARIO
			if (SW1PortUser.Sts.B.fPend)
				{
				memcpy(SW1PortUser.Data,SW1.buf,SW1_sizebuf);
				SW1PortUser.Sts.Value=0;
				SW1PortUser.Sts.B.fReady=true;
				if (SW1.Sts.Value & SW1_stsErr) SW1PortUser.Sts.B.fErr=true;
				if (SW1PortUser.Cmd.B.Wr)
					{
					if (SW1PortUser.Data[0] != 0xFF) SW1PortUser.Sts.B.fErr=true;
					}
				if (!SW1PortUser.Sts.B.fErr) SW1PortUser.Sts.B.fOk=true;
				}
			}
		else
			{
			//SISTEMA
			if (SW1PortSys.Sts.B.fPend)
				{
				memcpy(SW1PortSys.Data,SW1.buf,SW1_sizebuf);
				SW1PortSys.Sts.Value=0;
				SW1PortSys.Sts.B.fReady=true;
				if (SW1.Sts.Value & SW1_stsErr)
					{ 
					SW1PortSys.Sts.B.fErr=true;
					}
				if (SW1PortSys.Cmd.B.Wr)
					{
					if (SW1PortSys.Data[0] != 0xFF)
						{ 
						SW1PortSys.Sts.B.fErr=true;
						}
					}
				if (!SW1PortSys.Sts.B.fErr)
					{ 
					SW1PortSys.Sts.B.fOk=true;
					}
				}
			}
		}

	for(UChar00=0;UChar00<2;UChar00++)
		{
		SW1_ActPort++;
		SW1_ActPort &= 0x01;
		if (SW1_ActPort)
			{
			//USUARIO
			if (SW1PortUser.Sts.B.fPend)
				{
				memcpy(SW1.buf,SW1PortUser.Data,SW1_sizebuf);
				SW1.Cmd.Value=SW1PortUser.Cmd.Value;
				SW1.bufTxCount=SW1PortUser.TxCount;
				SW1.bufRxCount=SW1PortUser.RxCount;
				SW1_ActPort |= 0x80;
				}
			}
		else
			{
			//SISTEMA
			if (SW1PortSys.Sts.B.fPend)
				{
				memcpy(SW1.buf,SW1PortSys.Data,SW1_sizebuf);
				SW1.Cmd.Value=SW1PortSys.Cmd.Value;
				SW1.bufTxCount=SW1PortSys.TxCount;
				SW1.bufRxCount=SW1PortSys.RxCount;
				SW1_ActPort |= 0x80;
				}
			}
		if (SW1_ActPort & 0x80)
			{
			SW1_InitSend();
			break;
			}
		}
}
//-----------------------------------------------------------------------------

//******************************************************************************
//******************************************************************************
//	              RUTINAS DE USO INTERNO
//******************************************************************************
//******************************************************************************
void SW1_InitSend(void)
{
	OUT_SW1_BUS=0;
	SW1.Sts.Value=0;
	SW1.txInit=0;
	T3CONbits.TON=0;
	IFS0bits.T3IF=0;
	IEC0bits.T3IE=1;
	TMR3=kSW1_TSTARTTX;
	T3CONbits.TON=1;
	SW1.bcount=0;
	SW1.Sts.B.Tx=1;
}
//******************************************************************************

