/*****************************************************************************
 *	DECLARACION DE LAS VARIABLES GOLBALES ULITIZADAS EN EL PROGRAMA
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
	#include 	<p24Fxxxx.h>
	#include 	"ES_Conf.h"
	#include	"Constantes.h"
	#include 	"sw1master.h"

	void sw1MasterISR(void);

		void __attribute__((interrupt, auto_psv)) _INT1Interrupt ()
	{
	//	IFS1bits.INT1IF		= 0; //borrar esto
		sw1MasterISR();
	}
	void __attribute__((interrupt, auto_psv)) _T3Interrupt ()
	{
	
		sw1MasterISR();
	}
//------------------------------------------------------------------------------
//	RUTINA DE INTERRUPCION SW1
//------------------------------------------------------------------------------
void sw1MasterISR(void)
{
	if (SW1.Sts.B.Tx)
	{
//TRANSMISION
		if (IFS0bits.T3IF)
		{
			if (OUT_SW1_BUS)
			{
				if(IN_SW1_BUS)
				{
//CORTO CIRCUITO O SOBRE CARGA	
SW1_ISP_TX_ABORT:
					SW1.Sts.Value=0;
SW1_ISP_TX_END:
					SW1.Sts.B.TxErr=1;
					OUT_SW1_BUS=0;
					T3CONbits.TON=0;
					IEC0bits.T3IE=0;
					IEC1bits.INT1IE=0;
					IFS0bits.T3IF=0;
					IFS1bits.INT1IF=0;
				}
				else
				{
					if (SW1.txInit < 5) SW1.txInit++;
					else
					{
						if (!SW1.Sts.B.ByteVal) OUT_SW1_BUS=0;
						SW1.Sts.B.ByteVal=0;
					}
					goto SW1_ISP_TX_EXIT;
				}
			}
			else
			{
				if (SW1.bcount >= 13)
				{
//SE TIENE QUE DETECTAR PRESENCIA DEL ESCLAVO
					if (IN_SW1_BUS) 
					{
						SW1.Sts.Value=0;
						SW1.Sts.B.RxErr=1;
						goto SW1_ISP_TX_END;
					}
					else
					{
//PRESECIA OK
						SW1.Sts.B.RxErr=1;
						SW1.Sts.B.RxErr=0;
					}
				}
				else
				{
					if (!IN_SW1_BUS) goto SW1_ISP_TX_ABORT;
				}
					if (SW1.Sts.B.TxEnd)
				{
//INICIA MODO RECEPCION
					OUT_SW1_BUS=0;
					SW1.Sts.Value=0;
					SW1.Sts.B.Rx=1;
					SW1.bufIndex=0;
					SW1.bcount=0;
					
					IFS1bits.INT1IF=0;
					INTCON2bits.INT1EP 	= 1; 	//1>0
					IEC1bits.INT1IE=1;
					
					T3CONbits.TON=0;
					TMR3=kSW1_TFTRXSTART;
					IFS0bits.T3IF=0;
					IEC0bits.T3IE=1;
					goto SW1_ISP_TX_EXIT00;
				}

				OUT_SW1_BUS=1;

				if (!SW1.txInit) 
				{
					SW1.Sts.B.ByteVal=0;
					SW1.Sts.B.TxCmd=0;
					SW1.bcount=0;
					SW1.bufIndex=0;
					goto SW1_ISP_TX_EXIT;
				}

				if (!SW1.Sts.B.TxCmd)
				{
					if (!SW1.bcount)
					{
						SW1.data=SW1.Cmd.Value;
						SW1.Sts.B.ByteVal=0;
						goto SW1_ISP_TX_NEXTB;
					}
					else if (SW1.bcount >= 13)
					{
						SW1.bcount=0;
						SW1.Sts.B.ByteVal=1;
						SW1.Sts.B.TxCmd=1;
						if (SW1.bufIndex >= SW1.bufTxCount) SW1.Sts.B.TxEnd=1;
						goto SW1_ISP_TX_EXIT;
					}
					else
					{
						if (SW1.data & 0x8000) SW1.Sts.B.ByteVal=1;
						else SW1.Sts.B.ByteVal=0;
						SW1.data <<= 1;
						goto SW1_ISP_TX_NEXTB;
					}
				}
				else
				{
					if (!SW1.bcount)
					{
						SW1.data=SW1.buf[SW1.bufIndex];
						SW1.bufIndex++;
						SW1.Sts.B.ByteVal=0;
						goto SW1_ISP_TX_NEXTB;
					}
					else if (SW1.bcount >= 9)
					{
						SW1.bcount=0;
						SW1.Sts.B.ByteVal=1;
						if (SW1.bufIndex >= SW1.bufTxCount) SW1.Sts.B.TxEnd=1;
						goto SW1_ISP_TX_EXIT;
					}
					else
					{
						if (SW1.data & 0x0080) SW1.Sts.B.ByteVal=1;
						else SW1.Sts.B.ByteVal=0;
						SW1.data <<= 1;
SW1_ISP_TX_NEXTB:
						SW1.bcount++;
SW1_ISP_TX_EXIT:
						T3CONbits.TON=0;
						TMR3 -= (kSW1_CLK-7);
SW1_ISP_TX_EXIT00:
						T3CONbits.TON=1;
						IFS0bits.T3IF=0;
					}
				}
			}
		}
	}
	else if (SW1.Sts.B.Rx)
	{
//RECEPCION
		if (IFS1bits.INT1IF)
		{
			T3CONbits.TON=0;
			IFS0bits.T3IF=0;
			IFS1bits.INT1IF=0;
///---VER SI ESTA BIEN LO DEL FLAG DE INTERRUPCION----
			if (!INTCON2bits.INT1EP)//0>1
			{

				SW1.timStore=TMR3;
				TMR3=kSW1_TFTRXBIT;
				T3CONbits.TON=1;
				INTCON2bits.INT1EP=1;
				if (SW1.timStore >= kSW1_CLK_MAX)
				{ 
					goto SW1_ISP_RX_ABORT;
				}
				if (SW1.timStore <= kSW1_CLK_MIN)
				{
					goto SW1_ISP_RX_ABORT;
				}
					if (SW1.timStore >= kSW1_CLK_MED) SW1.Sts.B.ByteVal=1;
				else SW1.Sts.B.ByteVal=0;
	
				if (!SW1.bcount)
				{
//START
					if (SW1.Sts.B.ByteVal)
					{ 
						goto SW1_ISP_RX_ABORT; 
					}
					SW1.data=0;
					SW1.bcount++;
				}
				else if (SW1.bcount >= 9)
				{
//STOP
					if (!SW1.Sts.B.ByteVal)
					{ 
						goto SW1_ISP_RX_ABORT; 
					}
					if (!SW1.bufRxCount)
					{
						SW1.Sts.Value=0;
						SW1.Sts.B.RxEnd=1;
						goto SW1_ISP_RX_END;
					}
						SW1.buf[SW1.bufIndex]=SW1.data;
					SW1.bufIndex++;
					SW1.bcount=0;
						if (SW1.bufIndex >= SW1.bufRxCount)
					{
						SW1.Sts.Value=0;
						SW1.Sts.B.RxEnd=1;
						goto SW1_ISP_RX_END;
					}
				}
				else
				{
					SW1.data <<= 1;
					SW1.data &= 255;
					if (SW1.Sts.B.ByteVal) SW1.data |= 1;
					SW1.bcount++;
				}
			}
			else
			{
	//1>0
				TMR3=kSW1_TRSTCOMPRX;
				T3CONbits.TON=1;
				INTCON2bits.INT1EP=0;//--VER SI ESTA BIEN
			}
		}
			if (IFS0bits.T3IF)
		{
			goto SW1_ISP_RX_ABORT;
		}
			if (IFS0bits.T3IF)
		{
SW1_ISP_RX_ABORT:
			SW1.Sts.Value=0;
			SW1.Sts.B.RxErr=1;
SW1_ISP_RX_END:
			OUT_SW1_BUS=0;
			T3CONbits.TON=0;
			IEC0bits.T3IE=0;
			IEC1bits.INT1IE=0;
			IFS0bits.T3IF=0;
			IFS1bits.INT1IF=0;
		}
	}
	else
	{
		IFS0bits.T3IF=0;
		IFS1bits.INT1IF=0;
	}
}
//------------------------------------------------------------------------------
//	FIN DE RUTINA DE INTERRUPCION SW1
//------------------------------------------------------------------------------
