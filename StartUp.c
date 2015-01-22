


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

void SenStart000(void)
{
//	unsigned char Id;
	unsigned char Medicion;
	unsigned char SenId;
	unsigned char TMB1;
	unsigned char TDB1;
	unsigned char SIB1;
	unsigned char TMB2;
	unsigned char TDB2;
	unsigned char SIB2;
	unsigned char SInd;

	for(SenId=0;SenId<64;SenId++)
	{
		SInd = SenId & 0x1F;
		if(SenId<0x20)
		{
			if( BUS1.Sie[SInd].Sts.B.Mod)
			{
				TMB1 = k_GruesaTM;
				TDB1 = k_GruesaTD;
				SIB1 = k_GruesaCS;
			}
			else
			{
				TMB1 = k_FinaTM;
				TDB1 = k_FinaTD;
				SIB1 = k_FinaCS;
			}
			if( BUS2.Sie[SInd].Sts.B.Mod)
			{
				TMB2 = k_GruesaTM;
				TDB2 = k_GruesaTD;
				SIB2 = k_GruesaCS;
			}
			else
			{
				TMB2 = k_FinaTM;
				TDB2 = k_FinaTD;
				SIB2 = k_FinaCS;
			}
		}
		else
		{
			if( BUS1.Fer[SInd].Sts.B.Mod)
			{
				TMB1 = k_GruesaTM;
				TDB1 = k_GruesaTD;
				SIB1 = k_GruesaCS;
			}
			else
			{
				TMB1 = k_FinaTM;
				TDB1 = k_FinaTD;
				SIB1 = k_FinaCS;
			}
			
			if( BUS2.Fer[SInd].Sts.B.Mod)
			{
				TMB2 = k_GruesaTM;
				TDB2 = k_GruesaTD;
				SIB2 = k_GruesaCS;
			}
			else
			{
				TMB2 = k_FinaTM;
				TDB2 = k_FinaTD;
				SIB2 = k_FinaCS;
			}
		}
		
//-------LECTURA DEL REGISTRO 1 TIEMPO DE MEDICION-----------------------------
		SW1_PortUserStart(SenId,0x01 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x01 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;

		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
		
		if(!SW1PortUser.Sts.B.fOk && !SW2PortUser.Sts.B.fOk) continue;
	
		if (SW1PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(SenId<0x20)
			{
				BUS1.Sie[SInd].Sts.B.Con = true;
				BUS1.Sie[SInd].Sts.B.Det = true;
			}
			else
			{
				BUS1.Fer[SInd].Sts.B.Con = true;
				BUS1.Fer[SInd].Sts.B.Det = true;
			}	
			if(Medicion != TMB1)
			{
				SW1_PortUserStart(SenId,0x01 | SW1_cmdWr,1);
				SW1_PortUserWr(TMB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		else
		{
			if(SenId<0x20)
			{
				BUS1.Sie[SInd].Sts.B.Con = false;
				BUS1.Sie[SInd].Sts.B.Det = false;
			}
			else
			{
				BUS1.Fer[SInd].Sts.B.Con = false;
				BUS1.Fer[SInd].Sts.B.Det = false;
			}
		}
		
		if (SW2PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(SenId<0x20)
			{
				BUS2.Sie[SInd].Sts.B.Con = true;
				BUS2.Sie[SInd].Sts.B.Det = true;
			}
			else
			{
				BUS2.Fer[SInd].Sts.B.Con = true;
				BUS2.Fer[SInd].Sts.B.Det = true;
			}
			if(Medicion != TMB2)
			{
				SW2_PortUserStart(SenId,0x01 | SW2_cmdWr,1);
				SW2_PortUserWr(TMB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}
		else
		{
			if(SenId<0x20)
			{
				BUS2.Sie[SInd].Sts.B.Con = false;
				BUS2.Sie[SInd].Sts.B.Det = false;
			}
			else
			{
				BUS2.Fer[SInd].Sts.B.Con = false;
				BUS2.Fer[SInd].Sts.B.Det = false;
			}
		}			
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();

//-------LECTURA DEL REGISTRO 2 TIEMPO DE DETECCION----------------------------	
		SW1_PortUserStart(SenId,0x02 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x02 | SW2_cmdRd,1);
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
		if (SW1PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(Medicion != TDB1)
			{
				SW1_PortUserStart(SenId,0x02 | SW1_cmdWr,1);
				SW1_PortUserWr(TDB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		if (SW2PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW2PortUser.Data;
			BUS2.Sie[SInd].Sts.B.Con = true;
			BUS2.Sie[SInd].Sts.B.Det = true;
			if(Medicion != TDB2)
			{
				SW2_PortUserStart(SenId,0x02 | SW2_cmdWr,1);
				SW2_PortUserWr(TDB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();		

//---LECTURA DEL REGISTRO 3 CANTIDAD DE SEMILLAS PARA EL INICIO DE MEDICION----	
		SW1_PortUserStart(SenId,0x03 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x03 | SW2_cmdRd,1);
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
		if (SW1PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW1PortUser.Data;
			if(Medicion != SIB1)
			{
				SW1_PortUserStart(SenId,0x03 | SW1_cmdWr,1);
				SW1_PortUserWr(SIB1);
				SW1PortUser.Sts.B.fPend=true;
			}
		}
		if (SW2PortUser.Sts.B.fOk)
		{
			Medicion = *(unsigned char *)SW2PortUser.Data;
			if(Medicion != SIB2)
			{
				SW2_PortUserStart(SenId,0x03 | SW2_cmdWr,1);
				SW2_PortUserWr(SIB2);
				SW2PortUser.Sts.B.fPend=true;
			}
		}		
//		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
		
		if(SenId <= 0x1F)
		{
			DtsComBus1.Sie.Sts[SInd] = BUS1.Sie[SInd].Sts.Val;
			DtsComBus2.Sie.Sts[SInd] = BUS2.Sie[SInd].Sts.Val;
		}
		else
		{
			DtsComBus1.Fer.Sts[SInd] = BUS1.Fer[SInd].Sts.Val;
			DtsComBus2.Fer.Sts[SInd] = BUS2.Fer[SInd].Sts.Val;
		}
	}
	GuardaConfSen();
}


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
	unsigned char Error;
	unsigned char SInd;

	if (SW1PortSys.Sts.B.fPend || SW2PortSys.Sts.B.fPend) ExeTask();
	Proceso.B.fInicio = true;
	
	memset(&SenDtsCon,0x00,sizeof(struct _SenDts));
	memset(&ConPer,0x00,sizeof(struct _DtsPer));
	memset(&BusPer,0x00,sizeof(struct _DtsPer));

	for(SenId=0;SenId<64;SenId++)
	{
		SInd = SenId & 0x1F;
		Error = 0;
//---------------------------------------------------------------------------
//	Redeteccion de los sensores de semillas
lRelectura:
		SW1_PortUserStart(SenId,0x07 | SW1_cmdRd,1);
		SW2_PortUserStart(SenId,0x07 | SW2_cmdRd,1);
		
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;

		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
		
		if (SW1PortUser.Sts.B.fOk)
		{
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
		else
		{
			Error ++;
			if(Error < 3)
				goto lRelectura;
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
		else
		{	
			Error ++;
			if(Error < 3)
				goto lRelectura;
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
	}

	GuardaConfSen();
//-----------------------------------------------------------------------------
//	Redeteccion Moduladoras
	for(SenId=0;SenId<16;SenId++)
	{
		Error = 0;
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
			Error ++;
			if(Error < 3)
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
		Error = 0;
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
			Error ++;
			if(Error < 3)
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

		Error = 0;
lErrorLectTRB:

		Id = 0xD3 + SenId;
		SInd = SenId;
		SW1_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW1_PortUserWr(0x80);		//REG.ID
		
		SW2_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW2_PortUserWr(0x80);		//REG.ID
		
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
			Error ++;
			if(Error < 3)
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
		Error = 0;

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
			SenTol[SInd].B.Con = true;
			SenTol[SInd].B.Bus = false;
			SenTol[SInd].B.Det = true;
			SenTol[SInd].B.FDs = false;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			SenTol[SInd].B.Con = true;
			SenTol[SInd].B.Bus = true;
			SenTol[SInd].B.Det = true;
			SenTol[SInd].B.FDs = false;
		}
		else
		{
			Error ++;
			if(Error < 3)
				goto lErrorLectTolva;
				
			SenTol[SInd].B.Con = false;
			SenTol[SInd].B.Bus = false;
			SenTol[SInd].B.Det = false;
			SenTol[SInd].B.FDs = false;
		}
	}
	GrabaConfTol();
	Proceso.B.fInicio = false;
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

	Proceso.B.fInicio = true;
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
	Proceso.B.fInicio = false;
}


