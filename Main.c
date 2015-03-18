/*****************************************************************************
 *	MONITOR DE SIEMBRA COLOR
 * 
 * DESARROLLO PIC24F256GA106
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/


	#include 	<p24Fxxxx.h>
	#include	<string.h>
	#include 	"ES_Conf.h"
	#include	"UART.h"
	#include	"Constantes.h"
	#include	"VarGlob.h"
	#include 	"ExeTask.h"	
	#include 	"sw1master.h"
	#include 	"sw2master.h"
	#include 	"ADC.h"	
	#include 	"Timer.h"	
	#include 	"eeprom_I2C.h"
	#include 	"Perifericos.h"
	#include	"Master3_I2C.h"
	#include 	"Comandos.h"
	#include 	"GPS.h"
	#include 	"fifoRS232.h"

	
			
	_CONFIG1 ( 0x0FE8)
	_CONFIG2 ( 0xF3EE)
	_CONFIG3 ( 0X61FF)
	
//	_CONFIG1 ( 0xFFFF)
//	_CONFIG2 ( 0xF3EE)
//	_CONFIG3 ( 0X61FF)	
	
int main (void)
{
//	int Control;
	union _UInt16 LocUserW00;
//	int i;
//	unsigned char Trans[256]= "HOLA ESTO ES UNA PRUEBA DE 256 CARACTERES PARA VER COMO SE COMPORTA EL FIFO EN TRANSMICION, ES SOLO PAR LLENAR LA FIFO Y VER COMO SE COMPORTA JUNTO CON LA TRASMICION DE DATOS QUE HAY QUE VER QUE PASA YA QUE TENGO QUE TRASMITIR 256 CARACTERES FIN DE LA CITA";
//-----------------------------------------------------------------------------
//Configuracion de Osciladores

	OSCCON = 0x14A7;
	
	
//-----------------------------------------------------------------------------
//	Configuracion temporizadores

//	Configuracion Timer1
	T1CON = 0x0020;			//Prescaler = 256
							//Fuente	= Oscilador Secundario
	PR1	  = 0x00E6;			//Un Pulso cada 1 mili segundo
		
	IPC0bits.T1IP = 1;		//Prioridad de interrupcion 1
				
	IEC0bits.T1IE = 1;		//Habilitacion Interrupcion Timer1
	IFS0bits.T1IF = 0;		//Borro las interupcion Timer1
		
//	IPC0bits.INT0IP = 1;		//Prioridad de interrupcion 1
	T1CONbits.TON =	1;		//Activo Timer1		
	OSCCONbits.SOSCEN = 1;
//	Configuracion Timer2
	T2CON = 0x0030;			// 0b1000000000000000;
	PR2	= 0x0FFF;
	TMR2 = 0x00;
//		IFSObits.T2IP = 0x03; 	//Prioridad de la interrupcion
	IEC0bits.T2IE = 1;	
	IFS0bits.T2IF = 0;	
	T2CONbits.TON =	1;		//Inicializo el timer

//Bloqueo las comunicaciones LIN
	Proceso.B.fInicio = true;
//-----------------------------------------------------------------------------
//Configuracion Entradas/Salidas Inicialisacion de Puertos
	Config_ES();
	
//-----------------------------------------------------------------------------
//	Encendido de todos los perifericos para comunicacion
	Pwr_Gral	= true;		//3v3 alimentacion general de la placa
	Nop();
	Pwr_GPS 	= true;		//Alimentacion 3v3 GPS
	GPSsts.B.fOn = true;	//Flag que indica que esta encendido el GPS
	Nop();
	Pwr_Wifi 	= true;		//Alimentacion 3v3 Modulo WiFi
	Nop();
	Hab_Wifi 	= true;		//Habilitacion modulo WiFi
	Nop();
	Hab_GPS 	= true;		//Habilitacion modulo GPS
	Nop();
	Rele1 		= true;		//Activacion Rele Bus 1
	Nop();		
	Rele2 		= true;		//Activacion Rele Bus 2
	Dly_10_MiliSec(25);	
	HabMen 		= false;		//Bloqueo la escritura de memoria
	PORTFbits.RF0 = true;

//-----------------------------------------------------------------------------
//	Inicializacion de I2C para la lectura/escritura de memoria
	InitI2C3();	
	
//-----------------------------------------------------------------------------
//Inicializacion de los puertos de comunicaciones UART
	EepromRDBuf(M_BAUDRATE,&LocUserW00,sizeof(union _UInt16));
	
	BaudiosUART2 = SCI_SetBaud(LocUserW00.B.V[0]);
	BaudiosUART3 = SCI_SetBaud(LocUserW00.B.V[1]);
	

	UART1Init();	//GPS
	UART2Init();	//Puerto 232
	UART3Init();	//WiFi
//	UART4Init();	//WiFi
	
	RSFIFOInitRX();		//Inicialización del buffer de recepcion RS-232
	WifiFIFOInitRX ();	//Inicialización del buffer de recepcion Wifi

	
//-----------------------------------------------------------------------------
//	INICIA COMUNICACIONES LIN
	SW1_Init();
	SW2_Init();
	
//-----------------------------------------------------------------------------
//	Configuracion del modulo GPS

	EepromRDBuf(M_GPS_AUX,(unsigned char *)&MGPS,sizeof(struct _MGPS));

	if( MGPS.Puerto == 2)
	{
		RPINR18bits.U1RXR = RP11;	//Puerto Auxiliar
		RPOR12bits.RP24R = U1TX_IO; //Puerto Auxiliar 
		if( MGPS.Baudrate <= 6 )
		{
			U1BRG = SCI_SetBaud(MGPS.Baudrate);	
		}
		else
		{
			U1BRG = SCI_SetBaud(1);
		}
	}
	Dly_1_MiliSec(12);
	EepromRDBuf(M_INFD_SN_0,(unsigned char *)&SerialNum,sizeof(SerialNum));
//-----------------------------------------------------------------------------
//	Comienzo de la configuracion inicial del sistema

//	ADConvInt();

	memset(&BUS1,0x00,sizeof(struct _BUS_MED));
	memset(&BUS2,0x00,sizeof(struct _BUS_MED));
	
//------Inicializacion de la memoria con los parametros de fabrica--------------
  	LED_LIN1 = true;
	LED_LIN2 = true;
	LED_CAN	 = true;
	LED_UART = true;
	LED_GPS = true;	
	LED_WIFI = true;
	LED_POWER = true;
	Dly_100_MiliSec(10);			
		
	MemInit();
	HabMen 		= false;	
//-------------------Cargo datos guardados en la memoria------------------------			
	MemLoad();
  	LED_LIN1 = false;
	LED_LIN2 = false;
	LED_CAN	 = false;
	LED_UART = false;
	LED_GPS = false;	
	LED_WIFI = false;
	LED_POWER = false;
	
	
	LED_POWER = true;
	LED_POWER_E = true;
	SenDtsMod.SemB1 = 0xFFFFFFFF;
	SenDtsMod.SemB2 = 0xFFFFFFFF;
	SenDtsMod.FerB1 = 0x00000000;
	SenDtsMod.FerB2 = 0x00000000;
	
	SenDtsHab.FerB1 = 0xFFFFFFFF;
	SenDtsHab.FerB2 = 0xFFFFFFFF;
	SenDtsHab.SemB2 = 0xFFFFFFFF;
	SenDtsHab.SemB1 = 0xFFFFFFFF;

	CargaConfSen();	
	CargaConfPer();
////-------------Lectura inicial de los sensores y perifericos--------------------
	SenRedetecion();
	Proceso.B.fInicio = true;
	SenStart000();	//Lectura inicial de los sensores de semilla y fertilizante
	ModStart000();	//Lectura inicial de las Moduladoras
	TRBStart000();	//Lectura inicial de los sensores de RPM
	RotStart000();	//Lectura inicial de los sensores de Rotacion
	TOLStart000();	//Lectuta inicial de los sensores de Tolva
	SenTuboSucio();

	GuardaPar();	//Guarda los parametros de los perifericos
//------------------------------------------------------------------------------
// LECTURA DEL NOMBRE DEL EQUIPO
	Dly_10_MiliSec(50);

	EepromRDBuf(M_NOMBRE_SEMB,(unsigned char *)Nombre,20);


//--------------Finaliza el proceso de inicializacion del sistema---------------
	Proceso.B.fInicio = false;

	

//------------------------------------------------------------------------------
//Loop principal

	while(1)
	{ 
				asm("CLRWDT");
		ExeTask();
		if(!Sts_Tmr.B.ADCInit)
		{		
			ADConf();
			Sts_Tmr.B.ADCInit = true;
		}
		if(Proceso.B.fAdqMod1 && Proceso.B.fAdqMod2)
		{
			Mod2DtsCom();
			Proceso.B.fAdqMod1 = false;
			Proceso.B.fAdqMod2 = false;	
		}
		if(Proceso.B.fAdqRot1 && Proceso.B.fAdqRot2)
		{
			ROT2DtsCom();
			Proceso.B.fAdqRot1 = false;
			Proceso.B.fAdqRot2 = false;	
		}
		if(Proceso.B.fAdqTRB1 && Proceso.B.fAdqTRB2)
		{
			TRB2DtsCom();
			Proceso.B.fAdqTRB1 = false;
			Proceso.B.fAdqTRB2 = false;	
		}
		if(Proceso.B.fAdqNTL1 && Proceso.B.fAdqNTL2)
		{
			TOL2DtsCom();
			Proceso.B.fAdqNTL1 = false;
			Proceso.B.fAdqNTL2 = false;	
		}
//----------------------------------------------------------------------------
//	ENVIO DE DATOS DESDE EL MODULO DE SIEMBRA AL TERMINAL		
		if(Proceso.B.fRxCom && !Wifi.LeeDato)
		{
			if(RS232.fCom)
			{
				Comando(Rx232Buf);	
			}
			if(Wifi.fCom)
			{
				Comando(Rx3Buf);
			}
			Proceso.B.fRxCom = false;
			Wifi.fCom = false;
			RS232.fCom = false;
			//Reseteo el buffer de recepcion para que no me joda
	//		Rx232Ind = 0;
	//		memset(Rx232Buf,0,sizeof(Rx232Buf));
	//		Rx3Ind = 0;
	//		memset(Rx3Buf,0,sizeof(Rx3Buf));
			
		}	
	}
}


  void __attribute__((__interrupt__,__shadow__)) _T1Interrupt( void )
 {
	IFS0bits.T1IF = 0;
	Sts_Tmr.B.Pls0001 = true;
	
	TLin1.Timer++; //incremento contador de tiempo de la vuelta de
	TLin2.Timer++; //los buses de comunicacion
	TiempoDatosL1++;
	if (TMRdly0001_0) TMRdly0001_0--;
	
	Sts_Tmr.Cnt0010++;
	if(Sts_Tmr.Cnt0010>=10)
	{
		Sts_Tmr.Cnt0010 = 0;
		Sts_Tmr.B.Pls0010  = true;
		if (TMRdly0010_0) TMRdly0010_0--;
	
		Sts_Tmr.Cnt0100++;
		if(Sts_Tmr.Cnt0100>=10)	
		{
			Sts_Tmr.Cnt0100 = 0;
			Sts_Tmr.B.Pls0100  = true;
			if (TMRdly0100_0) TMRdly0100_0--;

			Sts_Tmr.Cnt1000++;
			if(Sts_Tmr.Cnt1000>=10)
			{
				Sts_Tmr.Cnt1000 = 0;
				Sts_Tmr.B.Pls1000  = true;
				Sts_Tmr.B.SetID0500 = true;
			}
			Sts_Tmr.Cnt0500++;
			if(Sts_Tmr.Cnt0500>=5)
			{
				Sts_Tmr.Cnt0500 = 0;
//				Sts_Tmr.B.SetID0500 = true;
				
			}
			if(!Sts_Tmr.B.GPS5Hz)
			{
				Sts_Tmr.CntGPS5++;
				if(Sts_Tmr.CntGPS5>=100)
				{
					Sts_Tmr.CntGPS5 = 0;
					Sts_Tmr.B.GPS5Hz  = true;
				}
			}
			tlecgpstx ++;
			if(tlecgpstx  >=10)
			{
				Proceso.B.Puto = true;
				tlecgpstx = 0;
			}
		}
	}
}

 
 //-----------------------------------------------------------------------------
	//INTERRUPCION DEL TIMER2

 void __attribute__((__interrupt__,__shadow__)) _T2Interrupt( void )
 {
	 IFS0bits.T2IF = 0;
	 	PR2	= 0xFFFF;
}

/*
void __attribute__ ((__interrupt__)) _ADC1Interrupt(void)
{
	int *Conv;
	int i;
	
	IFS0bits.AD1IF = 0;
	Tension = 0;
	Conv = &ADC1BUF0;
	for(i=0;i<16;i++)
	{
		Tension = Tension + *Conv;
		Conv++;
	}	
	Tension = Tension/16;


	LED1 = true;
}
*/

/*****************************************************************************
 * EOF
 *****************************************************************************/ 
   
	