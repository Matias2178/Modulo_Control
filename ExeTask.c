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
//	if(Wifi_RTS)
//	{
//		LED_CAN = true;
//	}
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
				CGPS.Sentencias ++;
				if(GPSsts.B.fMsgOk = GPSCheckSum(GPSmsg))
					CGPS.Correctas ++;
				else
					CGPS.Fallas ++;

				GPSInd = 0;
				break;
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

		while(!RSFIFOEmptyRX() && !RS232.B.fCom)
		{	
			Dato = RSFIFOReadRX();
			if ((Dato == '>')||(Rx232Ind>=256))
			{
				Rx232Ind = 0;
				RS232.B.lCom = true;
			}
			if(RS232.B.lCom)
			{
				Rx232Buf[Rx232Ind] = Dato;	
				Rx232Ind++;
			}
			if(Dato =='<')
			{
				Proceso.B.fRxCom = true;	//Fin del bloque
				RS232.B.fCom = true;
				RS232.B.lCom = false;
				Rx232Buf[Rx232Ind] = 0x00;	
				Rx232Ind++;	
				break;		
			}
		}
//-----------------------------------------------------------------------------
// Escritura de datos desde el modulo a la pantalla
//-----------------------------------------------------------------------------		
		if(Wifi.B.EscDato)
		{
			Sts_Tmr.CntWifi = 0;
			if(!Wifi.B.IniDato)
			{
				Wifi.B.IniDato = true;
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
					Proceso.B.fLecturaOk = false;
					if(WifiFIFOEmptyRX())
					{
						failcont++;		//borrar
						ExeTask();
						continue;
					} 
					if(!Datos.C.V[0])
					{
						Datos.C.V[0] = WifiFIFOReadRX();
					}
					else if	(!Datos.C.V[1])
					{
						Datos.C.V[1] = WifiFIFOReadRX();
					}
					if(Datos.C.V[0] && Datos.C.V[1])
					{	
						Dato = ArrtoHex(&Datos.C.V[0]);
						Proceso.B.fLecturaOk = true;
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
					else if (Proceso.B.fLecturaOk)
					{
						Datos.C.V[0] = 0;
						Datos.C.V[1] = 0;
						SAVE[i] = Dato;
						Checksum += Dato;
						i++;
						Proceso.B.fLecturaOk = false;
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
				Wifi.B.EscDato = false;
				Proceso.B.fGrabaDts = true;
			}	
		}
//-----------------------------------------------------------------------------
// 	Comandos que llegan desde la Pantalla (modulo Wifi)
//-----------------------------------------------------------------------------	
		else
		{
			
			while(!WifiFIFOEmptyRX() && !Wifi.B.fCom)
			{				
				Dato = WifiFIFOReadRX();
//-----------------------------------------------------------------------------
// 	Comandos del modulo Wifi
//-----------------------------------------------------------------------------
				if (Dato =='*')
				{
					if (Wifi.B.lMod)
					{
						Wifi.B.lMod = false;
						if(Check(WFcmd,"OPEN",5))
						{
							Sts_Tmr.CntWifi = 0;
							Wifi.B.fMod = true;
							Wifi.B.fClose = false;
							
						}
						else if(Check(WFcmd,"CLOSE",6) ||Check(WFcmd,"CLOS",5))
						{
							Wifi.B.fMod = false;
							Wifi.B.fClose = true;
							Sts_Tmr.CntWifi = 0;
							ModuloWf.cont++;
						}
						if(Check(WFcmd,"HELLO",5))
						{
							Sts_Tmr.CntWifi = 0;
							Wifi.B.fConectado = true;
						}
						if(Check(WFcmd,"Reboot",6))
						{
							Proceso.B.fRN_SoftRst = false;
							Sts_Tmr.CntWifi = 0;
							Wifi.val = 0;
							Com_DtsTask_RNConf = 0;
						}
					}
					else
					{
						Sts_Tmr.CntWifi = 0;
						Wifi.B.lMod = true;
	//					memset(WFcmd,0,sizeof(WFcmd));
						WFind = 0;
					}
				}
				else if(Wifi.B.lMod)
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
						Sts_Tmr.CntWifi = 0;
						Rx3Ind = 0;
						Wifi.B.lCom = true;		
					}
					if(Wifi.B.lCom)
					{
						Sts_Tmr.CntWifi = 0;
						Rx3Buf[Rx3Ind] = Dato;	
						Rx3Ind++;
					}
					if(Dato =='<')
					{
						Sts_Tmr.CntWifi = 0;
						Proceso.B.fRxCom = true;	//Fin del bloque	
						Wifi.B.lCom = false;
						Wifi.B.fCom = true;
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
		if(GPSdts.sys.act == 'A')
		{
			if(GPSdts.pos.vel >= 2.0)
				VSiembra = (unsigned int)(GPSdts.pos.vel*10);
			else
				VSiembra = 0;
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
//-----------------------------------------------------------------------------
// Transferencia de datos para comunicacion
//-----------------------------------------------------------------------------	

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
//		LED_WIFI = Destello(DestWf.Sec, DestWf.Duty);
		LED_WIFI = Blink(&Dest_WF);
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
		Sensores.tSIE ++;
		Sensores.tROT ++;	
		Sensores.tMOD ++;
		Sensores.tTRB ++;
		Sensores.tTOL ++;
		Sensores.tPRE ++;
//-----------------------------------------------------------------------------		
// Calculos de los metros recorridos
// Solo si la velocidad es mayor a 2.0 km/h
//-----------------------------------------------------------------------------
		if( GPSdts.pos.vel > 2.0)
		{		
			GPSdts.pos.dist +=  (GPSdts.pos.vel/3.6)/10;
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
//-----------------------------------------------
		Sts_Tmr.CntWifi++;
		if(!Pwr_Wifi)
		{
//El modulo esta apagado
		//	Sts_Tmr.CntWifi++;
			Dest_WF.Duty = 5;
			Dest_WF.Sec = 0x0000;
			Proceso.B.fApagadoRN = false;
			Wifi.B.fConectado = false;	
			if(Sts_Tmr.CntWifi > 10)
			{
				Sts_Tmr.CntWifi = 0;
				Pwr_Wifi 	= true;		//Alimentacion 3v3 Modulo WiFi			
			}
		}
		else 
		{
//-----------------------------------------------
//Modulo Encendido
			if((!Proceso.B.fWifiConf)&&(Sts_Tmr.CntWifi > 600))
			{
//-------------------------------------------------------------------------
//Apagado del modulo si en 60 segundos no se abrio el puerto y recibe datos	
				Proceso.B.fRN_SoftRst = true; //Activo el reset por software
				if (Sts_Tmr.CntWifiRst >= 3)
				{
					Sts_Tmr.CntWifi = 0;
					Pwr_Wifi 	= false;		//Alimentacion 3v3 Modulo WiFi
					RN171_Desc++;
					Wifi.B.lMod = false;
					Wifi.B.fMod = false;
					Proceso.B.fRN_SoftRst = false;
					Sts_Tmr.CntWifiRst = 0;
					Com_DtsTask_RNConf = 0;
				}			
			}
		
			if (Wifi.B.fMod)
			{
//Puerto Abierto
//Sts_Tmr.CntWifi++;
		
				if (Sts_Tmr.CntWifi >50)
				{
//-------------------------------------
//No recibe el KAV	
	   				Dest_WF.Duty = 6;
					Dest_WF.Sec = 0x0033;
					if(!Wifi.B.fKAV)
					{
						Wifi.B.fKAV = true;
						Wifi.B.fKAVx = true;
						KAV_cont++;
					}
				}
	
				else
				{
//-------------------------------------
//Se abrio el puerto esta transmitiendo datos
	
					Proceso.B.fApagadoRN = false;
					Dest_WF.Duty = 3;
					Dest_WF.Sec = 0xAAAA;
					Wifi.B.fKAV = false;
				}
				if((!Proceso.B.fWifiConf)&&(Sts_Tmr.CntWifi > 66))//6
				{
//-----------------------------------------------------------------------------
//Apagado del modulo si en 3,3 seg con el puerto abierto no recibio datos
					//Activo el reinicio por software
					Proceso.B.fRN_SoftRst = true;				
					Sts_Tmr.CntWifi = 66;	//para que no halla desbordamiento mientras hago el reset
					if (Sts_Tmr.CntWifiRst >= 3)
					{
						Sts_Tmr.CntWifiRst = 0;
						RN171_Desc++;
						Wifi.B.lMod = false;
						Wifi.B.fMod = false;
						Sts_Tmr.CntWifi = 0;
						Pwr_Wifi 	= false;		//Alimentacion 3v3 Modulo WiFi	
						Proceso.B.fRN_SoftRst = false;	//No se pudo resetear por soft
						Sts_Tmr.CntWifiRst = 0;	
						Com_DtsTask_RNConf = 0;
					}
				}	
			}
			else if (Wifi.B.fClose && !Wifi.B.fMod)
			{
//-----------------------------------------------
//Se Abrio y se cerro el puerto
				Sts_Tmr.CntWifi++;
	//			LED_CAN = true;
				Proceso.B.fApagadoRN = true;
				Dest_WF.Duty = 30;
				Dest_WF.Sec = 0x0333;
				if(Sts_Tmr.CntWifi > 100)
				{
					Sts_Tmr.CntWifi = 0;
					Pwr_Wifi 	= false;		//Alimentacion 3v3 Modulo WiFi
					Wifi.B.lMod = false;	
					Wifi.B.fClose = false;
					RN171_Desc++;
					EepromWRBuf(M_RN171_OFF,&RN171_Desc,sizeof(RN171_Desc));		
				}
			}
	
			else if(Wifi.B.fConectado)
			{
//-----------------------------------------------
//Se conecto un dispositivo al modulo
				Dest_WF.Duty = 25;
				Dest_WF.Sec = 0x00AA;
			}

			else 
			{
//-----------------------------------------------
//El modulo esta encendido 
	//No hay dispositivos conectados				
	//			LED_CAN = LED_CAN ? true : false;
				Dest_WF.Duty = 25;
				Dest_WF.Sec = 0x0F0F;
			}
		}
//-----------------------------------------------------------------------------
		
//-----------------------------------------------------------------------------
//Envio el dato Wait cada 500 mseg
//-----------------------------------------------------------------------------
		if((Proceso.B.fInicio || Proceso.B.fConfPer) && Sts_Tmr.B.WaitPls)	
		{
			DtoTerminal();	
		}
	}

//*****************************************************************************
//	Tareas que se ejecutan cada 1000ms
//*****************************************************************************
	if(Sts_Tmr.B.Pls1000)
	{
//	espera++;
//		if(espera>20 && espera< 32 )
//		{
//		if(Wifi_Conf)
//		{
//				Wifi_Conf = false;
//				LED_CAN = true;
//		}		
//		else
//		{
//				Wifi_Conf = true;
//				LED_CAN = false;
//		}
//		}
		
		Sts_Tmr.B.Pls1000 = false; 
		Sts_Tmr.B.ADCInit = false;
		TLed++;
		if(TLed>=5)
		{
			Sensores.STS.B.DIAG = true;
			TLed = 0;			
		}
//Si esta cargando Id de los sensores incrementa el timer sino no		
	//	if(Proceso.B.fSetId)
	//	{
	//		SetId.TMR ++;
	//	}
//-----------------------------------------------------------------------------	
//Calculo de las alarmas de los sensores de Turbina
//-----------------------------------------------------------------------------	
//		for(i=0;i++;i<kTRBMax)
//		{
//			SenId = i;
//			if(Turbina[SenId].Sts.B.Hab && Turbina[SenId].Sts.B.Det)
//			{
//				TRBChekAl00(SenId);
//			}
//		}
//		i = 0;
//		for(i=0;i++;i<kPREMax)
//		{
//			SenId = i;
//			if(Presion[SenId].Sts.B.Hab && Presion[SenId].Sts.B.Det)
//			{
//				PREChekAl00(SenId);
//			}
//		}
		
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
		Sts_Tmr.TMRModB2 ++;
		Sts_Tmr.TMRModB1 ++;
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

