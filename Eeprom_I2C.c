
		#include	"ES_Conf.h"
		#include	"Master3_I2C.h"
		#include	"Timer.h"
		#include	"Constantes.h"
		#include	"VarGlob.h"
		#include	<string.h>

/******************************************************************************
*	Funcion:		EepromWRBuf()	
*	Descricpion:	Graba en la direccion de memoria asignada a cantidad de da-
*					tos que esta apuntando el puntero
*	Ingreso Datos:	Direccion de Memoria a escribir
*					Puntero del donde se leen los datos
*					Cantidad de datos a escribir
*	Salida Datos:	Ninguno
******************************************************************************/
char EepromWRBuf(unsigned int Addr,unsigned char *pBuf,unsigned char Size)
{
    HabMen = 0;	
    if(StartWrI2C3(Addr)) 
	{
		BufferWrI2C3(pBuf,Size);
		//Envio el fin de transimicion
		I2C3CONbits.PEN = 1;
		while (I2C3CONbits.PEN);
		HabMen = 1;
		return true;
	}
	I2C3CONbits.PEN = 1;
	HabMen = 1;
	return false;	
}

/******************************************************************************
*	Funcion:		EepromRDBuf()	
*	Descricpion:	Lee de la direccion de memoria asignada a cantidad de da-
*					tos que esta apuntando el puntero y los guarda en donde 
*					apunta el puntero
*	Ingreso Datos:	Direccion de Memoria a leer
*					Puntero del donde se escribiran los datos
*					Cantidad de datos a leer
*	Salida Datos:	Ninguno
******************************************************************************/
char EepromRDBuf(unsigned int Addr,unsigned char *pBuf,unsigned char Size)
{
	if(StartRdI2C3(Addr))
	{
		BufferRdI2C3(pBuf,Size);	
		//Envio fin de transmicion
		I2C3CONbits.PEN = 1;
		while (I2C3CONbits.PEN);
		return true;
	}
	I2C3CONbits.PEN = 1;
	return false;
	
}
/******************************************************************************
*	Funcion:		MemInit()
*	Descricpion:	Graba los parametros iniciales en la memoria 
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void MemInit(void)
{
	int Control;
	union _UInt32 LocUserDW00;
	unsigned char Cmd[10];
	
	if(!EepromRDBuf(M_CHECK_FAB,(unsigned char *)&Control,sizeof(Control)))
		return;
	if(Control != k_MEM_CHECK)
	{
		memset(Cmd,0x00,10);
		ultos(SerialNum,Cmd);
		memset(Nombre,0x00,20);
		strcpy(Nombre,"ECU5000-");
		strcat(Nombre,Cmd);
		EepromWRBuf(M_NOMBRE_SEMB,Nombre,20);
		
		Dly_1_MiliSec(12);
		//Por defecto quedan habilitados todos los sensores
		memset(&SenDtsHab,0xFF,sizeof(struct _SenDts));
		LocUserDW00.BIT.B0 = EepromWRBuf(M_STS_HAB_SEN,(unsigned char *)&SenDtsHab,sizeof(struct _SenDts));
		
		Dly_1_MiliSec(12);		
		//Por defecto no tengo ningun sensor que este coenctado
		memset(&SenDtsCon,0xFF,sizeof(struct _SenDts));
		LocUserDW00.BIT.B1 = EepromWRBuf(M_STS_CON_SEN,(unsigned char *)&SenDtsCon,sizeof(struct _SenDts));
		
		Dly_1_MiliSec(12);		
		memset(&HabPer,0xFF,sizeof(struct _DtsPer));
	 	HabPer.TRB &= 0x07;
		LocUserDW00.BIT.B2 = EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
		
		Dly_1_MiliSec(12);		
		memset(&ConPer,0xFF,sizeof(struct _DtsPer));
		ConPer.TRB &= 0x07;
		LocUserDW00.BIT.B3 = EepromWRBuf(M_STS_CON_PER,(unsigned char *)&ConPer,sizeof(struct _DtsPer));
		
		Dly_1_MiliSec(12);		
		memset(&BusPer,0xFF,sizeof(struct _DtsPer));
		BusPer.TRB &= 0x07;
		LocUserDW00.BIT.B4 = EepromWRBuf(M_STS_BUS_PER,(unsigned char *)&BusPer,sizeof(struct _DtsPer));
		
		Dly_1_MiliSec(12);
		memset(&DtsSiembra,0x00,sizeof(struct _DtsSiembra));
		LocUserDW00.BIT.B6 = EepromWRBuf(M_DATOS_SEMBRADORA,(unsigned char *)&DtsSiembra,sizeof(struct _DtsSiembra));
		
		Dly_1_MiliSec(12);
		Control = k_MEM_CHECK;
		LocUserDW00.BIT.B7 = EepromWRBuf(M_CHECK_FAB,(unsigned char *)&Control,sizeof(Control));
		
		Dly_1_MiliSec(12);
		RN171_Desc = 0;
		LocUserDW00.BIT.B8 = EepromWRBuf(M_RN171_OFF,&RN171_Desc,sizeof(RN171_Desc));
		Control = k_MEM_CHECK;
	}
}

/******************************************************************************
*	Funcion:		MemLoad()
*	Descricpion:	Lee los parametros guardados en la memoria
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void MemLoad(void)
{	
	union _UInt32 LocUserDW00;
	LocUserDW00.BIT.B0 = EepromRDBuf(M_STS_HAB_SEN,(unsigned char *)&SenDtsHab,sizeof(struct _SenDts));
	
	LocUserDW00.BIT.B1 = EepromRDBuf(M_STS_CON_SEN,(unsigned char *)&SenDtsCon,sizeof(struct _SenDts));
	
	LocUserDW00.BIT.B1 = EepromRDBuf(M_STS_MOD_SEN,(unsigned char *)&SenDtsMod,sizeof(struct _SenDts));
	
	LocUserDW00.BIT.B3 = EepromRDBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
		
	LocUserDW00.BIT.B4 = EepromRDBuf(M_STS_CON_PER,(unsigned char *)&ConPer,sizeof(struct _DtsPer));

	LocUserDW00.BIT.B5 = EepromRDBuf(M_STS_BUS_PER,(unsigned char *)&BusPer,sizeof(struct _DtsPer));
	
	LocUserDW00.BIT.B6 = EepromRDBuf(M_DATOS_SEMBRADORA,(unsigned char *)&DtsSiembra,sizeof(struct _DtsSiembra));
	
	LocUserDW00.BIT.B7 = EepromRDBuf(M_AJS_TIME_ZONE,(unsigned char *)&TimeZone,sizeof(TimeZone));
	
	LocUserDW00.BIT.B8 = EepromRDBuf(M_FAC_AJS_ALTA_DENS,(unsigned char *)&CorrAlDen,sizeof(union _UInt32));
	
	LocUserDW00.BIT.B8 = EepromRDBuf(M_RN171_OFF,(unsigned char *)&CorrAlDen,sizeof(TimeZone));
	
	Nop();
	
	
}	

void GuardaPar(void)
{
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_CON_PER,(unsigned char *)&ConPer,sizeof(struct _DtsPer));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_BUS_PER,(unsigned char *)&BusPer,sizeof(struct _DtsPer));
}
		
		
		
		
			