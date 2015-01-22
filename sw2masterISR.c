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
	#include 	"SW2master.h"

	void SW2MasterISR(void);

		void __attribute__((interrupt, auto_psv)) _INT2Interrupt ()
	{
			SW2MasterISR();
	}
	void __attribute__((interrupt, no_auto_psv)) _T5Interrupt ()
	{
	
		SW2MasterISR();
	}
//------------------------------------------------------------------------------
//	RUTINA DE INTERRUPCION SW2
//------------------------------------------------------------------------------
void SW2MasterISR(void)
{
	if (SW2.Sts.B.Tx)
	{
//TRANSMISION
		if (IFS1bits.T5IF)
		{
			if (OUT_SW2_BUS)
			{
				if(IN_SW2_BUS)
				{
//CORTO CIRCUITO O SOBRE CARGA	
SW2_ISP_TX_ABORT:
					SW2.Sts.Value=0;
SW2_ISP_TX_END:
					SW2.Sts.B.TxErr=1;
					OUT_SW2_BUS=0;
					T5CONbits.TON=0;
					IEC1bits.T5IE=0;
					IEC1bits.INT2IE=0;
					IFS1bits.T5IF=0;
					IFS1bits.INT2IF=0;
				}
				else
				{
					if (SW2.txInit < 5) SW2.txInit++;
					else
					{
						if (!SW2.Sts.B.ByteVal) OUT_SW2_BUS=0;
						SW2.Sts.B.ByteVal=0;
					}
					goto SW2_ISP_TX_EXIT;
				}
			}
			else
			{
				if (SW2.bcount >= 13)
				{
//SE TIENE QUE DETECTAR PRESENCIA DEL ESCLAVO
					if (IN_SW2_BUS) 
					{
						SW2.Sts.Value=0;
						SW2.Sts.B.RxErr=1;
						goto SW2_ISP_TX_END;
					}
					else
					{
//PRESECIA OK
						SW2.Sts.B.RxErr=1;
						SW2.Sts.B.RxErr=0;
					}
				}
				else
				{
					if (!IN_SW2_BUS) goto SW2_ISP_TX_ABORT;
				}
					if (SW2.Sts.B.TxEnd)
				{
//INICIA MODO RECEPCION
					OUT_SW2_BUS=0;
					SW2.Sts.Value=0;
					SW2.Sts.B.Rx=1;
					SW2.bufIndex=0;
					SW2.bcount=0;
					
					IFS1bits.INT2IF=0;
					INTCON2bits.INT2EP 	= 1; 	//1>0
					IEC1bits.INT2IE=1;
					
					T5CONbits.TON=0;
					TMR5=kSW2_TFTRXSTART;
					IFS1bits.T5IF=0;
					IEC1bits.T5IE=1;
					goto SW2_ISP_TX_EXIT00;
				}

				OUT_SW2_BUS=1;

				if (!SW2.txInit) 
				{
					SW2.Sts.B.ByteVal=0;
					SW2.Sts.B.TxCmd=0;
					SW2.bcount=0;
					SW2.bufIndex=0;
					goto SW2_ISP_TX_EXIT;
				}

				if (!SW2.Sts.B.TxCmd)
				{
					if (!SW2.bcount)
					{
						SW2.data=SW2.Cmd.Value;
						SW2.Sts.B.ByteVal=0;
						goto SW2_ISP_TX_NEXTB;
					}
					else if (SW2.bcount >= 13)
					{
						SW2.bcount=0;
						SW2.Sts.B.ByteVal=1;
						SW2.Sts.B.TxCmd=1;
						if (SW2.bufIndex >= SW2.bufTxCount) SW2.Sts.B.TxEnd=1;
						goto SW2_ISP_TX_EXIT;
					}
					else
					{
						if (SW2.data & 0x8000) SW2.Sts.B.ByteVal=1;
						else SW2.Sts.B.ByteVal=0;
						SW2.data <<= 1;
						goto SW2_ISP_TX_NEXTB;
					}
				}
				else
				{
					if (!SW2.bcount)
					{
						SW2.data=SW2.buf[SW2.bufIndex];
						SW2.bufIndex++;
						SW2.Sts.B.ByteVal=0;
						goto SW2_ISP_TX_NEXTB;
					}
					else if (SW2.bcount >= 9)
					{
						SW2.bcount=0;
						SW2.Sts.B.ByteVal=1;
						if (SW2.bufIndex >= SW2.bufTxCount) SW2.Sts.B.TxEnd=1;
						goto SW2_ISP_TX_EXIT;
					}
					else
					{
						if (SW2.data & 0x0080) SW2.Sts.B.ByteVal=1;
						else SW2.Sts.B.ByteVal=0;
						SW2.data <<= 1;
SW2_ISP_TX_NEXTB:
						SW2.bcount++;
SW2_ISP_TX_EXIT:
						T5CONbits.TON=0;
						TMR5 -= (kSW2_CLK-7);
SW2_ISP_TX_EXIT00:
						T5CONbits.TON=1;
						IFS1bits.T5IF=0;
					}
				}
			}
		}
	}
	else if (SW2.Sts.B.Rx)
	{
//RECEPCION
		if (IFS1bits.INT2IF)
		{
			T5CONbits.TON=0;
			IFS1bits.T5IF=0;
			IFS1bits.INT2IF=0;

			if (!INTCON2bits.INT2EP)//0>1
			{

				SW2.timStore=TMR5;
				TMR5=kSW2_TFTRXBIT;
				T5CONbits.TON=1;
				INTCON2bits.INT2EP=1;
				if (SW2.timStore >= kSW2_CLK_MAX)
				{ 
					goto SW2_ISP_RX_ABORT;
				}
				if (SW2.timStore <= kSW2_CLK_MIN)
				{
					goto SW2_ISP_RX_ABORT;
				}
					if (SW2.timStore >= kSW2_CLK_MED) SW2.Sts.B.ByteVal=1;
				else SW2.Sts.B.ByteVal=0;
	
				if (!SW2.bcount)
				{
//START
					if (SW2.Sts.B.ByteVal)
					{ 
						goto SW2_ISP_RX_ABORT; 
					}
					SW2.data=0;
					SW2.bcount++;
				}
				else if (SW2.bcount >= 9)
				{
//STOP
					if (!SW2.Sts.B.ByteVal)
					{ 
						goto SW2_ISP_RX_ABORT; 
					}
					if (!SW2.bufRxCount)
					{
						SW2.Sts.Value=0;
						SW2.Sts.B.RxEnd=1;
						goto SW2_ISP_RX_END;
					}
						SW2.buf[SW2.bufIndex]=SW2.data;
					SW2.bufIndex++;
					SW2.bcount=0;
						if (SW2.bufIndex >= SW2.bufRxCount)
					{
						SW2.Sts.Value=0;
						SW2.Sts.B.RxEnd=1;
						goto SW2_ISP_RX_END;
					}
				}
				else
				{
					SW2.data <<= 1;
					SW2.data &= 255;
					if (SW2.Sts.B.ByteVal) SW2.data |= 1;
					SW2.bcount++;
				}
			}
			else
			{
	//1>0
				TMR5=kSW2_TRSTCOMPRX;
				T5CONbits.TON=1;
				INTCON2bits.INT2EP=0;
			}
		}
			if (IFS1bits.T5IF)
		{
			goto SW2_ISP_RX_ABORT;
		}
			if (IFS1bits.T5IF)
		{
SW2_ISP_RX_ABORT:
			SW2.Sts.Value=0;
			SW2.Sts.B.RxErr=1;
SW2_ISP_RX_END:
			OUT_SW2_BUS=0;
			T5CONbits.TON=0;
			IEC1bits.T5IE=0;
			IEC1bits.INT2IE=0;
			IFS1bits.T5IF=0;
			IFS1bits.INT2IF=0;
		}
	}
	else
	{
		IFS1bits.T5IF=0;
		IFS1bits.INT2IF=0;
	}
}
//------------------------------------------------------------------------------
//	FIN DE RUTINA DE INTERRUPCION SW2
//------------------------------------------------------------------------------
