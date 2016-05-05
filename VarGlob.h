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
  	#include "VarType.h"
  	#include "EEPROM_address.h"
//----------------------------------------------------------------------------
//	VARIABLES TEMPORALES PARA PRUEBAS BORRAR DESPUES DE UTILIZAR
	extern int	tlecgpstx;
	extern int  Checksum;
	extern int	Secuenciometro;
//----------------------------------------------------------------------------
//	VARIABLES PARA LA LECTURA DE LOS PUERTOS
	extern unsigned int RotRLect;
	extern unsigned int RLectCnt;
	extern unsigned int RotID;
	extern unsigned int ErrorRot;
//----------------------------------------------------------------------------
//	ESTRUCTURA DE LOS TEMPORIZADORES
	extern struct _Sts_Tmr  Sts_Tmr;

	extern int Adq_SelTask0010;
	extern int Adq_SelTask0011;
	extern int Adq_SelTask0020;
	extern int Adq_SelTask0021;
	extern unsigned char IDSen;
//	extern int Valor;

	extern int VSiembra;	
	extern int TimeZone;
	extern unsigned long RN171_Desc;
	extern unsigned int Mic_Desc;
	extern unsigned int KAV_cont;
	extern unsigned int IDSatelites;
	extern unsigned int MemAddr;
	extern unsigned int MemDatos;
	extern unsigned int fifoADDR;
	extern unsigned int fifoADDR1;
	extern unsigned int failcont;
	extern unsigned int BaudiosUART2;
	extern unsigned int BaudiosUART3;
	
	extern float Tension;
//	extern float VelTest; //Borrar solo para pruebas
	
	extern struct _TLin TLin1;
	extern struct _TLin TLin2;
	extern int	TiempoDatosL1;
	extern char TLed;
	extern unsigned long SerialNum;
	extern char Nombre[20];
	
//----------------------------------------------------------------------------
//	CONTADORES PARA TAREAS SECUENCIALES
	extern	char Com_DtsTask_Sen010;
	extern	char Com_DtsTask_Mod010;
	extern	char Com_DtsTask_TRB010;
	extern	char Com_DtsTask_ROT010;
	extern	char Com_DtsTask_DIAG10;
	extern	char Com_DtsTask_MEM010;	
	
	extern struct _BUS_MED BUS1;
	extern struct _BUS_MED BUS2;
	
	
	extern struct _DtsCom DtsComBus1;
	extern struct _DtsCom DtsComBus2;
	extern struct _DtsPerCom DtsPerCom;
	
	extern struct _DtsSensores DtsSie;
	extern struct _DtsSensores DtsFer;
	
	extern union _Proc Proceso;

	extern struct _SetId SetId;

	extern struct _SenDts SenDtsHab;
	extern struct _SenDts SenDtsCon;
	extern struct _SenDts SenDtsMod;	//Modo de funcionamiento del sensor
	extern struct _DtsPer HabPer;
	extern struct _DtsPer ConPer;
	extern struct _DtsPer BusPer;
	extern struct _DtsSiembra DtsSiembra;
	extern struct _SenRot	Rotacion[8];
 	extern struct _SenRot	Turbina[3];
	extern struct _Mod		Moduladora[16];
	extern struct _SenTol	Tolva[16];	
	extern struct _LIN232 LIN1_232;	
	extern struct _LIN232 LIN2_232;	
	
	extern union _UInt32 CorrAlDen;
	
	extern struct _GPS Lat;
	extern struct _GPS Lon;
	extern struct _SCom Wifi;
	extern struct _SCom RS232;
	extern struct _Sensores Sensores;
	extern struct _MGPS MGPS;
	extern struct _CGPS CGPS;
//	extern struct _Dest DestWf;
	extern struct _ModWf ModuloWf;
	extern struct _Destello Dest_WF;