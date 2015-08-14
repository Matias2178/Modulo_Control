/*****************************************************************************
 *	FUNCIONES DE ACONDICIONAMIENTO DE DATOS EN LOS BUFFER DE TRANSMICION
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
 	#include	"VarGlob.h"
	#include	"Constantes.h"
	#include 	"UART.h"
	#include 	"GPS.h"
	#include 	"Comunicacion.h"
	#include 	"Comandos.h"
	#include 	"FunMat.h"
	#include	<string.h>
	
	
unsigned char i;	
unsigned char * Dispositivo(unsigned char *S,unsigned char Id);	
unsigned char * CargaSN8(unsigned char *S);
/******************************************************************************
*	Funcion:		DtoTerminal()
*	Descricpion:	Transimite en forma secuencial los datos por el puerto 
*					serie al modulo Wifi 
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void DtoTerminal(void)
{	
	unsigned int i, Dir;
	unsigned char *S;
	unsigned char Val[2];
			
	memset(ComBuf,0,260);
	Proceso.B.fT2xCOM = false;
	Proceso.B.fT3xCOM = false;
	S = ComBuf;
	if(!U2STAbits.TRMT || !U3STAbits.TRMT)
			return;
	if(Wifi.LeeDato)
	{
		
		switch(Com_DtsTask_ROT010)
		{
		default:
			Com_DtsTask_ROT010 = 0;
		case 0:
			Checksum = 0;
			for(i=0;i<512;i=i+128)
			{
				Dir = MemAddr + i;
				EepromRDBuf(Dir,&SAVE[i],128);		
			}
			Dir = 0;
			for(i=0;i<512;i++)
			{
				Checksum = Checksum + SAVE[i];
			}
			Checksum++;
			strcpy((char*)S,">RMEM");
			S=S + strlen(">RMEM");
			*S = ',';
			S++;
			S = (unsigned char*)uitos(MemDatos,S);
			*S = ',';
			S++;
			S = (unsigned char*)InHextoArr(Checksum, S);
			*S = '<';
			
			Com_DtsTask_ROT010++;	
			
		break;	
		case 1:
			Dir = MemAddr + Com_DtsTask_ROT010;
	//		EepromRDBuf(Dir,SAVE,128);
	//		HextoArr(SAVE,ComBuf,128);
			HextoArr(&SAVE[0],ComBuf,128);
			
			Com_DtsTask_ROT010++;
					
		break;	
		case 2:
			Dir = MemAddr + (128 * Com_DtsTask_ROT010);
	//		EepromRDBuf(Dir,SAVE,128);
	//		HextoArr(SAVE,ComBuf,128);
			HextoArr(&SAVE[128],ComBuf,128);
			Com_DtsTask_ROT010++;
					
		break;
		case 3:
			Dir = MemAddr + (128 * Com_DtsTask_ROT010);
	//		EepromRDBuf(Dir,SAVE,128);
	//		HextoArr(SAVE,ComBuf,128);
			HextoArr(&SAVE[256],ComBuf,128);
			Com_DtsTask_ROT010++;
					
		break;
		case 4:
			Dir = MemAddr + (128 * Com_DtsTask_ROT010);
	//		EepromRDBuf(Dir,SAVE,128);
	//		HextoArr(SAVE,ComBuf,128);
			HextoArr(&SAVE[384],ComBuf,128);
		
			ComBuf[256] = 0x0D;
			ComBuf[257] = 0x0A;
			
			//Com_DtsTask_ROT010++;	
			Com_DtsTask_ROT010=0;
			Wifi.LeeDato = false;
		break;	
		}
		goto lComSinCheck;					
	}
	else if (Proceso.B.fApagadoRN && Sts_Tmr.B.WaitPls)
	{
		Sts_Tmr.B.WaitPls = false;
		S += Movstr(S,"*** Apangando RN171 en  ");
		i = (300 - Sts_Tmr.CntWifi) ;
//		S  = (unsigned char*)uitos(i,S);
		S  = (unsigned char*) ditos(i,S,3,1);
		S += Movstr(S," segundos *** ");
		*S = 0x0D;
		S++;
		*S = 0x0A;
		S++;
	}
	else if(Proceso.B.fInicio || Proceso.B.fConfPer)
	{
		if(Sts_Tmr.B.WaitPls)
 		{
			Sts_Tmr.B.WaitPls = false;
			strcpy((char*)ComBuf,"<WAIT>,");
		}
		else
			return;	
	}
	else if(Proceso.B.fSetId)
	{
		if(Sts_Tmr.B.SetID0500)
		{
			Sts_Tmr.B.SetID0500 = false;
			SetNumId("<SETID>",ComBuf);				
		}
		else
			return;
	}
	else if(U2STAbits.TRMT && U3STAbits.TRMT)
	{
		if(Proceso.B.fImpSw ^ Sts_Tmr.B.ImpSw)
		{
			Proceso.B.fImpSw = Sts_Tmr.B.ImpSw;
			if(Proceso.B.fImpSw)
			{		
				strcpy((char*)ComBuf,"<IMPSW>,UP");
			}
			else
			{
				strcpy((char*)ComBuf,"<IMPSW>,DOWN");
			}
		}
		else if (Proceso.B.fGrabaDts)
		{
			Proceso.B.fGrabaDts = false;
			CheckGrabaMem(">RMEM",Checksum,ComBuf);
			goto lComSinCheck;
		}	
		else if(Proceso.B.fGPSDtOk | Proceso.B.Puto)
		{
			tlecgpstx = 0;
			
			if(!Proceso.B.fRePuto)
			{
				GPSDatos("<GPSDT>",ComBuf);
				Proceso.B.fRePuto = true;
			}
			else
			{	
				GPSTMR("<GPSTMR>",ComBuf);
				Proceso.B.Puto = false;
				Proceso.B.fGPSDtOk = false;
				Proceso.B.fRePuto = false;
			}	
				
		}
		else if (Sensores.STS.B.TX_MOD && !Sensores.STS.B.MOD_Ax)
		{
			Sensores.STS.B.TX_MOD = false;
			Sensores.STS.B.MOD_Ax = true;
			//MedPerifericos ("<ALMOD>",DtsPerCom,ComBuf,1);
			StsPerifericos ("<SPMOD>",DtsPerCom,ComBuf,1);
		}
		else if (Sensores.STS.B.TX_TRB && !Sensores.STS.B.TRB_Ax)
		{
			switch(Com_DtsTask_TRB010)
			{	
			default:
				Com_DtsTask_TRB010 = 0;		
			case 0:
				MedPerifericos ("<MPTRB>",DtsPerCom,ComBuf,3);
				Com_DtsTask_TRB010++;
			break;
			case 1:
				StsPerifericos ("<SPTRB>",DtsPerCom,ComBuf,3);
				Sensores.STS.B.TX_TRB = false;
				Com_DtsTask_TRB010 = 0;
				Sensores.STS.B.TRB_Ax = true;
			break;
			}			
		}
		else if (Sensores.STS.B.TX_ROT && !Sensores.STS.B.ROT_Ax)
		{
			switch(Com_DtsTask_ROT010)
			{
			default:
				Com_DtsTask_ROT010 = 0;		
			case 0:
				MedPerifericos ("<MPROT>",DtsPerCom,ComBuf,2);
				Com_DtsTask_ROT010++;
			break;
			case 1:
				StsPerifericos ("<SPROT>",DtsPerCom,ComBuf,2);
				Sensores.STS.B.TX_ROT = false;
				Com_DtsTask_ROT010 = 0;
				Sensores.STS.B.ROT_Ax = true;
			break;
			}	
		}
		else if(Sensores.STS.B.TX_TOL && !Sensores.STS.B.TOL_Ax)
		{
			Sensores.STS.B.TX_TOL = false;
			StsPerifericos ("<SNTOL>",DtsPerCom,ComBuf,4);
			Sensores.STS.B.TOL_Ax = true;
		}
		
//Esto se envia cada 5 segundos
		else if(Sensores.STS.B.DIAG && !Sensores.STS.B.DIAG_Ax)
		{
			switch(Com_DtsTask_DIAG10)
			{	
				default:
					Com_DtsTask_DIAG10 = 0;		
				case 0:
					Diagnostico("<DIAGN>",ComBuf);
					Com_DtsTask_DIAG10++;
				break;
				case 1:
					Encabezado("<ENCAB>",ComBuf);
					Com_DtsTask_DIAG10++;
				break;
				case 2:
					if(Proceso.B.fImpSw)
						strcpy((char*)ComBuf,"<IMPSW>,UP");
					else
						strcpy((char*)ComBuf,"<IMPSW>,DOWN");
					Com_DtsTask_DIAG10 = 0;
					Sensores.STS.B.DIAG = false;
					Sensores.STS.B.DIAG_Ax = true;
				break;
//			if(!Proceso.B.fDtsSat)
//				{
//					GPSSatGan("<GPSTG>",ComBuf);
//				}
			}
		} 
		else if (Sensores.STS.B.TX_SF1 && !Sensores.STS.B.SSF_Ax)
		{
			switch(Com_DtsTask_Sen010)
			{	
			default:	
				Com_DtsTask_Sen010 = 0;	
			case 0:
				MedSensores ("<MSSB1>",DtsComBus1,ComBuf,1);
				Com_DtsTask_Sen010++;
			break;
			case 1:
				MedSensores ("<MSFB1>",DtsComBus1,ComBuf,2);
				Com_DtsTask_Sen010++;
			break;
			case 2:
				StsSensores ("<SSSB1>",DtsComBus1,ComBuf,1);
				Com_DtsTask_Sen010++;
			break;
			case 3:
				StsSensores ("<SSFB1>",DtsComBus1,ComBuf,2);
				Com_DtsTask_Sen010++;
				
			break;	
			case 4:
				MedSensores ("<MSSB2>",DtsComBus2,ComBuf,1);
				Com_DtsTask_Sen010++;
			break;
			case 5:
				MedSensores ("<MSFB2>",DtsComBus2,ComBuf,2);
				Com_DtsTask_Sen010++;
			break;
			case 6:
				StsSensores ("<SSSB2>",DtsComBus2,ComBuf,1);
				Com_DtsTask_Sen010++;
			break;
			case 7:
				StsSensores ("<SSFB2>",DtsComBus2,ComBuf,2);
			//	Com_DtsTask_Sen010= 0;
				Com_DtsTask_Sen010++;
				Sensores.STS.B.TX_SF1 = false;
		//		Sensores.STS.B.TX_SF2 = true;
//				Sensores.STS.B.TX_SF2 = false;
				Sensores.STS.B.SSF_Ax = true;
			break;
//--------------------------------------------------------------------------------------
			case 8:
				//TmrBusLin("<TLIN1>",TLin1,ComBuf,TDispLin1,TDispActLin1,TDispErrLin1);
				TmrBusLin("<TLIN1>",TLin1,ComBuf);
				Com_DtsTask_Sen010++;	
			break;		
			case 9:
			//	TmrBusLin("<TLIN2>",TLin2,ComBuf,TDispLin2,TDispActLin2,TDispErrLin2);
				TmrBusLin("<TLIN2>",TLin2,ComBuf);
				Com_DtsTask_Sen010 = 0;		
			break;			
//---------------------------------------------------------------------------------------
			}	
		}
		else
		{
			Sensores.STS.C.Aux = 0;
			return;
		}
	}
	else
		return;
lFinComunicacion:
		Nop();
		TxCheckSum(ComBuf);
lComSinCheck:
		strcpy((char*)U2TxBuf,(char*)ComBuf);
		U2TxInd=0;
		U2TXREG = U2TxBuf[0];
	
		strcpy((char*)U3TxBuf,(char*)ComBuf);
		U3TxInd=0;
		U3TXREG = U3TxBuf[0];
}

/******************************************************************************
*	Funcion:		CRNL()
*	Descricpion:	Inserta en el buffer de comunicaciones los caracteres de 
*					terminacion de linea
*	Ingreso Datos:	Direccion del buffer
*	Salida Datos:	Ninguno
******************************************************************************/	
void CRNL(unsigned char *S)
{
		S--;
		*S = 0x0A;
		S++;
		*S = 0x0D;
		S ++;
		*S = 0;	
}
	
/******************************************************************************
*	Funcion:		MedSensores()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					de medicion separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Semilla 0 Fertilizante
*	Salida Datos:	Ninguno
******************************************************************************/			
void MedSensores (char *lb, struct _DtsCom Datos,unsigned char *S,int T)
 {
	int i;
	unsigned int Med;

	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	for(i=0;i<32;i++)
	{
		if(T==1)
		{
			Med = Datos.Sie.Med[i];
		}
		else
		{
			Med = Datos.Fer.Med[i];
		}	
		S = uitos(Med,S);
		*S = ',';
		S++;
	}
		CRNL(S);
}

/******************************************************************************
*	Funcion:		CheckGrabaMem()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					de estado delos perifericos separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Moduladora 2 Rotacion 3 Turbina
*	Salida Datos:	Ninguno
******************************************************************************/			
void CheckGrabaMem(char *lb,unsigned int Check,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	S = (unsigned char*)uitos(MemDatos,S);
	*S = ',';
	S++;
	S = (unsigned char*)InHextoArr(Check, S);
	*S = ',';
	S++;
	*S = '<';
	S++; 
	*S = 0x0D;
	S++;
	*S = 0x0A;
}

/******************************************************************************
*	Funcion:		StsSensores()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					de estado de los sensores separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Semilla 0 Fertilizante
*	Salida Datos:	Ninguno
******************************************************************************/	
void StsSensores (char *lb, struct _DtsCom Datos,unsigned char *S,int T)
 {
	int i;
	unsigned int Med;
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	for(i=0;i<32;i++){
		if(T==1)
		{
			Med = Datos.Sie.Sts[i]; 
		}
		else
		{
			Med = Datos.Fer.Sts[i];
		}	
		S = (unsigned char*)uitos(Med,S);
		*S = ',';
		S++;
	}
		CRNL(S);
}


/******************************************************************************
*	Funcion:		MedPerifericos()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					de medicion de los perifericos separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Moduladora 2 Rotacion 3 Turbina
*	Salida Datos:	Ninguno
******************************************************************************/			
void MedPerifericos (char *lb, struct _DtsPerCom Datos,unsigned char *S,int T)
 {
	int i;
	unsigned int Med;

	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	if(T==1)
	{
		//Moduladora
		for(i=0;i<16;i++)
		{
			Med = Datos.MOD.Al[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}
	else if(T==2)
	{
		//Sensores de rotacion/tolva
		for(i=0;i<8;i++)
		{
			Med = Datos.ROT.Med[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}
	else
	{	
		//Sensores Turbina
		for(i=0;i<3;i++)
		{
			Med = Datos.TRB.Med[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	
	}	
		CRNL(S);
}


/******************************************************************************
*	Funcion:		StsPerifericos()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					de estado delos perifericos separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Moduladora 2 Rotacion 3 Turbina
*	Salida Datos:	Ninguno
******************************************************************************/			
void StsPerifericos (char *lb, struct _DtsPerCom Datos,unsigned char *S,int T)
 {
	int i;
	unsigned int Med;

	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	if(T==1)
	{
		//Moduladora
		for(i=0;i<16;i++)
		{
			Med = Datos.MOD.Sts[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}
	else if(T==2)
	{
		//Sensores de rotacion/tolva
		for(i=0;i<8;i++)
		{
			Med = Datos.ROT.Sts[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}
	else if(T==3)
	{	
		//Sensores Turbina
		for(i=0;i<3;i++)
		{
			Med = Datos.TRB.Sts[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}
	else if(T==4)
	{	
		//Sensores Nivel de Tolva
		for(i=0;i<16;i++)
		{
			Med = Datos.TOL.Sts[i];
			S = uitos(Med,S);
			*S = ',';
			S++;
		}
	}		
		CRNL(S);
}

/******************************************************************************
*	Funcion:		GPSDatos()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los datos 
*					del GPS separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de sensores
*					Puntero del buffer de transmicion
*					Tipo de sensor 1 Semilla 0 Fertilizante
*	Salida Datos:	Ninguno
******************************************************************************/	
void GPSDatos(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;

	*S = GPSdts.sys.act;				//GPS Activo
	S = *S ? (S+1) : S;
//	S++;
	*S = ',';
	S++;
	S  = uitos(GPSdts.sys.tvida,S);	//tiempo de vida de la señal
	*S = ',';
	S++;
	S  = uitos(GPSdts.sys.sat,S);		//Cantidad de satelites activos
	*S = ',';
	S++;
	S  = uitos(GPSdts.sys.gan,S);		//Ganancia promedio de los satelies
	*S = ',';
	S++;
	S  = ftos(GPSdts.sys.pres,S,3,2);	//Presicion
	*S = ',';
	S++;
	S += (Movstr(S,&GPSdts.pos.latc));
//	S  = ftos(GPSdts.pos.lat,S,4,4);	//Latitud
	*S = ',';
	S++;
	*S = GPSdts.pos.NS;				//Emisferio
	S++;
	*S = ',';
	S++;
	S += (Movstr(S,&GPSdts.pos.lonc));
//	S  = ftos(GPSdts.pos.lon,S,4,4);	//Longitud
	*S = ',';
	S++;
	*S = GPSdts.pos.EO;				//Meridiano
	S++;
	*S = ',';
	S++;
	S  = ftos(GPSdts.pos.vel,S,3,1);	//Velocidad
	*S = ',';
	S++;
	S  = ftos( GPSdts.pos.rmb ,S,3,1);	//Rumbo
	S++;
	CRNL(S);
}	
//------------------------------------------------------------
//Temporizador de datos del GPS	
void GPSTMR(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	S  = itos(GPSdts.fecha.d,S,2);
	*S = '/';
	S++;	
	S  = itos(GPSdts.fecha.m,S,2);
	*S = '/';
	S++;
	S  = itos(GPSdts.fecha.a,S,2);
	*S = ',';
	S++;
	S  = itos(GPSdts.hora.h,S,2);
	*S = ':';
	S++;
	S  = itos(GPSdts.hora.m,S,2);
	*S = ':';
	S++;
	S  = itos(GPSdts.hora.s,S,2);
	*S = ',';
	S++;
	S  = uitos(ModuloWf.cont,S);
	*S = ',';
	S++;
	S  = ultos(RN171_Desc,S);
	*S = ',';
	S++;
	CRNL(S);
}
	
//----------------------------------------------------------------------------
//						ENCABEZADO DEL BLOQUE DE DATOS
//	DESCRIPCION:Imprime el encabezado del bloque de datos que se envian 
//				al modulo contien nombre del equipo fecha y hora del GPS
//		
//	
//	ENTRADA:	Etiqueda de 
//				Puntero al array a cargar
//				
//  RETORNO:	Direccion del puntero incrementada
//----------------------------------------------------------------------------
/******************************************************************************
*	Funcion:		Encabezado()
*	Descricpion:	Carga en el encabezado en el buffer de transmicion
*	Ingreso Datos:	Direccion del Encabezado
*					Direccion del buffer de tranmicion
*	Salida Datos:	Ninguno
******************************************************************************/	
void Encabezado(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
//	strcat((char*)S,lNombre);
//	S=S + strlen(lNombre);
	for(i=0;i<20;i++)
	{
		if(!Nombre[i])
			break;
		*S = Nombre[i];
		S++;
	}
	*S = ',';
	S++;
	S  = itos(SoftVerH,S,2);
	*S = '.';
	S++;
	S  = itos(SoftVerL,S,2);
	*S = '.';
	S++;
	S  = uitos(Relase,S);
	*S = ',';
	S++;

//	strcat(S,"FechaVer:");
//	S=S + strlen("FechaVer:");
	S  = itos(FVerD,S,2);
	*S = '/';
	S++;
	S  = itos(FVerM,S,2);
	*S = '/';
	S++;
	S  = itos(FVerA,S,4);
	*S = ',';
	S++;	
	strcat((char*)S,"Fecha:");
	S=S + strlen("Fecha:");
	S  = itos(GPSdts.fecha.d,S,2);
	*S = '/';
	S++;	
	S  = itos(GPSdts.fecha.m,S,2);
	*S = '/';
	S++;
	S  = itos(GPSdts.fecha.a,S,2);
	*S = ',';
	S++;
	strcat((char*)S,"Hora:");
	S=S + strlen("Hora:");
	S  = itos(GPSdts.hora.h,S,2);
	*S = ':';
	S++;
	S  = itos(GPSdts.hora.m,S,2);
	*S = ':';
	S++;
	S  = itos(GPSdts.hora.s,S,2);
	*S = ',';
	S++;
	strcat((char*)S,"Bat:");
	S=S + strlen("Bat:");
	S  = ftos(Tension,S,2,2); 
//	*S = ',';
///	S++;
//	S  = itos(Valor,S,5);
	S++;
	CRNL(S);
	
}	
/******************************************************************************
*	Funcion:		Diagnostico()
*	Descricpion:	Carga en el encabezado en el buffer de transmicion
*	Ingreso Datos:	Direccion del Encabezado
*					Direccion del buffer de tranmicion
*	Salida Datos:	Ninguno
******************************************************************************/	
void Diagnostico(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
//Version de Software
	S  = itos(SoftVerH,S,2);
	*S = '.';
	S++;
	S  = itos(SoftVerL,S,2);
	*S = '.';
	S++;
	S  = uitos(Relase,S);
	*S = ',';
	S++;
//Numero de serie

	S = CargaSN8(S);

//	S = ultos(SerialNum,S);
	*S = ',';
	S++;
//Tension en la ECU
	S  = ftos(Tension,S,2,2); 
	*S = ',';
	S++;
//Hectareas sembradas
	S = ultos( DtsSiembra.SupSemTot,S);
	*S = ',';
	S++;
//Km recorridos 
	S = ultos( DtsSiembra.DistRecTot,S);
	*S = ',';
	S++;
//Hrs Trabajadas
	S = ultos( DtsSiembra.tTrabTot,S);
	*S = ',';
	S++;
	*S = 'G';
	S++;
	*S = 'P';
	S++;
	*S = 'S';
	S++;
	*S = ',';
	S++;
//Sentencias Recibias
	S = ultos( CGPS.Sentencias,S);
	*S = ',';
	S++;
//Sentencias Correctas
	S = ultos( CGPS.Correctas,S);
	*S = ',';
	S++;
//Sentencias Fallas
	S = ultos( CGPS.Fallas,S);
	*S = ',';
	S++;
//Sentencias GPRMC
	S = ultos( CGPS.GPRMC,S);
	*S = ',';
	S++;
//Sentencias GPGGA
	S = ultos( CGPS.GPGGA,S);
	*S = ',';
	S++;
//Sentencias GPGSV
	S = ultos( CGPS.GPGSV,S);
	S++;
	CRNL(S);
	
}
/******************************************************************************
*	Funcion:		GPSSatGan()
*	Descricpion:	Inserta en el buffer de comunicaciones los caracteres de 
*					terminacion de linea
*	Ingreso Datos:	Encabezado de datos del buffer
*					Direccion del buffer
*	Salida Datos:	Ninguno
******************************************************************************/	
void GPSSatGan(char *lb,unsigned char *S)
{
	int i;
	int Sat;
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	S  = itos(GPSdts.sys.satview,S,2);	//Cantidad de satelites activos
	*S = ',';
	S++;
	for(i=0;i<32;i++)
	{
		if(!SatInfo2[i].Gan)
			continue;
		Sat = i +1;
		S  = itos(Sat,S,2);	//Cantidad de satelites activos
		*S = ',';
		S++;
		S  = itos(SatInfo2[i].Elv,S,2);	//Elevacion
		*S = ',';
		S++;
		S  = itos(SatInfo2[i].Azt,S,3);	//Azimut
		*S = ',';
		S++;
		S  = itos(SatInfo2[i].Gan,S,2);	//Ganancia
//		*S = ',';
		S++;
		}
		CRNL(S);
		
}

/******************************************************************************
*	Funcion:		SetNumId()
*	Descricpion:	Envia datos para la carga de los ID de los sensores
*					
*	Ingreso Datos:	Encabezado de datos del buffer
*					Direccion del buffer
*	Salida Datos:	Ninguno
******************************************************************************/	
void SetNumId(char *lb,unsigned char *S)
{
	int i;
	int Num;
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	S = Dispositivo(S,SetId.Id);
	*S = ',';
	S++;
	if((SetId.IdMax | SetId.IdMin)&(SetId.IdMax!=0xFF | SetId.IdMin!=0xFF))
	{
		if( SetId.Id == SetId.VoidId)
		{
			Num = 0;
		}
		else
		{
			Num = (int) (SetId.Id - SetId.IdMin);
			Num++;
		}
	}
	else if ( (Num = ModIdNum (SetId.Id)))
	{
		Num = Num;
	}
	else
	{
		Num = 0;
	}
	S  = uitos(Num,S);	
//	*S = ',';
	S++;
	CRNL(S);
		
}
/******************************************************************************
*	Funcion:		Dispositivo()
*	Descricpion:	Carga en el String la etiqueta que le corresponde a cada 
*					Tipo de sensor
*	Ingreso Datos:	Encabezado de datos del buffer
*					Direccion del buffer
*	Salida Datos:	Ninguno
******************************************************************************/	
unsigned char * Dispositivo(unsigned char *S,unsigned char Id)
{
	//Bus 1 conectado	
	if( SetId.IdMax == 0xFF && SetId.IdMin == 0xFF)
	{
		strcpy((char*)S,"Desconectar");
		S=S + strlen("Desconectar");
	}
//Sensor conectado no modifica ID
	else if (ModIdNum (Id))
	{
		strcpy((char*)S,"MOD");
		S=S + strlen("MOD");
	}
//Bus 1 conectado	
	else if( !SetId.IdMax && !SetId.IdMin )
	{
		strcpy((char*)S,"Conectar");
		S=S + strlen("Conectar");
	} 	
	//Sensores de semillas
	else if((Id>=0x00 && Id<=0x1F) || Id == 0xF0 )
	{	
		strcpy((char*)S,"SEM");
		S=S + strlen("SEM");
	}
	else if((Id>=0x20 && Id<=0x3F))
	{	
		strcpy((char*)S,"FER");
		S=S + strlen("FER");
	}
	//Sensor de Rotacion
	else if((Id>=0x40 && Id<=0x47) || Id == 0xF1 )
	{	
		strcpy((char*)S,"ROT");
		S=S + strlen("ROT");
	}
	//Sensor de nivel de tolva
	else if((Id>=0x48 && Id<=0x57) || Id == 0xF2 )
	{	
		strcpy((char*)S,"TLV");
		S=S + strlen("TLV");
	}
	//Sensor de nivel de turbina
	else if((Id>=0xD3 && Id<=0xD5))
	{	
		strcpy((char*)S,"TRB");
		S=S + strlen("TRB");
	}
	else
	{
		strcpy((char*)S,"DSC");
		S=S + strlen("DSC");
	}

	return (unsigned char *)S;	
}
/******************************************************************************
*	Funcion:		TxCheckSum()
*	Descricpion:	Calcula e inserta en el buffer de comunicaciones los 
*					valores de CheckSum
*	Ingreso Datos:	Direccion del buffer
*					
*	Salida Datos:	Ninguno
******************************************************************************/	
void TxCheckSum(unsigned char * S)
{
	unsigned char Check;
	unsigned char ChSum;
	unsigned int Pos;
	unsigned fin;
	
	Check = 0;
	Pos = 0;
	fin=0;
	while(*S!='<'&& *S!='>')S++; 
	
	S++;
	
	while(*S!=';' && *S!=0x0A  && *S!=0x0D  && Pos<255 && *S )
	{
		if(*S==0x03  || *S==0x04)
		{
			fin =1;
			break;
		}
		Check ^= (*S & 0x7F);
		S++;
		Pos++;
	}
	*S= '*';
	S++;
	ChSum = (Check & 0xF0) >> 4;
	if(ChSum > 9)
	{
		ChSum -= 10;
		ChSum += 'A';
	}
	else
		ChSum += '0';
	*S=ChSum;	
	S++;
	ChSum = Check & 0x0F;		
	if(ChSum > 9)
	{
		ChSum -= 10;
		ChSum += 'A';
	}
	else
		ChSum += '0';
		
	*S=ChSum;
	S++;
//	if(fin)
//	{
//		*S = 0x03;
//		S++;
//		*S = 0x04;
//		S++;
//	}
//-------------------------------------------------
	Secuenciometro ++;
	*S = ',';
	S++;
	S  = (unsigned char*)uitos(Secuenciometro,S);
//-------------------------------------------------
	*S = 0x0D;
	S++;
	*S = 0x0A;
	S++;
	*S = 0x00;
}


/******************************************************************************
*	Funcion:		FindelaCita()
*	Descricpion:	Inserta los valores de fin de bloque de transmicion nueva
*					linea y retorno de carro y valor en cero del utimo dato a 
*					tranmitir
*
*	Ingreso Datos:	Direccion del buffer				
*	Salida Datos:	Ninguno
******************************************************************************/	
void FindelaCita(unsigned char* S)
{
	while(*S && *S!=0x0A && *S!=0x0A)
		S++;
		
//	*S = 0x03;
//	S++;
//	*S = 0x04;
//	S++;
//	S++;
	*S = 0x0D;
	S++;
	*S = 0x0A;
	S++;
	*S = 0x00;	
}
/******************************************************************************
*	Funcion:		TmrBusLin()
*	Descricpion:	Carga en el buffer de transmicion el encabezado y los
*					tiempos de las diez ultimos ciclos de lectura del buz
*					separados por coma
*	Ingreso Datos:	Encabezado de los datos a transmitir
*					Estructura de datos de los temporizadores
*					Puntero del buffer de transmicion
*	Salida Datos:	Ninguno
******************************************************************************/			
//void TmrBusLin (char *lb, struct _TLin Tmp,unsigned char *S, char Disp, char Act, char Err)
void TmrBusLin (char *lb, struct _TLin Tmp,unsigned char *S)
 {
	int i;
	unsigned int Med;
	unsigned int Prom;

	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;

//	*S = '(';
//	S++;
//	S = itos(Disp,S,2);
//	*S = ')';
//	S++;
//	*S = ',';
//	S++;
//	*S = '(';
//	S++;
//	S = itos(Act,S,2);
//	*S = ')';
//	S++;
//	*S = ',';
//	S++;
//	*S = '(';
//	S++;
//	S = itos(Err,S,2);
//	*S = ')';
//	S++;
//	*S = ',';
//	S++;
	i = Tmp.ind;
	Prom =0;
	do{

			Med = Tmp.Ult[i];
			Prom = Prom + Med;
			S = itos(Med,S,5);
			*S = ',';
			S++;
			i++;
			if(i>=10)
				i = 0;
		
	}while(i!= Tmp.ind);
	S = itos(Prom,S,5);
	*S = ',';
	S++;
	CRNL(S);
}


unsigned char * CargaSN8(unsigned char *S)
{
	unsigned char Tex[10];
	unsigned char i;
	unsigned char a;
	unsigned char b;
	b=0;	
	memset(Tex,0x00,10);
	
	ultos(SerialNum,Tex);
	for(i=0;i<10;i++)
	{
		if(Tex[i]==',' || !Tex[i])
			break;
	}
	if(i>8)
		i=8;
	a = 8 -i;
	for( i=0;i<8;i++)
	{
		if(i<a)
		{
			*S = '0';
		}
		else
		{
			*S = Tex[b];
			b++;
		}
		S++;
	}	
	return (unsigned char *)S;
}
void StsEcu(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
}

