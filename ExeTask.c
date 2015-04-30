/*****************************************************************************
 *	FUNCIONES DE LECTURA ANALOGICA
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 re*****************************************************************************/

	#include	"VarGlob.h"
	#include 	"ES_Conf.h"
	#include 	"UART.h"
	#include	"Constantes.h"
	#include 	"sw1master.h"
	#include 	"sw2master.h"
	#include 	"GPS.h"
	#include 	"Comunicacion.h"
	#include 	"Timer.h"
	#include 	"Master3_I2C.h"
	#include 	"ES_Conf.h"
//	#include 	"fifoRS232.h"
	#include	"User.h"

void ExeTask(void)
{
	unsigned int i;
	unsigned int SatAux;
	unsigned char SenId;
	unsigned char Dato;
	unsigned int  Dir;
	int	Ent;
	float Resto;
	union _UInt16 Datos;
	asm("CLRWDT");
//*****************************************************************************	
//	Tareas que se ejecutan cada 1ms
//*****************************************************************************
	if(Sts_Tmr.B.Pls0001)
	{
		Sts_Tmr.B.Pls0001 = false;

		LED_LIN1 = IN_SW1_BUS ? 0 : 1;	//Manejo led de comunicacion LIN1
		LED_LIN2 = IN_SW2_BUS ? 0 : 1;	//Manejo led de comunicacion LIN1
//		GPSdts.sys.t5hrz ++;
		LED_LIN1_E = IN_SW1_BUS ? 0 : 1;	//Manejo led de comunicacion LIN1
		LED_LIN2_E = IN_SW2_BUS ? 0 : 1;	//Manejo led de comunicacion LIN1
		}
	
//*****************************************************************************	
//	Tareas que se ejecutan cada 10ms
//*****************************************************************************	
	if(Sts_Tmr.B.Pls0010)
	{
		Sts_Tmr.B.Pls0010 = false;
//-----------------------------------------------------------------------------
//Temporizadores del GPS		
//-----------------------------------------------------------------------------
		while(!GPSFIFOEmptyRX() && !GPSsts.B.fMsgOk)// && !RS232.fCom)
		{
			Dato = GPSFIFOReadRX();
			if(Dato == '$')
			{
				GPSsts.B.fLec = true;
				GPSInd = 0;
			}
			if(GPSsts.B.fLec)
			{
				GPSmsg[GPSInd] = Dato;	
				GPSInd++;
			}
			if( GPSsts.B.fLec && (Dato == 0x0A))
			{
				GPSsts.B.fLec = false;
	//			GPSsts.B.fMsgOk = GPSCheckSum(GPSmsg);
				CGPS.Sentencias ++;
				if(GPSsts.B.fMsgOk = GPSCheckSum(GPSmsg))
					CGPS.Correctas ++;
				else
					CGPS.Fallas ++;
					
		//	GPSsts.B.fMsgOk = true;
				GPSInd = 0;
				break;
//				if(GPSsts.B.fMsgOk)
//				{
//					memcpy(GPSmsg,U1RxBuf,80);
//				}	
			}
		}
//-----------------------------------------------------------------------------
//	Control del implement Switch
//-----------------------------------------------------------------------------

		if( Sts_Tmr.B.ImpSw ^ IMPLEMENT_SW)
		{
			Sts_Tmr.TMRImpSw ++;
			if( Sts_Tmr.TMRImpSw > 30)
			{
				Sts_Tmr.B.ImpSw = IMPLEMENT_SW;
			}
		}
		else
		{
			Sts_Tmr.TMRImpSw = 0;
		}
//-----------------------------------------------------------------------------
//Proceso los datos que llegan del modulo GPS
		if(GPSsts.B.fMsgOk)
		{
			GPSInfo(GPSmsg);
			GPSsts.B.fMsgOk = false;
		}
//-----------------------------------------------------------------------------
//Lectura de datos desde el puerto RS-232

		while(!RSFIFOEmptyRX() && !RS232.fCom)
		{	
			Dato = RSFIFOReadRX();
			if ((Dato == '>')||(Rx232Ind>=256))
			{
				Rx232Ind = 0;
				RS232.lCom = true;
				
			}
			if(RS232.lCom)
			{
				Rx232Buf[Rx232Ind] = Dato;	
				Rx232Ind++;
			}
			if(Dato =='<')
			{
				Proceso.B.fRxCom = true;	//Fin del bloque
				RS232.fCom = true;
				RS232.lCom = false;
				Rx232Buf[Rx232Ind] = 0x00;	
				Rx232Ind++;	
				break;		
			}
		}
//-----------------------------------------------------------------------------
// Escritura de datos desde el modulo a la pantalla
//-----------------------------------------------------------------------------		
		if(Wifi.EscDato)
		{
			Sts_Tmr.CntWifi = 0;
			if(!Wifi.IniDato)
			{
				Wifi.IniDato = true;
				memset(SAVE,0x00,512);
				for(i=0;i<512;i=i+128)
				{
					Dir = MemAddr + i;
					EepromWRBuf(Dir,&SAVE[i],128);
					Dly_1_MiliSec(12);
				}
				i = 0;
				Checksum = 0;
				Datos.C.V[0] = 0;
				Datos.C.V[1] = 0;
				failcont = 0;			//borrar
				fifoADDR =	fifoADDR1;	//borrar
				do
				{
					if(WifiFIFOEmptyRX())
					{
						failcont++;		//borrar
						ExeTask();
						continue;
					} 
					if(!Datos.C.V[0])
					{
						Datos.C.V[0] = WifiFIFOReadRX();
						if	(!Datos.C.V[1])
						{
							Datos.C.V[1] = WifiFIFOReadRX();
						}
					}
					else if	(!Datos.C.V[1])
					{
						Datos.C.V[1] = WifiFIFOReadRX();
					}
					if(Datos.C.V[0] && Datos.C.V[1])
					{	
						Dato = ArrtoHex(&Datos.C.V[0]);
					}
					else
					{
						Proceso.B.fDataErr = true;
					}
					if((Datos.C.V[0]== 0x0A) || (Datos.C.V[0]== 0x0D)|| (Datos.C.V[1]== 0x0A) || (Datos.C.V[1]== 0x0D))
					{
						break;
					}
					if(Proceso.B.fDataErr)
					{
						failcont++;	//borrar
						if(!Dato)
						{
							Datos.C.V[0] = 0;
							Datos.C.V[1] = 0;
						}
						ExeTask();
					}
					else
					{
						Datos.C.V[0] = 0;
						Datos.C.V[1] = 0;
						SAVE[i] = Dato;
						Checksum += Dato;
						i++;
					}	
				}while(i<512);// && !Proceso.B.fDataErr);
				Checksum ++;
				for(i=0;i<512;i=i+128)
				{
					Dir = MemAddr + i;
					EepromWRBuf(Dir,&SAVE[i],128);
					Dly_1_MiliSec(12);
				}
				Dly_1_MiliSec(12);
				memset(SAVE,0x00,512);

				for(i=0;i<512;i=i+128)
				{
					Dir = MemAddr + i;
					EepromRDBuf(Dir,&SAVE[i],128);		
				}
				Wifi.EscDato = false;
				Proceso.B.fGrabaDts = true;
			}	
		}
//-----------------------------------------------------------------------------
// 	Comandos que llegan desde la Pantalla (modulo Wifi)
//-----------------------------------------------------------------------------	
		else
		{
			
			while(!WifiFIFOEmptyRX() && !Wifi.fCom)
			{				
				Dato = WifiFIFOReadRX();
//				U4FIFOWRITETX(Dato);
//-----------------------------------------------------------------------------
// 	Comandos del modulo Wifi
//-----------------------------------------------------------------------------
				if (Dato =='*')
				{
					if (Wifi.lMod)
					{
						Wifi.lMod = false;
						if(Check(WFcmd,"OPEN",5))
						{
							Sts_Tmr.CntWifi = 0;
							Wifi.fMod = true;
							
						}
						else if(Check(WFcmd,"CLOSE",6) ||Check(WFcmd,"CLOS",5))
						{
							Wifi.fMod = false;
							Sts_Tmr.CntWifi = 0;
						}
						if(Check(WFcmd,"HELLO",5))
						{
							Sts_Tmr.CntWifi = 0;
							Wifi.fConectado = true;
						}
					}
					else
					{
						Wifi.lMod = true;
	//					memset(WFcmd,0,sizeof(WFcmd));
						WFind = 0;
					}
				}
				else if(Wifi.lMod)
				{
					WFcmd[WFind] = Dato;
					WFind++;
				}
//-----------------------------------------------------------------------------
// 	Comandos desde el Tablero
//-----------------------------------------------------------------------------
				else
				{	
//-----------------------------------------------------------------------------
// 	Inicio del comando
//-----------------------------------------------------------------------------	
					if((Dato == '>')||(Rx3Ind >=256))
					{
						Rx3Ind = 0;
						Wifi.lCom = true;		
					}
					if(Wifi.lCom)
					{
						Rx3Buf[Rx3Ind] = Dato;	
						Rx3Ind++;
					}
					if(Dato =='<')
					{
						Proceso.B.fRxCom = true;	//Fin del bloque	
						Wifi.lCom = false;
						Wifi.fCom = true;
						break;
					}
				}
			}		
		}
//-----------------------------------------------------------------------------
//	Control de datos del modulo
//-----------------------------------------------------------------------------	
		if(Dato = WifiFIFOModulo())
		{
			if(Dato == '>')
			{
				ModuloWf.Sts.B.fCom = true;
				ModuloWf.ind = 0;
			}
			else if(ModuloWf.Sts.B.fCom)
			{
				if(Dato == 'K' || Dato == 'A' || Dato == 'V')
				{
					ModuloWf.tex[ModuloWf.ind] = Dato;
					ModuloWf.ind++;
				}
				else if (Dato == '<')
				{
					ModuloWf.Sts.B.fCom = false;
					ModuloWf.ind = 0;
					Sts_Tmr.CntWifi = 0;
				}
				else 
				{
					ModuloWf.Sts.B.fCom = false;
					ModuloWf.ind = 0;
				}
				
			}
		}


		
//-----------------------------------------------------------------------------	
//Velocidad del GPS	
//-----------------------------------------------------------------------------	
		if((GPSdts.sys.act == 'A') && (GPSdts.pos.vel >= 2.0))
		{
			VSiembra = (unsigned int)(GPSdts.pos.vel*10);
		}
		else
		{
		//Velocidad desde moduladora
			for(i=0;i<16;i++)
			{	
				if( Moduladora[i].Sts.B.Con && Moduladora[i].Vel)
				{
					VSiembra = Moduladora[i].Vel;
					break;
				}
				else 
				{
					VSiembra = 0;	
				}
			}	
		}
//		else
//		{
//		//	VSiembra = 80; 	//solo para probador estatico
//			VSiembra = 0;	//Para final
//		}
//		VSiembra = (unsigned int)(GPSdts.pos.vel*10);	//solo para probador estatico
//-----------------------------------------------------------------------------
// Transferencia de datos para comunicacion
//-----------------------------------------------------------------------------		
		if(Proceso.B.fAdqSie1 && Proceso.B.fAdqSie2 && !Sensores.STS.B.TX_SF1 && !Sensores.STS.B.TX_SF2)
		{	
			Medicion = 0;
			Bus1aDtsCom();	
			Bus2aDtsCom();
			Sensores.tSIE ++;
			if(Sensores.STS.B.fSAct1 || Sensores.STS.B.fFAct1 || Sensores.STS.B.fSAct2 || Sensores.STS.B.fFAct2 || !Medicion || (Sensores.tSIE >= kMaxEscan))
			{		
				Sensores.STS.B.TX_SF1 = true;
				Sensores.STS.B.fSAct1 = false;
				Sensores.STS.B.fFAct1 = false;
				Sensores.STS.B.fSAct2 = false;
				Sensores.STS.B.fFAct2 = false;
				Sensores.tSIE = 0;
			}
//			Sensores.STS.B.TX_SF1 = true;
			Proceso.B.fAdqSie2 = false;
			Proceso.B.fAdqSie1 = false;		
		}
		//Modificación de los ID de los Sensors
		if(Proceso.B.fSetId)
		{
			CargaId();
		}
		//Lectura de los datos de los sensores
		else if(!Proceso.B.fInicio && !Proceso.B.fConfPer)
		{
			Adq_Proc_Lin1(); 
			Adq_Proc_Lin2();
		}
		if(!Proceso.B.fStopDts)
		{
			DtoTerminal();
		}	
		SW1_Tick0010();
		SW2_Tick0010();	
		GPSdts.sys.tvid ++;	
		GPSdts.sys.tvida = GPSdts.sys.tvid;
//Si el tiempo de vida supera los diez segundos borro la velocidad
		if( GPSdts.sys.tvid > 1000)
		{
			GPSdts.pos.vel = 0;
		}		
//-----------------------------------------------------------------------------			
//Manejo del destelo de los LEDs
//-----------------------------------------------------------------------------	
//	LED_CAN = Destello(TMR_CAN,3);
	LED_WIFI = Destello(DestWf.Sec, DestWf.Duty);
	LED_WIFI_E = LED_WIFI;	
	}
//*****************************************************************************
//	Tareas que se ejecutan cada 100ms
//*****************************************************************************
	if(Sts_Tmr.B.Pls0100)
	{
		Sts_Tmr.B.Pls0100 = false;
//-----------------------------------------------------------------------------			
//Incremento de tiempo devida de la señal de GPS
//Se resetea si la señal recibida es valida
//-----------------------------------------------------------------------------	
//		GPSdts.sys.tvid++;	
//-----------------------------------------------------------------------------		
// Calculos de los metros recorridos
// Solo si la velocidad es mayor a 2.0 km/h
//-----------------------------------------------------------------------------
		if( GPSdts.pos.vel > 2.0)
		{		
			GPSdts.pos.dist +=  (GPSdts.pos.vel/3.6)/10;
	//		Resto = GPSdts.pos.lat % 100;
	//		Ent = (int) GPSdts.pos.lat / 10;
	//		Lat.Ant = Lat.Act;
	//		Lat.Act = (float) Ent + ((Resto*100)/60);
		}
		

//-----------------------------------------------------------------------------	
//Control de recepcion de datos del GPS
//Si despues de 3 segundos no recibio un dato valido lo reseteo
//-----------------------------------------------------------------------------	
//		GPSdts.sys.tGPSon ++;
//		if( GPSdts.sys.tGPSon > 100 )
//		{
//			GPSdts.sys.tGPSon = 0;
//			if(GPSsts.B.fOn)	
//			{
//				GPSsts.B.fBaud = false;
//				GPSsts.B.fOn = false;
//				Pwr_GPS 	=  false;
//				GPSdts.sys.tGPSon = 25;
//				GPSdts.sys.Baud ++;
//				if(GPSdts.sys.Baud > 6)
//				{
//					GPSdts.sys.Baud =1;
//				}
//				U1BRG = SCI_SetBaud( GPSdts.sys.Baud);
//			}
//			else
//			{
//				GPSsts.B.fOn = true;
//				Pwr_GPS 	=  true;
//			}
//		}
//-----------------------------------------------------------------------------	
//		Manejo del reset del Modulo WiFi
//		El tiempo se maneja en decimas de segundo
//-----------------------------------------------------------------------------
//El modulo esta apagado
		if(!Pwr_Wifi)
		{
			DestWf.Duty = 5;
			DestWf.Sec = 0x0000;
			
			Wifi.fConectado = false;	
			Sts_Tmr.CntWifi++;
			if(Sts_Tmr.CntWifi > 100)
			{
				Sts_Tmr.CntWifi = 0;
				Pwr_Wifi 	= true;		//Alimentacion 3v3 Modulo WiFi			
			}
		}
		else if (Wifi.fMod)
		{
//Se abrio el puerto esta transmitiendo datos	
			if(Wifi.fConectado)
			{
				DestWf.Duty = 25;
				DestWf.Sec = 0x00AA;
			}
//No recibe el KAV	
			else if (Sts_Tmr.CntWifi >40)
			{
				DestWf.Duty = 6;
				DestWf.Sec = 0x0033;
			}
//Se abrio el puerto esta transmitiendo datos
			else
			{
				DestWf.Duty = 3;
				DestWf.Sec = 0xAAAA;
			}			
			Sts_Tmr.CntWifi++;
			if(Sts_Tmr.CntWifi > 900)
			{
				Sts_Tmr.CntWifi = 0;
				Pwr_Wifi 	= false;		//Alimentacion 3v3 Modulo WiFi
				Wifi.fMod = false;
				Wifi.lMod = false;		
			}
		}
//El modulo esta encendido pero el puerto no esta abierto
		else 
		{
			DestWf.Duty = 25;
			DestWf.Sec = 0x0F0F;
		}	
	}

//*****************************************************************************
//	Tareas que se ejecutan cada 1000ms
//*****************************************************************************
	if(Sts_Tmr.B.Pls1000)
	{


		Sts_Tmr.B.Pls1000 = false; 
		Sts_Tmr.B.ADCInit = false;
		TLed++;
		if(TLed>=5)
		{
			Sensores.STS.B.DIAG = true;
			TLed = 0;			
		}
//		LED_POWER = IMPLEMENT_SW ? 1 : 0;
//Si esta cargando Id de los sensores incrementa el timer sino no		
	//	if(Proceso.B.fSetId)
	//	{
	//		SetId.TMR ++;
	//	}

//-----------------------------------------------------------------------------
//	Reset de los bit de comunicacion para que no quede colgado el programa
//-----------------------------------------------------------------------------
//		if(Proceso.B.fComWf)
//		{
//			Proceso.B.fComWf = false;
//		}
//		if(Proceso.B.fComRS)
//		{
//			Proceso.B.fComRS = false;
//		}
//-----------------------------------------------------------------------------	
//Calculo de las alarmas de los sensores de Turbina
//-----------------------------------------------------------------------------	
		for(i=0;i++;i<kTRBMax)
		{
			SenId = i;
			if(Turbina[SenId].Sts.B.Hab && Turbina[SenId].Sts.B.Det)
			{
				TRBChekAl00(SenId);
			}
		}
//-----------------------------------------------------------------------------	
//Control de los led del GPS
//-----------------------------------------------------------------------------	
		if(GPSdts.sys.act !='A' && GPSsts.B.fOn)
		{
			LED_GPS = LED_GPS ? 0 : 1;
			LED_GPS_E = LED_GPS ? 0 : 1;
		}
		else if (!GPSsts.B.fOn)
		{
			LED_GPS = 0;
			LED_GPS_E = 0;
		}
		else
		{
			LED_GPS = 1;
			LED_GPS_E = 1;
		}
//-----------------------------------------------------------------------------	
//Majejo de la alimentacion del modulo GPS y el cambio a 5Hrz
//		if(GPSsts.B.fBaud && (GPSdts.sys.Baud != 3) && GPSsts.B.fOn && Sts_Tmr.B.GPS5Hz)
//		{
//			Sts_Tmr.B.GPS5Hz = false;
//			GPSsts.B.fBaud = false;
//			GPSbaud();
//		}
//		
//		if( GPSsts.B.fLec5hz && (GPSdts.sys.Baud == 3) && GPSsts.B.fOn && Sts_Tmr.B.GPS5Hz)
//		{
//			Sts_Tmr.B.GPS5Hz = false;
//			GPSsts.B.fLec5hz = false;
//			GPS5hrz();
//		}

	}	
}

