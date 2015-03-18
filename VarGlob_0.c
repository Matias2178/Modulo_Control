/*****************************************************************************
 *	DECLARACION DE LAS VARIABLES GOLBALES ULITIZADAS EN EL PROGRAMA
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 
 *	
 *****************************************************************************/
 //	#include "VarType.h"
// 	#include "VarType1.h"
 	#include "VarGlob.h"
 	
//----------------------------------------------------------------------------
//	VARIABLES PARA LA LECTURA DE LOS PUERTOS


//	unsigned int SenB2ID; 	
//----------------------------------------------------------------------------
//	VARIABLES TEMPORALES PARA PRUEBAS BORRAR DESPUES DE UTILIZAR
//	char ModDir;
//	char ModNum;	
//----------------------------------------------------------------------------
//	ESTRUCTURA DE TOTALIZADORES

	struct _Sts_Tmr  Sts_Tmr;
//	int control ;
//	int Control2 ;
//	int Cnt1;
//	int Cnt2;
//	int Cont0010;
//	int Cont0100;
//	int Cont1000;
//	int contador;
//	int Valor;
	int Adq_SelTask0010;
	int Adq_SelTask0011;
	int Adq_SelTask0020;
	int Adq_SelTask0021;
	unsigned char IDSen;


//Control de dispositivos conectados Lin
	char DispLin1;
	char DispActLin1;
	char DispErrLin1;
	char DispLin2;
	char DispActLin2;
	char DispErrLin2;
	char TDispLin1;
	char TDispActLin1;
	char TDispErrLin1;
	char TDispLin2;
	char TDispActLin2;
	char TDispErrLin2;
	int	tlecgpstx;
	int TimeZone;
	unsigned int MemAddr;
	unsigned int MemDatos;
	unsigned int fifoADDR;
	unsigned int fifoADDR1;
	unsigned int failcont;
	unsigned int BaudiosUART2;
	unsigned int BaudiosUART3;
	float Tension;
	
	unsigned long Aux1;
	
	
//	float VelTest; //Borrar solo para pruebas
//	char Comunic;
	int VSiembra;
	int	Medicion;
	int Checksum;
	
	struct _TLin TLin1;
	struct _TLin TLin2;
	
	int	TiempoDatosL1;
	char TLed;
	unsigned long SerialNum;
	char Nombre[20];
//----------------------------------------------------------------------------
//	CONTADORES PARA TAREAS SECUENCIALES
//	char Com_DtsTask010;
	char Com_DtsTask_Sen010;
	char Com_DtsTask_Mod010;
	char Com_DtsTask_TRB010;
	char Com_DtsTask_ROT010;
	char Com_DtsTask_DIAG10;
	char Com_DtsTask_MEM010;

	struct _SetId SetId;

	
	struct _BUS_MED BUS1;
	struct _BUS_MED BUS2;

//----------------------------------------------------------------------------
// Datos para Communicaciones

	struct _DtsCom DtsComBus1;
	struct _DtsCom DtsComBus2;
	struct _DtsPerCom DtsPerCom;
	

	
	union _Proc Proceso;
	struct _SenDts SenDtsHab;
	struct _SenDts SenDtsCon;
	struct _SenDts SenDtsMod;
	struct _DtsPer HabPer;
	struct _DtsPer ConPer;
	struct _DtsPer BusPer;

	struct _DtsSiembra DtsSiembra;
	struct _SenRot	Rotacion[8];
	struct _SenRot	Turbina[3];
	struct _Mod		Moduladora[16];	
	union  _SenTol	SenTol[16];
	
	union _UInt32 CorrAlDen;

	struct _GPS Lat;
	struct _GPS Lon;
	struct _SCom Wifi;
	struct _SCom RS232;
	struct _Sensores Sensores;
	struct _MGPS MGPS;
