//******************************************************************************
//SW2_M.C
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

//RUTINAS DEL BUS SW2 MODO MASTER
	#include 	<p24Fxxxx.h>
	#include 	<string.h>
	#include 	"ES_Conf.h"
	#include 	"SW2master.h"
	#include 	"Constantes.h"
	#include	"ExeTask.h"


//VARIABLES INTERNAS USADAS
//volatile near 
struct _TSw2power SW2Power;
struct _TSw2master SW2;
struct _TSw2Port SW2PortSys;
struct _TSw2Port SW2PortUser;

//VARIABLES USADAS
unsigned char SW2_ActPort;

//RUTINAS DE USO INTERNO
void SW2_InitSend(void);

//******************************************************************************
//******************************************************************************
//	              RUTINAS DE USUARIO
//******************************************************************************
//******************************************************************************
void SW2_Init(void)
{
	OUT_SW2_BUS=0;
	SW2.Sts.Value=0;
	SW2.bufIndex=0;
	SW2.bufTxCount=0;
	SW2.bufRxCount=0;
	SW2.bcount=0;
	SW2_ActPort=0;
	
	T5CON=0b00010000;	//8 BIT R/W
						//PRESCALER = 2
						//TMR STOP

	TMR5=0;
	IEC1bits.T5IE=0;
	IEC1bits.INT2IE=0;
	INTCON2bits.INT2EP = 1; //INTEDG0=0;	//1>0
	IFS1bits.T5IF=0;
	IFS1bits.INT2IF=0;
	IPC7bits.T5IP=1;		//TMR3 Overflow Interrupt High Priority
}
//-----------------------------------------------------------------------------
void SW2_PortSysStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen)
{
	if (!SW2PortSys.Sts.B.fPend)
		{
		SW2PortSys.Sts.Value=0;
		SW2PortSys.Cmd.Value=0;
		SW2PortSys.Cmd.V.Id=Id;
		SW2PortSys.Cmd.V.Cmd=Cmd & 15;
		SW2PortSys.RxCount=RxLen;
		if (SW2PortSys.Cmd.B.Wr) SW2PortSys.RxCount=1;
		SW2PortSys.TxCount=0;
		}
}
//-----------------------------------------------------------------------------
void SW2_PortSysWr(unsigned char Data)
{
	if (!SW2PortSys.Sts.Value)
		{
		if (SW2PortSys.TxCount <= SW2_sizebuf)
			{		
			SW2PortSys.Data[SW2PortSys.TxCount]=Data;
			SW2PortSys.TxCount++;
			}
		}
}	
//-----------------------------------------------------------------------------
void SW2_PortSysWrBuf(void *pTxData,unsigned char TxLen)
{
unsigned char *pUChar=(unsigned char *)pTxData;

	if (!SW2PortSys.Sts.Value)
		{
		if ((SW2PortSys.TxCount + TxLen) <= SW2_sizebuf)
			{
			while(TxLen)
				{								
				SW2PortSys.Data[SW2PortSys.TxCount]=*pUChar;
				SW2PortSys.TxCount++;
				pUChar++;
				TxLen--;
				}
			}
		}
}
//-----------------------------------------------------------------------------
void SW2_PortSysSend(void)
{
	if (!SW2PortSys.Sts.Value)
		{
		SW2PortSys.Sts.B.fPend=true;
		}
}
//-----------------------------------------------------------------------------
void SW2_PortUserStart(unsigned char Id,unsigned char Cmd,unsigned char RxLen)
{
	if (!SW2PortUser.Sts.B.fPend)
		{
		SW2PortUser.Sts.Value=0;
		SW2PortUser.Cmd.Value=0;
		SW2PortUser.Cmd.V.Id=Id;
		SW2PortUser.Cmd.V.Cmd=Cmd & 15;
		SW2PortUser.RxCount=RxLen;
		if (SW2PortUser.Cmd.B.Wr) SW2PortUser.RxCount=1;
		SW2PortUser.TxCount=0;
		}
}
//-----------------------------------------------------------------------------
void SW2_PortUserWr(unsigned char Data)
{
	if (!SW2PortUser.Sts.Value)
		{
		if (SW2PortUser.TxCount <= SW2_sizebuf)
			{		
			SW2PortUser.Data[SW2PortUser.TxCount]=Data;
			SW2PortUser.TxCount++;
			}
		}
}
//-----------------------------------------------------------------------------
void SW2_PortSysWr2(unsigned char Data)
//void SW2_PortSysWr2(unsigned int Data)
{
int i;											//agregado MM
	for(i=0;i<2;i++)							//agregado MM
	{											//agregado MM
		if (!SW2PortSys.Sts.Value)
			{
			if (SW2PortSys.TxCount <= SW2_sizebuf)
				{		
				SW2PortSys.Data[SW2PortSys.TxCount]=Data;
				SW2PortSys.TxCount++;
				}	
			}
	}
}						
//-----------------------------------------------------------------------------
void SW2_PortUserWrBuf(void *pTxData,unsigned char TxLen)
{
unsigned char *pUChar=(unsigned char *)pTxData;

	if (!SW2PortUser.Sts.Value)
		{
		if ((SW2PortUser.TxCount + TxLen) <= SW2_sizebuf)
			{
			while(TxLen)
				{								
				SW2PortUser.Data[SW2PortUser.TxCount]=*pUChar;
				SW2PortUser.TxCount++;
				pUChar++;
				TxLen--;
				}
			}
		}
}
//-----------------------------------------------------------------------------
char SW2_PortUserSend(char fWait)
{
	if (!SW2PortUser.Sts.Value)
		{
		SW2PortUser.Sts.B.fPend=true;
		if (!fWait) return(true);
		while(SW2PortUser.Sts.B.fPend) ExeTask();
		if (SW2PortUser.Sts.B.fOk) return(true);
		}
	return(false);
}
//-----------------------------------------------------------------------------
char Sw2_RdPV(unsigned char Id,unsigned char Addr,unsigned char LenPV)
{
	SW2_PortUserStart(Id,Addr | SW2_cmdRd,LenPV);
	return(SW2_PortUserSend(true));
}
//-----------------------------------------------------------------------------
char Sw2_RdReg(unsigned char Id,unsigned char RegId,unsigned long *Value)
{
	SW2_PortUserStart(Id,0x06 | SW2_cmdRd,4);
	SW2_PortUserWr(RegId);		//REG.ID
	if (SW2_PortUserSend(true))
		{
		*Value=*(unsigned long *)SW2PortUser.Data;
		return(true);	
		}
	return(false);
}
//-----------------------------------------------------------------------------
char Sw2_WrReg(unsigned char Id,unsigned char RegId,unsigned long *Value)
{
	SW2_PortUserStart(Id,0x06 | SW2_cmdWr,0);
	SW2_PortUserWr(RegId);	//REG.ID
	SW2_PortUserWrBuf(Value,sizeof(unsigned long));
	return(SW2_PortUserSend(true));
}
//-----------------------------------------------------------------------------
void SW2_Tick0010(void)
{
unsigned char UChar00;

	if (SW2.Sts.Value & SW2_stsWait) return;

	if (SW2_ActPort & 0x80)
		{
		//RESPUESTA PENDIENTE
		if (SW2_ActPort & 0x01)
			{
			//USUARIO
			if (SW2PortUser.Sts.B.fPend)
				{
				memcpy(SW2PortUser.Data,SW2.buf,SW2_sizebuf);
				SW2PortUser.Sts.Value=0;
				SW2PortUser.Sts.B.fReady=true;
				if (SW2.Sts.Value & SW2_stsErr) SW2PortUser.Sts.B.fErr=true;
				if (SW2PortUser.Cmd.B.Wr)
					{
					if (SW2PortUser.Data[0] != 0xFF) SW2PortUser.Sts.B.fErr=true;
					}
				if (!SW2PortUser.Sts.B.fErr) SW2PortUser.Sts.B.fOk=true;
				}
			}
		else
			{
			//SISTEMA
			if (SW2PortSys.Sts.B.fPend)
				{
				memcpy(SW2PortSys.Data,SW2.buf,SW2_sizebuf);
				SW2PortSys.Sts.Value=0;
				SW2PortSys.Sts.B.fReady=true;
				if (SW2.Sts.Value & SW2_stsErr)
					{ 
					SW2PortSys.Sts.B.fErr=true;
					}
				if (SW2PortSys.Cmd.B.Wr)
					{
					if (SW2PortSys.Data[0] != 0xFF)
						{ 
						SW2PortSys.Sts.B.fErr=true;
						}
					}
				if (!SW2PortSys.Sts.B.fErr)
					{ 
					SW2PortSys.Sts.B.fOk=true;
					}
				}
			}
		}

	for(UChar00=0;UChar00<2;UChar00++)
		{
		SW2_ActPort++;
		SW2_ActPort &= 0x01;
		if (SW2_ActPort)
			{
			//USUARIO
			if (SW2PortUser.Sts.B.fPend)
				{
				memcpy(SW2.buf,SW2PortUser.Data,SW2_sizebuf);
				SW2.Cmd.Value=SW2PortUser.Cmd.Value;
				SW2.bufTxCount=SW2PortUser.TxCount;
				SW2.bufRxCount=SW2PortUser.RxCount;
				SW2_ActPort |= 0x80;
				}
			}
		else
			{
			//SISTEMA
			if (SW2PortSys.Sts.B.fPend)
				{
				memcpy(SW2.buf,SW2PortSys.Data,SW2_sizebuf);
				SW2.Cmd.Value=SW2PortSys.Cmd.Value;
				SW2.bufTxCount=SW2PortSys.TxCount;
				SW2.bufRxCount=SW2PortSys.RxCount;
				SW2_ActPort |= 0x80;
				}
			}
		if (SW2_ActPort & 0x80)
			{
			SW2_InitSend();
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
void SW2_InitSend(void)
{
	OUT_SW2_BUS=0;
	SW2.Sts.Value=0;
	SW2.txInit=0;
	T5CONbits.TON=0;
	IFS1bits.T5IF=0;
	IEC1bits.T5IE=1;
	TMR5=kSW2_TSTARTTX;
	T5CONbits.TON=1;
	SW2.bcount=0;
	SW2.Sts.B.Tx=1;
}
//******************************************************************************
