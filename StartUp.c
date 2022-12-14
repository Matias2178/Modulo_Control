  	#include	"VarGlob.h"
  	#include	"Constantes.h"
  	#include 	"sw1master.h"
  	#include 	"sw2master.h"
  	#include 	"ExeTask.h"


/******************************************************************************
*	Funcion:		SenStart000()	
*	Descricpion:	Lee los parametros de configuracion de los sensores si
*					hay un error en la comunicacion pasa al siguente sensor
*					si las mediciones son distintas carga los nuevos valores
*					si son iguales deja pasa a leer el siguiente
*					A estas tareas las realiza en los dos buses al mismo tiempo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/		

//void SenStart000(void)
//{
////	unsigned char Id;
//	unsigned char Medicion;
//	unsigned char SenId;
//	unsigned char TMB1;
//	unsigned char TDB1;
//	unsigned char SIB1;
//	unsigned char TMB2;
//	unsigned char TDB2;
//	unsigned char SIB2;
//	unsigned char SInd;
//Proceso.B.fConfPer = true;
//
//	for(SenId=0;SenId<64;SenId++)
//	{
//		SInd = SenId & 0x1F;
////Cargo los tiempos seg?n la configuracion del sensor
//		if(SenId<0x20)
//		{
//		//Sensores de Semilla
//			if( BUS1.Sie[SInd].Sts.B.Mod)
//			{
//			//Bus 1 Gruesa
//				TMB1 = k_GruesaTM;
//				TDB1 = k_GruesaTD;
//				SIB1 = k_GruesaCS;
//			}
//			else
//			{
//			//Bus 1 Fina
//				TMB1 = k_FinaTM;
//				TDB1 = k_FinaTD;
//				SIB1 = k_FinaCS;
//			}
//			if( BUS2.Sie[SInd].Sts.B.Mod)
//			{
//			//Bus 2 Gruesa
//				TMB2 = k_GruesaTM;
//				TDB2 = k_GruesaTD;
//				SIB2 = k_GruesaCS;
//			}
//			else
//			{
//			// Bus 2 Fina
//				TMB2 = k_FinaTM;
//				TDB2 = k_FinaTD;
//				SIB2 = k_FinaCS;
//			}
//		}
//		else
//		{
//		// Sensores de Fertilizante
//			if( BUS1.Fer[SInd].Sts.B.Mod)
//			{
//			//Bus 1 Gruesa
//				TMB1 = k_GruesaTM;
//				TDB1 = k_GruesaTD;
//				SIB1 = k_GruesaCS;
//			}
//			else
//			{
//			// Bus 1 Fina
//				TMB1 = k_FinaTM;
//				TDB1 = k_FinaTD;
//				SIB1 = k_FinaCS;
//			}
//			
//			if( BUS2.Fer[SInd].Sts.B.Mod)
//			{
//			// Bus 2 Gruesa
//				TMB2 = k_GruesaTM;
//				TDB2 = k_GruesaTD;
//				SIB2 = k_GruesaCS;
//			}
//			else
//			{
//			// Bus 2 Fina
//				TMB2 = k_FinaTM;
//				TDB2 = k_FinaTD;
//				SIB2 = k_FinaCS;
//			}
//		}
//		
////-------LECTURA DEL REGISTRO 1 TIEMPO DE MEDICION-----------------------------
//		SW1_PortUserStart(SenId,0x01 | SW1_cmdRd,1);
//		SW2_PortUserStart(SenId,0x01 | SW2_cmdRd,1);
//		
//		SW1PortUser.Sts.B.fPend=true;
//		SW2PortUser.Sts.B.fPend=true;
//
//		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//		
//		if(!SW1PortUser.Sts.B.fOk && !SW2PortUser.Sts.B.fOk) continue;
//	
//		if (SW1PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW1PortUser.Data;
//			if(SenId<0x20)
//			{
//				BUS1.Sie[SInd].Sts.B.Con = true;
//				BUS1.Sie[SInd].Sts.B.Det = true;
//			}
//			else
//			{
//				BUS1.Fer[SInd].Sts.B.Con = true;
//				BUS1.Fer[SInd].Sts.B.Det = true;
//			}	
//			if(Medicion != TMB1)
//			{
//				SW1_PortUserStart(SenId,0x01 | SW1_cmdWr,1);
//				SW1_PortUserWr(TMB1);
//				SW1PortUser.Sts.B.fPend=true;
//			}
//		}
//		else
//		{
//			if(SenId<0x20)
//			{
//				BUS1.Sie[SInd].Sts.B.Con = false;
//				BUS1.Sie[SInd].Sts.B.Det = false;
//			}
//			else
//			{
//				BUS1.Fer[SInd].Sts.B.Con = false;
//				BUS1.Fer[SInd].Sts.B.Det = false;
//			}
//		}
//		
//		if (SW2PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW2PortUser.Data;
//			if(SenId<0x20)
//			{
//				BUS2.Sie[SInd].Sts.B.Con = true;
//				BUS2.Sie[SInd].Sts.B.Det = true;
//			}
//			else
//			{
//				BUS2.Fer[SInd].Sts.B.Con = true;
//				BUS2.Fer[SInd].Sts.B.Det = true;
//			}
//			if(Medicion != TMB2)
//			{
//				SW2_PortUserStart(SenId,0x01 | SW2_cmdWr,1);
//				SW2_PortUserWr(TMB2);
//				SW2PortUser.Sts.B.fPend=true;
//			}
//		}
//		else
//		{
//			if(SenId<0x20)
//			{
//				BUS2.Sie[SInd].Sts.B.Con = false;
//				BUS2.Sie[SInd].Sts.B.Det = false;
//			}
//			else
//			{
//				BUS2.Fer[SInd].Sts.B.Con = false;
//				BUS2.Fer[SInd].Sts.B.Det = false;
//			}
//		}			
//		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
//
////-------LECTURA DEL REGISTRO 2 TIEMPO DE DETECCION----------------------------	
//		SW1_PortUserStart(SenId,0x02 | SW1_cmdRd,1);
//		SW2_PortUserStart(SenId,0x02 | SW2_cmdRd,1);
//		SW1PortUser.Sts.B.fPend=true;
//		SW2PortUser.Sts.B.fPend=true;
//		
//		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//	
//		if (SW1PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW1PortUser.Data;
//			if(Medicion != TDB1)
//			{
//				SW1_PortUserStart(SenId,0x02 | SW1_cmdWr,1);
//				SW1_PortUserWr(TDB1);
//				SW1PortUser.Sts.B.fPend=true;
//			}
//		}
//		if (SW2PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW2PortUser.Data;
//			BUS2.Sie[SInd].Sts.B.Con = true;
//			BUS2.Sie[SInd].Sts.B.Det = true;
//			if(Medicion != TDB2)
//			{
//				SW2_PortUserStart(SenId,0x02 | SW2_cmdWr,1);
//				SW2_PortUserWr(TDB2);
//				SW2PortUser.Sts.B.fPend=true;
//			}
//		}
//		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();		
//
////---LECTURA DEL REGISTRO 3 CANTIDAD DE SEMILLAS PARA EL INICIO DE MEDICION----	
//		SW1_PortUserStart(SenId,0x03 | SW1_cmdRd,1);
//		SW2_PortUserStart(SenId,0x03 | SW2_cmdRd,1);
//		SW1PortUser.Sts.B.fPend=true;
//		SW2PortUser.Sts.B.fPend=true;
//		
//		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//	
//		if (SW1PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW1PortUser.Data;
//			if(Medicion != SIB1)
//			{
//				SW1_PortUserStart(SenId,0x03 | SW1_cmdWr,1);
//				SW1_PortUserWr(SIB1);
//				SW1PortUser.Sts.B.fPend=true;
//			}
//		}
//		if (SW2PortUser.Sts.B.fOk)
//		{
//			Medicion = *(unsigned char *)SW2PortUser.Data;
//			if(Medicion != SIB2)
//			{
//				SW2_PortUserStart(SenId,0x03 | SW2_cmdWr,1);
//				SW2_PortUserWr(SIB2);
//				SW2PortUser.Sts.B.fPend=true;
//			}
//		}		
////		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
//		
//		if(SenId <= 0x1F)
//		{
//			DtsComBus1.Sie.Sts[SInd] = BUS1.Sie[SInd].Sts.C.Val;
//			DtsComBus2.Sie.Sts[SInd] = BUS2.Sie[SInd].Sts.C.Val;
//		}
//		else
//		{
//			DtsComBus1.Fer.Sts[SInd] = BUS1.Fer[SInd].Sts.C.Val;
//			DtsComBus2.Fer.Sts[SInd] = BUS2.Fer[SInd].Sts.C.Val;
//		}
//	}
//
//Proceso.B.fConfPer = false;
//	GuardaConfSen();
//}


/******************************************************************************
*	Funcion:		SenRedetecion()	
*	Descricpion:	Lee los parametros de configuracion de los sensores si
*					hay un error en la comunicacion pasa al siguente sensor
*					si las mediciones son distintas carga los nuevos valores
*					si son iguales deja pasa a leer el siguiente
*					A estas tareas las realiza en los dos buses al mismo tiempo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/		

void SenRedetecion(void)
{
	unsigned char Id;
	unsigned char SenId;
	unsigned char Num;
	unsigned char Error1;
	unsigned char Error2;
	unsigned char Falla;
	unsigned char SInd;

	if (SW1PortSys.Sts.B.fPend || SW2PortSys.Sts.B.fPend) ExeTask();
	Proceso.B.fConfPer = true;
	
	memset(&SenDtsCon,0x00,sizeof(struct _SenDts));
	memset(&ConPer,0x00,sizeof(struct _DtsPer));
	memset(&BusPer,0x00,sizeof(struct _DtsPer));

	for(SenId=0;SenId<64;SenId++)
	{
		SInd = SenId & 0x1F;
		Error1 = 0;
		Error2 = 0;
		Falla = 0;
//---------------------------------------------------------------------------
//	Redeteccion de los sensores de semillas
		
		SW1_PortUserStart(SenId,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
lRelectura:
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
		
		if (SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
			if(SenId<0x20)
			{
				BUS1.Sie[SInd].Sts.B.Con = true;
				BUS1.Sie[SInd].Sts.B.Det = true;
				BUS1.Sie[SInd].Sts.B.FDs = false;
			}
			else
			{
				BUS1.Fer[SInd].Sts.B.Con = true;
				BUS1.Fer[SInd].Sts.B.Det = true;
				BUS1.Fer[SInd].Sts.B.FDs = false;
			}	
		}
		else if( SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1 < 3)
			{
				SW1_PortUserStart(SenId,0x07 | SW1_cmdRd,1);
				SW1PortUser.Sts.B.fPend=true;
				Falla = 1;
				
			}	
			if(SenId<0x20)
			{
				BUS1.Sie[SInd].Sts.B.Con = false;
				BUS1.Sie[SInd].Sts.B.Det = false;
				BUS1.Sie[SInd].Sts.B.FDs = false;
			}
			else
			{
				BUS1.Fer[SInd].Sts.B.Con = false;
				BUS1.Fer[SInd].Sts.B.Det = false;
				BUS1.Fer[SInd].Sts.B.FDs = false;
			}
		}
///**********************************************************		
		if (SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
			if(SenId<0x20)
			{
				BUS2.Sie[SInd].Sts.B.Con = true;
				BUS2.Sie[SInd].Sts.B.Det = true;
				BUS2.Sie[SInd].Sts.B.FDs = false;
			}
			else
			{
				BUS2.Fer[SInd].Sts.B.Con = true;
				BUS2.Fer[SInd].Sts.B.Det = true;
				BUS2.Fer[SInd].Sts.B.FDs = false;
			}
		}
		else if( SW2PortUser.Sts.B.fErr)
		{	
			Error2 ++;
			if(Error2 < 3)
			{
				SW2_PortUserStart(SenId,0x07 | SW2_cmdRd,1);
				SW2PortUser.Sts.B.fPend=true;
				Falla = 1;
			}
				
			if(SenId<0x20)
			{
				BUS2.Sie[SInd].Sts.B.Con = false;
				BUS2.Sie[SInd].Sts.B.Det = false;
				BUS2.Sie[SInd].Sts.B.FDs = false;
			}
			else
			{
				BUS2.Fer[SInd].Sts.B.Con = false;
				BUS2.Fer[SInd].Sts.B.Det = false;
				BUS2.Fer[SInd].Sts.B.FDs = false;
			}
		}
		if(Falla)
		{
			Falla = 0;
			goto lRelectura;
		}			
	}

	GuardaConfSen();
//-----------------------------------------------------------------------------
//	Redeteccion Moduladoras
	for(SenId=0;SenId<16;SenId++)
	{
		Error1 = 0;
		Error2 = 0;
lErrorLectMod:	
		Id = ModDirId(SenId);
		SInd =SenId;
		SW1_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(Id,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Moduladora[SInd].Sts.B.Con = true;
			Moduladora[SInd].Sts.B.Bus = false;
			Moduladora[SInd].Sts.B.Det = true;
			Moduladora[SInd].Sts.B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Moduladora[SInd].Sts.B.Con = true;
			Moduladora[SInd].Sts.B.Bus = true;
			Moduladora[SInd].Sts.B.Det = true;
			Moduladora[SInd].Sts.B.FDs = false;
		}
		else
		{
			Error1 ++;
			if(Error1 < 3)
				goto lErrorLectMod;
				
			Moduladora[SInd].Sts.B.Con = false;
			Moduladora[SInd].Sts.B.Bus = false;
			Moduladora[SInd].Sts.B.Det = false;
			Moduladora[SInd].Sts.B.FDs = false;
		}
	}
	GrabaConfMod();
//-----------------------------------------------------------------------------
//	Redeteccion Sensores Rotacion
	for(SenId=0;SenId<8;SenId++)
	{
		Error1 = 0;
lErrorLectRot:
		Id = 0x40 + SenId;
		SInd = SenId;
		
		SW1_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(Id,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Rotacion[SInd].Sts.B.Con = true;
			Rotacion[SInd].Sts.B.Bus = false;
			Rotacion[SInd].Sts.B.Det = true;
			Rotacion[SInd].Sts.B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Rotacion[SInd].Sts.B.Con = true;
			Rotacion[SInd].Sts.B.Bus = true;
			Rotacion[SInd].Sts.B.Det = true;
			Rotacion[SInd].Sts.B.FDs = false;
		}
		else
		{
			Error1 ++;
			if(Error1 < 3)
				goto lErrorLectRot;
				
			Rotacion[SInd].Sts.B.Con = false;
			Rotacion[SInd].Sts.B.Bus = false;
			Rotacion[SInd].Sts.B.Det = false;
			Rotacion[SInd].Sts.B.FDs = false;
		}
	}
	GrabaConfROT();
//-----------------------------------------------------------------------------
//	Redeteccion sensores de RPM secumdarios	
	for(SenId=0;SenId<3;SenId++)
	{
		Error1 = 0;
lErrorLectTRB:
		Id = 0xD3 + SenId;
		SInd = SenId;
		SW1_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend = true;
		SW2PortUser.Sts.B.fPend = true;
		
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Turbina[SInd].Sts.B.Con = true;
			Turbina[SInd].Sts.B.Bus = false;
			Turbina[SInd].Sts.B.Det = true;
			Turbina[SInd].Sts.B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Turbina[SInd].Sts.B.Con = true;
			Turbina[SInd].Sts.B.Bus = true;
			Turbina[SInd].Sts.B.Det = true;
			Turbina[SInd].Sts.B.FDs = false;
		}
		else
		{
			Error1 ++;
			if(Error1 < 3)
				goto lErrorLectTRB;
			
			Turbina[SInd].Sts.B.Con = false;
			Turbina[SInd].Sts.B.Bus = false;
			Turbina[SInd].Sts.B.Det = false;
			Turbina[SInd].Sts.B.FDs = false;
		}
	}
	GrabaConfTRB();
//-----------------------------------------------------------------------------
//	Redeteccion Sensores de Tolva
	for(SenId=0;SenId<16;SenId++)
	{
		Error1 = 0;

lErrorLectTolva:
		Id = 0x48 + SenId;
		SInd = SenId;
		SW1_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(Id,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Tolva[SInd].Sts.B.Con = true;
			Tolva[SInd].Sts.B.Bus = false;
			Tolva[SInd].Sts.B.Det = true;
			Tolva[SInd].Sts.B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Tolva[SInd].Sts.B.Con = true;
			Tolva[SInd].Sts.B.Bus = true;
			Tolva[SInd].Sts.B.Det = true;
			Tolva[SInd].Sts.B.FDs = false;
		}
		else
		{
			Error1 ++;
			if(Error1 < 3)
				goto lErrorLectTolva;
				
			Tolva[SInd].Sts.B.Con = false;
			Tolva[SInd].Sts.B.Bus = false;
			Tolva[SInd].Sts.B.Det = false;
			Tolva[SInd].Sts.B.FDs = false;
		}
	}
	GrabaConfTol();
//-----------------------------------------------------------------------------
//	Redeteccion Sensores de Presion
	for(SenId=0;SenId<9;SenId++)
	{
		Error1 = 0;

lErrorLectPresion:
		Id = 0x80 + SenId;
		SInd = SenId;
		SW1_PortUserStart(Id,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(Id,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Presion[SInd].Sts.B.Con = true;
			Presion[SInd].Sts.B.Bus = false;
			Presion[SInd].Sts.B.Det = true;
			Presion[SInd].Sts.B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Presion[SInd].Sts.B.Con = true;
			Presion[SInd].Sts.B.Bus = true;
			Presion[SInd].Sts.B.Det = true;
			Presion[SInd].Sts.B.FDs = false;
		}
		else
		{
			Error1 ++;
			if(Error1 < 3)
				goto lErrorLectPresion;
				
			Presion[SInd].Sts.B.Con = false;
			Presion[SInd].Sts.B.Bus = false;
			Presion[SInd].Sts.B.Det = false;
			Presion[SInd].Sts.B.FDs = false;
		}
	}
	GrabaConfPer();
	Proceso.B.fConfPer = false;
}

/******************************************************************************
*	Funcion:		SenTuboSucio00()	
*	Descricpion:	Lee el estado de tubo sucio de los sensores en los dos 
*					Buses al mismo tiempo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/		

void SenTuboSucio(void)
{
//	unsigned char Id;
	unsigned char Medicion;
	unsigned char SenId;
	unsigned char SInd;

	Proceso.B.fConfPer = true;
	for(SenId=0;SenId<64;SenId++)
	{
//		Id = SenId;
		SInd = SenId & 0x1F;
		SW1_PortUserStart(SenId,0x04 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x04 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;

		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
		if (SW1PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(SenId<0x20)
			{
				BUS1.Fer[SInd].Sts.B.AvTs = false;
				if(!Medicion)
					BUS1.Sie[SInd].Sts.B.AvTs = true;
			}
			else
			{
				BUS1.Fer[SInd].Sts.B.AvTs = false;
				if(!Medicion)
					BUS1.Fer[SInd].Sts.B.AvTs = true;
			}	
		}
		if (SW2PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(SenId<0x20)
			{
				BUS2.Fer[SInd].Sts.B.AvTs = false;
				if(!Medicion)
					BUS2.Sie[SInd].Sts.B.AvTs = true;
			}
			else
			{
				BUS2.Fer[SInd].Sts.B.AvTs = false;
				if(!Medicion)
					BUS2.Fer[SInd].Sts.B.AvTs = true;
			}
		}
	}
	Proceso.B.fConfPer = false;
}

/******************************************************************************
*	Funcion:		SenConfig00()	
*	Descricpion:	Lee los parametros de configuracion de los sensores si
*					hay un error en la comunicacion pasa al siguente sensor
*					si las mediciones son distintas carga los nuevos valores
*					si son iguales deja pasa a leer el siguiente
*					A estas tareas las realiza en los dos buses al mismo tiempo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/		

void SenConfig00(void)
{
	unsigned char Medicion;
	unsigned char SenId;
	unsigned char TMB1;
	unsigned char TDB1;
	unsigned char SIB1;
	unsigned char TMB2;
	unsigned char TDB2;
	unsigned char SIB2;
	unsigned char SIndice;
	unsigned char Error1;
	unsigned char Error2;
	unsigned char Falla;
	unsigned char SenB1;
	unsigned char SenB2; 

	
Proceso.B.fConfPer = true;
Error1 = Error2 = Falla = SenB1 = SenB2 = 0;
	for(SenId=0;SenId<64;SenId++)
	{
		SIndice = SenId & 0x1F;
//Cargo los tiempos seg?n la configuracion del sensor
		if(SenId<0x20)
		{
			if(!BUS1.Sie[SIndice].Sts.B.Det && !BUS2.Sie[SIndice].Sts.B.Det)
				continue;
		//Sensores de Semilla
			if( BUS1.Sie[SIndice].Sts.B.Mod)
			{
			//Bus 1 Gruesa
				TMB1 = k_GruesaTM;
				TDB1 = k_GruesaTD;
				SIB1 = k_GruesaCS;
			}
			else
			{
			//Bus 1 Fina
				TMB1 = k_FinaTM;
				TDB1 = k_FinaTD;
				SIB1 = k_FinaCS;
			}
			if( BUS2.Sie[SIndice].Sts.B.Mod)
			{
			//Bus 2 Gruesa
				TMB2 = k_GruesaTM;
				TDB2 = k_GruesaTD;
				SIB2 = k_GruesaCS;
			}
			else
			{
			// Bus 2 Fina
				TMB2 = k_FinaTM;
				TDB2 = k_FinaTD;
				SIB2 = k_FinaCS;
			}
		}
		else
		{
			if(!BUS1.Fer[SIndice].Sts.B.Det && !BUS2.Fer[SIndice].Sts.B.Det)
				continue;
		// Sensores de Fertilizante
			if( BUS1.Fer[SIndice].Sts.B.Mod)
			{
			//Bus 1 Gruesa
				TMB1 = k_GruesaTM;
				TDB1 = k_GruesaTD;
				SIB1 = k_GruesaCS;
			}
			else
			{
			// Bus 1 Fina
				TMB1 = k_FinaTM;
				TDB1 = k_FinaTD;
				SIB1 = k_FinaCS;
			}
			
			if( BUS2.Fer[SIndice].Sts.B.Mod)
			{
			// Bus 2 Gruesa
				TMB2 = k_GruesaTM;
				TDB2 = k_GruesaTD;
				SIB2 = k_GruesaCS;
			}
			else
			{
			// Bus 2 Fina
				TMB2 = k_FinaTM;
				TDB2 = k_FinaTD;
				SIB2 = k_FinaCS;
			}
		}
		
//-----------------------------------------------------------------------------
//		LECTURA DEL REGISTRO 1 TIEMPO DE MEDICION
		if(SenId<0x20)
		{
		//Semilla 
			if(BUS1.Sie[SIndice].Sts.B.Det)
			{
				SenB1 = true;
				SW1_PortUserStart(SenId,0x01 | SW1_cmdRd,1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;
				SenB1 = false;
			}
			if(BUS2.Sie[SIndice].Sts.B.Det)
			{
				SenB2 = true;
				SW2_PortUserStart(SenId,0x01 | SW2_cmdRd,1);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;
				SenB2 = false;
			}
		}
		else
		{
		//Fertilizante
			if(BUS1.Fer[SIndice].Sts.B.Det)
			{
				SenB1 = true;
				SW1_PortUserStart(SenId,0x01 | SW1_cmdRd,1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;
				SenB1 = false;
			}
			if(BUS2.Fer[SIndice].Sts.B.Det)
			{
				SenB2 = true;
				SW2_PortUserStart(SenId,0x01 | SW2_cmdRd,1);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;
				SenB2 = false;
			}
		}	
//-----------------------------------------------------------------------------
//		LECTURA REGISTRO 1 TIEMPO DE MEDICION	
		Error1 = false;
		Error2 = false;
lLecTmpMed:		
		Falla = false;
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus 1		
		if (SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW1PortUser.Data;	
			if(Medicion != TMB1)
			{
				SW1_PortUserStart(SenId,0x01 | SW1_cmdWr,1);
				SW1_PortUserWr(TMB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		else if (SW1PortUser.Sts.B.fErr)
		{			
			Error1 ++;
			if(Error1 < 3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x01 | SW1_cmdRd,1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				Error1 = 0;
				SW1PortUser.Sts.Value = 0;	
			}	
		}
//-------------------------------------
//	Control Bus2		
		if (SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(Medicion != TMB2)
			{
				SW2_PortUserStart(SenId,0x01 | SW2_cmdWr,1);
				SW2_PortUserWr(TMB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}
		else if (SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2 < 3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x01 | SW2_cmdRd,1);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				Error2 = 0;
				SW2PortUser.Sts.Value = 0;	
			}
		}
		if(Falla)
		{
			Falla = false;
			goto lLecTmpMed;
		}		
//-----------------------------------------------------------------------------
//		Control de la Escritura Tiempo de Medicion
		Error1 = 0;
		Error2 = 0;
lEscTmpMed:
		Falla = false;
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus1
		if( SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;	
		}
		else if( SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1 < 3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x01 | SW1_cmdWr,1);
				SW1_PortUserWr(TMB1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				Error1 = 0;
				SW1PortUser.Sts.Value = 0;	
			}	
		}
//-------------------------------------
//	Control Bus2
		if ( SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;	
		}
		else if( SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2 < 3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x01 | SW2_cmdWr,1);
				SW2_PortUserWr(TMB2);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{	
				Error2 = 0;
				SW2PortUser.Sts.Value = 0;	
			}	
		}
		if (Falla)
		{
			Falla = false;
			goto lEscTmpMed;
		}
//-----------------------------------------------------------------------------	
//		LECTURA DEL REGISTRO 2 TIEMPO DE DETECCION

		if(SenB1)
		{
			SW1_PortUserStart(SenId,0x02 | SW1_cmdRd,1);
			SW1PortUser.Sts.B.fPend=true;
		}	
		else
		{
			SW1PortUser.Sts.Value = 0;
		}
		if(SenB2)
		{
			SW2_PortUserStart(SenId,0x02 | SW2_cmdRd,1);
			SW2PortUser.Sts.B.fPend=true;
		}
		else
		{
			SW2PortUser.Sts.Value = 0;
		}
		Error1 = 0;
		Error2 = 0;	
lLecTmpDet:
		Falla = 0;
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus1
		if (SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(Medicion != TDB1)
			{
				SW1_PortUserStart(SenId,0x02 | SW1_cmdWr,1);
				SW1_PortUserWr(TDB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		else if(SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1<3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x02 | SW1_cmdRd,1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;	
				Error1 = 0;
			}
		}

//-------------------------------------
//	Control Bus2
		if (SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(Medicion != TDB2)
			{
				SW2_PortUserStart(SenId,0x02 | SW2_cmdWr,1);
				SW2_PortUserWr(TDB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}
		else if(SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2<3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x02 | SW2_cmdRd,1);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;	
				Error2 = 0;
			}
		}
		if(Falla)
		{
			Falla = false;
			goto lLecTmpDet;
		}
//-----------------------------------------------------------------------------
//		Control de la escritura de los sensores Tiempo de Deteccion
		Error1 = 0;
		Error2 = 0;
lEscTmpDet:
		Falla = false;
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus1		
		if( SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
		}
		else if( SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1 < 3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x02 | SW1_cmdWr,1);
				SW1_PortUserWr(TDB1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;	
				Error1 = 0;
			}	
		}
//-------------------------------------
//	Control Bus2
		if ( SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
		}
		else if( SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2 < 3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x02 | SW2_cmdWr,1);
				SW2_PortUserWr(TDB2);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;	
				Error2 = 0;
			}	
		}
		if (Falla)
		{
			Falla = false;
			goto lEscTmpDet;
		}
//-----------------------------------------------------------------------------
//	LECTURA DEL REGISTRO 3 CANTIDAD DE PULSOS PARA INICIO DE MEDICION
		if(SenB1)
		{
			SW1_PortUserStart(SenId,0x03 | SW1_cmdRd,1);
			SW1PortUser.Sts.B.fPend=true;
		}
		else
		{
			SW1PortUser.Sts.Value = 0;
		}
		if(SenB2)
		{	
			SW2_PortUserStart(SenId,0x03 | SW2_cmdRd,1);
			SW2PortUser.Sts.B.fPend=true;
		}
		else
		{
			SW2PortUser.Sts.Value = 0;
		}
		Error1 = 0;
		Error2 = 0;
lLecPulIni:
		Falla = 0;
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus1	
		if (SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(Medicion != SIB1)
			{
				SW1_PortUserStart(SenId,0x03 | SW1_cmdWr,1);
				SW1_PortUserWr(SIB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		else if ( SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1 < 3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x03 | SW1_cmdWr,1);
				SW1_PortUserWr(SIB1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;	
				Error1 = 0;
			}
		}
//-------------------------------------
//	Control Bus2
		if (SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(Medicion != SIB2)
			{
				SW2_PortUserStart(SenId,0x03 | SW2_cmdWr,1);
				SW2_PortUserWr(SIB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}
		else if ( SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2 < 3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x03 | SW2_cmdWr,1);
				SW2_PortUserWr(SIB2);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;	
				Error2 = 0;
			}
		}	
		if (Falla)
		{
			Falla = false;
			goto lLecPulIni;
		}		
//-----------------------------------------------------------------------------
//	Control de la escritura de los sensores Cantidad de Pulsos para Activacion
		Error1 = 0;
		Error2 = 0;
lEscPulIni:
		Falla = false;
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
//-------------------------------------
//	Control Bus1
		if( SW1PortUser.Sts.B.fOk)
		{
			SW1PortUser.Sts.Value = 0;
			Error1 = 0;
		}
		else if( SW1PortUser.Sts.B.fErr)
		{
			Error1 ++;
			if(Error1 < 3)
			{
				Falla = true;
				SW1_PortUserStart(SenId,0x03 | SW1_cmdWr,1);
				SW1_PortUserWr(SIB1);
				SW1PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW1PortUser.Sts.Value = 0;	
				Error1 = 0;
			}	
		}
//-------------------------------------
//	Control Bus2
		if ( SW2PortUser.Sts.B.fOk)
		{
			SW2PortUser.Sts.Value = 0;
			Error2 = 0;
		}
		else if( SW2PortUser.Sts.B.fErr)
		{
			Error2 ++;
			if(Error2 < 3)
			{
				Falla = true;
				SW2_PortUserStart(SenId,0x03 | SW2_cmdWr,1);
				SW2_PortUserWr(SIB2);
				SW2PortUser.Sts.B.fPend=true;
			}
			else
			{
				SW2PortUser.Sts.Value = 0;	
				Error2 = 0;
			}	
		}
		if (Falla)
		{
			Falla = false;
			goto lEscPulIni;
		}
	}
	Proceso.B.fConfPer = false;
}


