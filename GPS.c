/*****************************************************************************
 *	 FUNCIONES DE LECTURA DEL GPS
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - 2013
 *	
 *****************************************************************************/
	
	#include <string.h>
	#include <stdlib.h>
	#include <time.h>
	#include "GPS.h"
	#include "Constantes.h"
	#include "VarGlob.h"
	#include "UART.h"

struct _SatInfo SatInfo[32];
struct _SatInfo2 SatInfo2[32];
unsigned char GPSmsg[128];
unsigned int GPSInd;

union _GPSsts GPSsts;
struct _GPSdts	GPSdts;
struct tm Tiempo;

//union _GPSsts 	GPSs;
//struct _GPSdts	GPSd;
//unsigned char GPSm[128];
//unsigned int GPSI;

extern unsigned char RMC[128];
extern unsigned int PRMC;
extern unsigned char GGA[128];
extern unsigned int PGGA;
/******************************************************************************
*	Funcion:		artoflt()
*	Descricpion:	convierte los datos desde array a numero flotante
*	Ingreso Datos:	Direccion del array a convertir
*	Salida Datos:	Número Flotante
******************************************************************************/						
float artoflt(const char *s)
{
	float Val;
	int   coma=0;
	unsigned long int 	i =1;
	int	  Ent=0;
	int	 frac=0;
		
	Ent = *s - 48;
	s++;
	coma = 1;
	while(*s != 0)
	{
		if(*s=='.') 
		{
			coma=0;
			s++;
			continue;
		}
		
		if(coma)
		{
			Ent = (Ent*10) + (*s - 48);
		}
		else
		{
			frac = (frac*10) + (*s - 48);
			i = i*10;
		}
		s++;
	}
		Val = (float)(Ent + (float)frac/i);
		return (float)Val;
}


/******************************************************************************
*	Funcion:		MsgCheck()
*	Descricpion:	Compara dos array de 5 lugares y retorna verdadero si son 
*					iguales o falso en el caso de que no lo sean
*	Ingreso Datos:	Direccion del primer array
*					Direccion del segundo array
*	Salida Datos:	Verdadero si los mensajes son iguales o Falso si son distintos
******************************************************************************/					
int MsgCheck(unsigned char *Msg, char *Enc)
{
	int cont;
	int i;
	
	cont = 0;
	for(i=0;i<5;i++)
	{
		if(*Msg == *Enc)
		{
			Msg++;
			Enc++;
		}
		else
			return 0;
	}
	return 1;
}


/******************************************************************************
*	Funcion:		GPSCheckSum()
*	Descricpion:	Realiza el control del Checksum de un array segun las 
*					especificaciones del modulo GPS
*	Ingreso Datos:	Direccion del array a controlar
*					
*	Salida Datos:	Verdadero si el checksum es igual o Falso si son distintos
******************************************************************************/	
unsigned char GPSCheckSum(unsigned char *S)
{
	unsigned char Control;
	unsigned char Chk;
//	unsigned int Sat;
	S++;
	
	while(*S!='*')
	{
		if((*S!='$') && (*S!='*'))
		{
			Control = *S ^ Control;
		}	
		S++;
	}
	S++;	
	if(*S>='0'&&*S<='9')
	{
		Chk = *S - '0';
	}
	else if(*S='A'&&*S<='F')
	{
		Chk = *S - 'A' + 10;
	}
	S++;	
	if(*S>='0'&&*S<='9')
	{
		Chk =(Chk*0x10)+( *S - '0');
	}
	else if(*S>='A'&&*S<='F')
	{
		Chk = (Chk*0x10)+(*S - 'A' + 10);
	}
	if(Control == Chk)
		return true;
	return false;
}



/******************************************************************************
*	Funcion:		GPSInfo()
*	Descricpion:	Pasa los parametros desde el buffer de datos de recepcion 
*					despues que recibio una linea de datos y lo carga en las
*					variables que corresponden
*
*	Ingreso Datos:	Direccion del array 
*	Salida Datos:	Ninguna
******************************************************************************/		
void GPSInfo(unsigned char *S)
{
	unsigned char Cmd[20];
	int cont;
	int Sat;
	int Aux;
	int i;
	int SatAux;
	int Msg,AcMsg;
	float Velocidad;
	
	GPSdts.sys.tGPSon = 0;
//	GPSsts.B.fBaud = true;

	S++;
//	memset(&Cmd,0x00,15);
	S += (Movstr(&Cmd,S) + 1);
	
	if(MsgCheck(Cmd, "GPRMC"))
	{

//		GPSsts.B.fBaud = true;
		Proceso.B.fGPSDtOk = true; // Se actualizaron los datos del GPS
		
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		
		//Estado GPS A = Datos Validos	
		GPSdts.sys.act = *S;
		S +=2;
		if(GPSdts.sys.act =='V')
		{
			GPSdts.fecha.a 	= 0;
			GPSdts.fecha.m 	= 0;
			GPSdts.fecha.d 	= 0;
			
			GPSdts.sys.sat 	= 0;
			GPSdts.sys.gan 	= 0;
			GPSdts.sys.pres = 0;
			
			GPSdts.pos.lat 	= 0;
			GPSdts.pos.NS 	= 0;
			GPSdts.pos.lon 	= 0;
			GPSdts.pos.EO 	= 0;
			GPSdts.pos.vel 	= 0;
			GPSdts.pos.dist = 0;
			GPSdts.pos.rmb 	= 0;
			return;
		}	
//		if(GPSdts.sys.t5hrz > 800)
//		{
//			GPSsts.B.fLec5hz = true;
//			GPSdts.sys.t5hrz = 0;	
//		}
//		GPSdts.sys.tvida = GPSdts.sys.tvid;
		GPSdts.sys.tvid = 0;
		//Fecha del GPS
		GPSdts.hora.h	= ((Cmd[0]-48)*10) + (Cmd[1]-48);
		GPSdts.hora.m 	= ((Cmd[2]-48)*10) + (Cmd[3]-48);
		GPSdts.hora.s 	= ((Cmd[4]-48)*10) + (Cmd[5]-48);
	
		//Latitud
		memset(&GPSdts.pos.latc,0x00,20);
		Movstr(&GPSdts.pos.latc,S);
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);

		GPSdts.pos.lat 	= artoflt((const char*)Cmd);
//		memset(&Cmd,0x00,15);
		
		//Hemisferio
		GPSdts.pos.NS = *S;
		S +=2;
		
		//Longitud
		memset(&GPSdts.pos.lonc,0x00,20);
		Movstr(&GPSdts.pos.lonc,S);
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		GPSdts.pos.lon 	= artoflt((const char*)Cmd);
		
		//no se como se llama
		GPSdts.pos.EO = *S;
		S +=2;
		
		//Velocidad
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		Velocidad = artoflt((const char*)Cmd);	
		GPSdts.pos.vel = Velocidad * 1.852;		
		
		//Rumbo
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		GPSdts.pos.rmb = atoi((const char*)Cmd);
		
		//Fecha
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		GPSdts.fecha.d 	= ((Cmd[0]-48)*10) + (Cmd[1]-48);
		GPSdts.fecha.m 	= ((Cmd[2]-48)*10) + (Cmd[3]-48);
		GPSdts.fecha.a 	= ((Cmd[4]-48)*10) + (Cmd[5]-48);
		
		
//		//Carga de datos de fecha GTM
//		Tiempo.tm_sec 	=	GPSdts.hora.s; 
//		Tiempo.tm_min 	= 	GPSdts.hora.m;
//		Tiempo.tm_hour 	=	GPSdts.hora.h;
//		Tiempo.tm_mday 	=	GPSdts.fecha.d;
//		Tiempo.tm_mon 	=	GPSdts.fecha.m;
//		Tiempo.tm_year	=	GPSdts.fecha.a - 1900;
	
		//Correccion por posicion
		if(TimeZone<0)
		{
			if(GPSdts.hora.h < (TimeZone*-1))
			{
				GPSdts.hora.h = GPSdts.hora.h + 24	+ TimeZone;
				GPSdts.fecha.d--;
			}
			else
			{
				GPSdts.hora.h	= GPSdts.hora.h	+ TimeZone;
			}
		}
		else
		{
			GPSdts.hora.h	= GPSdts.hora.h	+ TimeZone;
			if(GPSdts.hora.h >= 24)
			{
				GPSdts.hora.h	= GPSdts.hora.h	-24;
				GPSdts.fecha.d++;
			}
		}	
	}
	else if(MsgCheck(Cmd, "GPGGA"))
	{
		if(GPSdts.sys.act !='A')
			return;
		cont = 0;
		while(cont<6)
		{
			if(*S == ',')
				cont++;
			S++;
		}
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		GPSdts.sys.sat = atoi((const char*)Cmd);
		
//		memset(&Cmd,0x00,15);
		S += (Movstr(&Cmd,S) + 1);
		GPSdts.sys.pres	=	atof((const char*)Cmd);

	}
	else if(MsgCheck(Cmd,"GPGSV"))
	{
		GPSdts.sys.t5hrz = 0;
		if(GPSdts.sys.act !='A')
			return;

		S += (MovtoInt(&Cmd,S,15) + 1);
		Msg = atoi((const char*)Cmd);

		S += (MovtoInt(&Cmd,S,15) + 1);
		AcMsg = atoi((const char*)Cmd);
		
		if(AcMsg == 1)
		{
			memset(&SatInfo2,0x00,sizeof(struct _SatInfo2));
		}
		S += (MovtoInt(&Cmd,S,15));
		GPSdts.sys.satview = atoi((const char*)Cmd);
		while(*S!='*' && *S)
		{
			S++;
			S += (MovtoInt(&Cmd,S,15) + 1);
			Sat = atoi((const char*)Cmd);
			
			Sat--;
			
			S += (MovtoInt(&Cmd,S,15) + 1);
			Aux = atoi((const char*)Cmd);
			SatInfo2[Sat].Elv = Aux; 
			
			S += (MovtoInt(&Cmd,S,15) + 1);
			Aux = atoi((const char*)Cmd);
			SatInfo2[Sat].Azt = Aux;
			
			S += (MovtoInt(&Cmd,S,15));
			
			Aux = atoi((const char*)Cmd);	
			SatInfo2[Sat].Gan = Aux;
		}
				
		if(Msg == AcMsg)
		{
			GPSdts.sys.gan = 0;
			SatAux = 0;
			
			for(i=1;i<33;i++)
			{
				if(SatInfo2[i].Gan > 20)
				{
					GPSdts.sys.gan = GPSdts.sys.gan + SatInfo2[i].Gan;
					SatAux++;	
				}
			}
			GPSdts.sys.gan = GPSdts.sys.gan / SatAux;

		}
	}	
}
	
char  MovtoInt(unsigned char *Dest, unsigned char *Orig, int lDest)
{	
	char i = 0;
	int a;
	unsigned char *PAux;
	
	PAux = Dest;
	
	for(a=0;a<lDest;a++)
	{
		*PAux = 0;
		PAux++;
	}
		
	while(*Orig!=','&& *Orig!='*' && *Orig)
	{
		if(*Orig!='.')
		{
		//	*Dest = *Orig - '0';
			*Dest = *Orig;
		}
		Dest++;
		Orig++;
		i++;
	}
	return (char) i;
}
/******************************************************************************
*	Funcion:		GPSbaud()
*	Descricpion:	Configura el modulo GPS para que trasnsmita los datos a
*					 19200 baudios
*					
*
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguna
******************************************************************************/	
void GPSbaud(void)
{
	unsigned char *S;
	S = U1TxBuf; 
	memset(U1TxBuf,0,sizeof(U1TxBuf));
	U1TxInd = 0;
	strcpy((char *)S,"$PSRF100,1,19200,8,1,0*38");
	S=S + strlen("$PSRF100,1,19200,8,1,0*38");
	*S = 0x0D;
	S++;
	*S = 0x0A;
	U1TXREG = U1TxBuf[0];
	GPSdts.sys.Baud = 2; 
	
}


/******************************************************************************
*	Funcion:		GPS5Hrz()
*	Descricpion:	Configura el modulo GPS para que trasnsmita los datos a
*					5 Hrz y el puerto de comunicaciones a 19200 baudios
*					
*
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguna
******************************************************************************/	
void GPS5hrz(void)
{
	unsigned char *S;
	S = U1TxBuf; 
	memset(U1TxBuf,0,sizeof(U1TxBuf));
	U1TxInd = 0;
	strcpy((char *)S,"$PSRF103,04,06,00,01*26");
	S=S + strlen("$PSRF103,04,06,00,01*26");
	*S = 0x0D;
	S++;
	*S = 0x0A;
	U1TXREG = U1TxBuf[0];	

}			