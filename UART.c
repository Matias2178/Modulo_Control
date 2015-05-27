/*****************************************************************************
 *	 FUNCIONES DE CONFIGURACION, INTERRUPCIONES DE LAS UART
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - 2013
 *	
 *****************************************************************************/
//include "system.h"
	#include 	<p24Fxxxx.h>
	#include	<string.h>
	#include	"VarGlob.h"
	#include 	"ES_Conf.h"
	#include 	"UART.h"
	#include 	"GPS.h"
	#include 	"Comandos.h"
	#include 	"fifoRS232.h"
	#include 	"fifoWifi.h"
	
/*****************************************************************************
 * U2BRG register value and baudrate mistake calculation
 *****************************************************************************/
#define SYSCLK	 7372800
// Baudrate
#define BAUDRATE2		9600

#define BAUDRATEREG2 ((SYSCLK/BAUDRATE2)/8)-1

#define UART_BaudDIV (SYSCLK/8)

#if BAUDRATEREG2 > 255
#error Cannot set up UART2 for the SYSCLK and BAUDRATE.\
 Correct values in main.h and uart2.h files.
#endif

unsigned int U1TxInd;
unsigned int U2TxInd;
unsigned int U3TxInd;

unsigned char RxBuf[256];
unsigned int RxInd;


unsigned char Rx232Buf[256];
unsigned int Rx232Ind;
			 

unsigned char Rx3Buf[256];
unsigned int Rx3Ind;

unsigned char U1RxBuf[80];
unsigned int U1RxInd;
unsigned int U1RxBf;

unsigned char U1TxBuf[80];
unsigned char U2TxBuf[260];
unsigned char U3TxBuf[260];

unsigned char SAVE[512];

unsigned char QTxBuf[256];
unsigned char ComBuf[260];

unsigned char WFcmd[6];
unsigned char WFind;

/******************************************************************************
*	Funcion:		UART2Init()
*	Descricpion:	Inicializa los parametros de la UART2 para la comunicacion  
*					por el pueto serie con la PC
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void UART2Init()
{
//Baudios = BAUDRATEREG2;
//	U2BRG = 95; //Baurate 9600
//	U2BRG = BAUDRATEREG2;
//	U2BRG = 7;	//Baurate 115200

		
	U2BRG = BaudiosUART2;
	U2MODE 	= 0x0000;
	U2STA 	= 0x0000;

//Cofgiguracion del nivel de interrupcion	
	IPC7bits.U2RXIP 		= 0x4;					// Mid Range Interrupt Priority level, no urgent reason
	IPC7bits.U2TXIP 		= 0x4;					// Mid Range Interrupt Priority level, no urgent reason
	
//Reseteo de los flags y habilitacion de las interrupciones
	IFS1bits.U2TXIF 	= 0;		// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE 	= 1;		// Enable Transmit Interrupts
	IFS1bits.U2RXIF 	= 0;		// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE 	= 1;		// Enable Recieve Interrupts
	
//Habilito las comunicaciones	
	U2MODEbits.UARTEN 	= 1;
	U2STAbits.UTXEN 	= 1;
}

	
/******************************************************************************
*	Funcion:		_U2RXInterrupt()
*	Descricpion:	Interrupcion por recepcion de datos en el puerto UART2  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
	
	char Registro;
//	Registro = U2RXREG;
	IFS1bits.U2RXIF = 0;
	
	if((Registro == 0x0A) ||(Registro == 0x0D)|| !Registro)
	{
		LED_UART = 0;
//		LED_UART_E = 0;
	}
	else
	{	
		LED_UART = LED_UART ? 0 : 1;
//		LED_UART_E = LED_UART ? 0 : 1;
	}
	RSFIFOISRRX();
//	if(Proceso.B.fGPSLec)
//	{
//		if(Registro =='$' )
//		{
//			Rx2Ind = 0;
//			U1TxInd = 0;
//			memset(U1TxBuf,0,sizeof(U1TxBuf));
//		}
//		if(Registro ==0x0A )
//		{
//			U1TxBuf[U1TxInd] = Registro;
//		//	U1TxInd++;
//		//	U1TxBuf[U1TxInd] = U2RXREG;
//			U1TxInd = 0;
//			U1TXREG = U1TxBuf[U1TxInd];
//		}
//		else
//		{		
//			
//			U1TxBuf[U1TxInd] = Registro;	//lo use para ver el modulo wifi
//			U1TxInd ++;
//		}
//	}
//	
//	else if(Proceso.B.fWifiConf)
//	{
//		U3TXREG = Registro;	//lo use para ver el modulo wifi
//	}
//	
//	if( Registro == 0x0D || Registro =='<' )
////	if(Registro =='<' )
//	{
//		Proceso.B.fRxCom = true;	//Fin del bloque
//		Proceso.B.fComRS = true;			
//	}
//	else if((Registro == '>')||(Rx2Ind>=256))
//	{
//		Rx2Ind = 0;
//		memset(Rx2Buf,0,sizeof(Rx2Buf));
//	}
////	if(!Proceso.B.fComRS)
////	{
//		Rx2Buf[Rx2Ind] = Registro;	
//		Rx2Ind++;
////	}
	
}

/******************************************************************************
*	Funcion:		_U2TXInterrupt()
*	Descricpion:	Interrupcion por transmicion de datos en el puerto UART2  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void)
{
	IFS1bits.U2TXIF = 0;
                                                               
	if((U2TxBuf[U2TxInd] == 0x0A) ||(U2TxBuf[U2TxInd] == 0x0D)|| !U2TxBuf[U2TxInd])
	{
		LED_UART = 0;
//		LED_UART_E = 0;
	}
	else
	{
		LED_UART = LED_UART ? 0 : 1;
//		LED_UART_E = LED_UART ? 0 : 1;
	}
	if((!Proceso.B.fGPSLec && !Proceso.B.fWifiConf))
//	if((Proceso.B.fGPSLec && !Proceso.B.fWifiConf)) //Solo para pruebas del modulo GPS
	{
		U2TxInd++;
		if(U2TxInd<260 && U2TxBuf[U2TxInd])
		{
			U2TXREG = U2TxBuf[U2TxInd];
		}
		else
		{
				Proceso.B.fT2xCOM = true;
		}
	}

}

/******************************************************************************
*	Funcion:		UART1Init()
*	Descricpion:	Inicializa los parametros de la UART1 para la comunicacion  
*					por el pueto serie con el modulo GPS
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	 
 void UART1Init()
{
	U1BRG = 191;
//	U1BRG = 95; //Baurate 9600
//	U1BRG = 47; //Baurate 19200
//	U1BRG = SCI_SetBaud(6);
//	U1BRG = SCI_SetBaud(3); //19200
	U1MODE 	= 0x0000;
	U1STA 	= 0x0000;
	MGPS.Puerto = 1; 
	MGPS.Baudrate = 1;

//Cofgiguracion del nivel de interrupcion	
//	IPC7 = 0x4400;					// Mid Range Interrupt Priority level, no urgent reason
	
//Reseteo de los flags y habilitacion de las interrupciones
	IFS0bits.U1TXIF 	= 0;		// Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE 	= 1;		// Enable Transmit Interrupts
	IFS0bits.U1RXIF 	= 0;		// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE 	= 1;		// Enable Recieve Interrupts

//Habilito las comunicaciones	
	U1MODEbits.UARTEN 	= 1;
	U1STAbits.UTXEN 	= 1;
}


/******************************************************************************
*	Funcion:		_U1RXInterrupt()
*	Descricpion:	Interrupcion por recepcion de datos en el puerto UART1  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) 
{
	IFS0bits.U1RXIF = 0;
	char Registro;
	GPSFIFOISRRX();

//	Registro = U1RXREG;
//	U4TXREG = Registro;
//	
//	if(Proceso.B.fGPSLec)
//	{			
//		U2TXREG = Registro;
//		U3TXREG	= Registro;
//	}
//
//	if( GPSsts.B.fLec && (Registro == 0x0A))
//	{
//		GPSsts.B.fLec = false;
//		GPSsts.B.fMsgOk = GPSCheckSum(U1RxBuf);
//		if(GPSsts.B.fMsgOk)
//		{
//			memcpy(GPSmsg,U1RxBuf,80);
//		}
//	}
//
//	else if( Registro == '$')
//	{
//		GPSsts.B.fLec = true;
//		U1RxInd = 0;
//		memset(U1RxBuf,0,strlen((const char*)U1RxBuf));
//	}
//	if(U1RxInd >75)
//	{
//		U1RxInd = 0;
//	}
//	U1RxBuf[U1RxInd] = Registro;	
//	U1RxInd++;
}


/******************************************************************************
*	Funcion:		_U1TXInterrupt()
*	Descricpion:	Interrupcion por transmicion de datos en el puerto UART1  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
	IFS0bits.U1TXIF = 0;
	if(Proceso.B.fGPSLec)
	{
		U1TxInd++;
		if(U1TxInd<70 && U1TxBuf[U1TxInd])
		{
			U1TXREG = U1TxBuf[U1TxInd];
		}
	}
}


/******************************************************************************
*	Funcion:		UART3Init()
*	Descricpion:	Inicializa los parametros de la UART3 para la comunicacion  
*					por el pueto serie con el modulo WIFI para la transmicion 
*					de datos hacia la pantalla/PC
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	 
 void UART3Init()
{
	U3BRG = BaudiosUART3;
//	U3BRG = 95;
	U3MODE 	= 0x0000;
	U3STA 	= 0x0000;
//	RTSU3 = 0;

//Cofgiguracion del nivel de interrupcion	
//	IPC7 = 0x4400;					// Mid Range Interrupt Priority level, no urgent reason
	
//Reseteo de los flags y habilitacion de las interrupciones
	IFS5bits.U3TXIF 	= 0;		// Clear the Transmit Interrupt Flag
	IEC5bits.U3TXIE 	= 1;		// Enable Transmit Interrupts
	IFS5bits.U3RXIF 	= 0;		// Clear the Recieve Interrupt Flag
	IEC5bits.U3RXIE 	= 1;		// Enable Recieve Interrupts
	
//Habilito las comunicaciones	
	U3MODEbits.UARTEN 	= 1;
	U3STAbits.UTXEN 	= 1;
}


/******************************************************************************
*	Funcion:		_U3RXInterrupt()
*	Descricpion:	Interrupcion por recepcion de datos en el puerto UART3  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U3RXInterrupt(void) {
	char Registro;
	
//	Registro = U3RXREG;
	IFS5bits.U3RXIF = 0;

	WifiFIFOISRRX();
//	if (Registro =='*')
//	{
//		if (Proceso.B.fWifiOC)
//		{
//			Proceso.B.fWifiOC = false;
//			if(Check(WFcmd,"OPEN",5))
//			{
//				Sts_Tmr.CntWifi = 0;
//				Proceso.B.fWifiOpen = true;
//			}
//			else if(Check(WFcmd,"CLOSE",6) ||Check(WFcmd,"CLOS",5))
//			{
//				Proceso.B.fWifiOpen = false;
//				Sts_Tmr.CntWifi = 0;
//			}
//		}
//		else
//		{
//			Proceso.B.fWifiOC = true;
//			memset(WFcmd,0,sizeof(WFcmd));
//			WFind = 0;
//		}
//	}
//	else if(Proceso.B.fWifiOC)
//	{
//		WFcmd[WFind] = Registro;
//		WFind++;
//		
//	}
//
//	if(Proceso.B.fWifiConf)
//	{
//		U2TXREG = Registro;	//lo use para ver el modulo wifi
//	}
//
//	else
//	{	
//		if( Registro == 0x0D || Registro =='<')
//		{
//			Proceso.B.fRxCom = true;	//Fin del bloque	
//			Proceso.B.fComWf = true;
//			Rx3Buf[Rx3Ind] = Registro;	
//			Rx3Ind++;		
//		}
//
//		else if((Registro == '>')||(Rx3Ind >=256))
//		{
//			Rx3Ind = 0;
//			memset(Rx3Buf,0,sizeof(Rx3Buf));
//		}
//		if(!Proceso.B.fComWf)
//		{
//			Rx3Buf[Rx3Ind] = Registro;	
//			Rx3Ind++;
//		}
//
//	}
}


/******************************************************************************
*	Funcion:		_U3TXInterrupt()
*	Descricpion:	Interrupcion por transmicion de datos en el puerto UART3  
*					
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void __attribute__ ((interrupt, no_auto_psv)) _U3TXInterrupt(void) {
	IFS5bits.U3TXIF = 0;
	if((!Proceso.B.fGPSLec && !Proceso.B.fWifiConf))
	{
		U3TxInd++;
		if(U3TxInd<260 && U3TxBuf[U3TxInd])
		{
			U3TXREG = U3TxBuf[U3TxInd];
		}
		else
		{
			Proceso.B.fT3xCOM = true;
		}
	}
}


/******************************************************************************
*	Funcion:		UART3Init()
*	Descricpion:	Inicializa los parametros de la UART3 para la comunicacion  
*					por el pueto serie con el modulo WIFI para la transmicion 
*					de datos hacia la pantalla/PC
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	 
 void UART4Init()
{
	U4BRG = BaudiosUART3;
//	U3BRG = 95;
	U4MODE 	= 0x0000;
	U4STA 	= 0x8000;
//	RTSU3 = 0;

//Cofgiguracion del nivel de interrupcion	
//	IPC7 = 0x4400;					// Mid Range Interrupt Priority level, no urgent reason
	
//Reseteo de los flags y habilitacion de las interrupciones
	IFS5bits.U4TXIF 	= 0;		// Clear the Transmit Interrupt Flag
	IEC5bits.U4TXIE 	= 1;		// Enable Transmit Interrupts
	IFS5bits.U4RXIF 	= 0;		// Clear the Recieve Interrupt Flag
	IEC5bits.U4RXIE 	= 1;		// Enable Recieve Interrupts
	
//Habilito las comunicaciones	
	U4MODEbits.UARTEN 	= 1;
	U4STAbits.UTXEN 	= 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _U4TXInterrupt(void) {
	IFS5bits.U4TXIF = 0;
//	U4FIFOReadTX();
	
	
	
}
void __attribute__ ((interrupt, no_auto_psv)) _U4RXInterrupt(void) {
	char Registro;
	
	Registro = U4RXREG;
	IFS5bits.U4RXIF = 0;


}

/******************************************************************************
*	Funcion:		SCI_SetBaud()
*	Descricpion:	Devuelve el valor que se tiene que cargar al registro UxBRG 
*					segun los baudios a transmitir y la frecuencia de clock del
*					micro
*					
*	Ingreso Datos:	Seleccion de baudios a transmitir
*					1:	4800
*					2:	9600
*					3:	19200
*					4:	38400
*					5:	57600
*					6:	115200
*					def:9600
*	Salida Datos:	Baudrate - valor que se carga en el registro de baudios
******************************************************************************/	
int SCI_SetBaud(char Baud)
{
	int BaudRate;
	switch(Baud)	
	{
		case 1:
			BaudRate=(UART_BaudDIV/4800)-1;		//BRG = 191
			break;
		case 2:
			BaudRate=(UART_BaudDIV/9600)-1;		//BRG = 95
			break;
		case 3:
			BaudRate=(UART_BaudDIV/19200)-1;	//BRG = 47
			break;
		case 4:						
			BaudRate=(UART_BaudDIV/38400)-1;	//BRG = 
			break;
		case 5:						
			BaudRate=(UART_BaudDIV/57600)-1;	//BRG = 
			break;
		case 6:						
			BaudRate=(UART_BaudDIV/115200)-1;	//BRG = 
			break;
		default:
			BaudRate=(UART_BaudDIV/9600)-1;		//BRG = 7
			break;
	}
	return BaudRate;	
}

/*****************************************************************************
 * EOF
 *****************************************************************************/
