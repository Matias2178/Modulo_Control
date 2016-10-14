/*****************************************************************************
 *	ADQUISICION DE DATOS DEL PUERTO LIN 1 
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00  
 *	FECHA		:	2013
 *****************************************************************************/
  	#include	"VarGlob.h"
	#include	"Constantes.h"
	#include 	"sw1master.h"
	#include	"Perifericos.h"
	#include	"ES_Conf.h"
	#include 	<p24Fxxxx.h>
	
_NEAR char ErrorB1;
_NEAR int SenB1ID;

/******************************************************************************
*	Funcion:		Adq_Proc_Lin1()	
*	Descricpion:	Adquiere los datos de proceso de los sensores conectados
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void Adq_Proc_Lin1(void)
{
	unsigned int Id;
	unsigned int Medicion;
	union _UInt32 LocUserDW00;
	
	if (SW1PortSys.Sts.B.fPend)	//Hay una comunicacion pendiente de ser atendida
		return;
		
	switch(Adq_SelTask0011){
		default:
			Adq_SelTask0011=0;
		
		case 0:
//Control del tiempo de vuleta del bus lin
			TLin1.Ult[TLin1.ind] = TLin1.Timer;
			TLin1.Timer = 0;
			TLin1.ind++;
			if(TLin1.ind>=10)
			{
				TLin1.ind = 0;
			}
//Control de dispositivos en el bus lin
			SenB1ID 		=	0;
			Adq_SelTask0011	=	2;
		case 2:
//Lectura de datos sensores de semilla
SemLIN1:
			for(;SenB1ID<32;)
			{
				Id = SenB1ID;
				if(BUS1.Sie[SenB1ID].Sts.B.Hab && BUS1.Sie[SenB1ID].Sts.B.Det)
				{
					//Sensor habilitado para lectura
					break;			
				}
				else
				{
					Med_Sen_Bus1(Id,0);
					BUS1.Sie[SenB1ID].Med = 0;
					BUS1.Sie[SenB1ID].tMed = 0;
			//		BUS1.Sie[SenB1ID].Sts.C.Aux = 0;
					BUS1.Sie[SenB1ID].Sts.B.Con = false;
					BUS1.Sie[SenB1ID].Sts.B.Act = false;
					BUS1.Sie[SenB1ID].Sts.B.AvTs = false;
					BUS1.Sie[SenB1ID].Sts.B.FDs = false;			
				}
				SenB1ID++;
			}
			if(SenB1ID>=32)
			{
				SenB1ID = 0;
				Adq_SelTask0011=4;
				goto FerLIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();	
		break;
		case 3:
			Id = SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				Medicion = *(unsigned int*)&SW1.buf[0];
				Med_Sen_Bus1(Id,Medicion);
				BUS1.Sie[SenB1ID].Sts.B.Con = true;
				BUS1.Sie[SenB1ID].Sts.B.FDs = false;
				BUS1.Sie[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !BUS1.Sie[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					ErrorB1=0;
					if(BUS1.Sie[SenB1ID].Sts.B.AxDesc)
					{
						Med_Sen_Bus1(Id,0);
						BUS1.Sie[SenB1ID].Sts.B.Con = false;
						BUS1.Sie[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						BUS1.Sie[SenB1ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;		
			if(SenB1ID<32)
			{
				Adq_SelTask0011=2;
				goto SemLIN1;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011=4;
			}	
		case 4:
//Lectura de datos sensores de fertilizante
FerLIN1:
			for(;SenB1ID<32;)
			{
				Id = 0x20 + SenB1ID;
				if(BUS1.Fer[SenB1ID].Sts.B.Hab && BUS1.Fer[SenB1ID].Sts.B.Det )
				{
					//Sensor habilitado para lectura
					break;
				}
				else
				{
					Med_Sen_Bus1(Id,0);
					BUS1.Fer[SenB1ID].Med = 0;
					BUS1.Fer[SenB1ID].tMed = 0;
			//		BUS1.Fer[SenB1ID].Sts.C.Aux = 0;
					BUS1.Fer[SenB1ID].Sts.B.Con = false;
					BUS1.Fer[SenB1ID].Sts.B.Act = false;
					BUS1.Fer[SenB1ID].Sts.B.AvTs = false;
					BUS1.Fer[SenB1ID].Sts.B.FDs = false;
				}
				SenB1ID++;
			}	
			if(SenB1ID>=32)
			{
				SenB1ID = 0;
				Adq_SelTask0011=6;
				Proceso.B.fAdqSie1 = true;
				goto RotLIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();
		break;
		case 5:
			Id = 0x20 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				Medicion = *(unsigned int*)&SW1.buf[0];
				Med_Sen_Bus1(Id,Medicion);
				BUS1.Fer[SenB1ID].Sts.B.Con = true;
				BUS1.Fer[SenB1ID].Sts.B.FDs = false;
				BUS1.Fer[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !BUS1.Fer[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					ErrorB1=0;	
					if(BUS1.Fer[SenB1ID].Sts.B.AxDesc)
					{				
						Med_Sen_Bus1(Id,0);
						BUS1.Fer[SenB1ID].Sts.B.Con = false;
						BUS1.Fer[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						BUS1.Fer[SenB1ID].Sts.B.AxDesc = true;
					}	
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<32)
			{
				Adq_SelTask0011=4;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Proceso.B.fAdqSie1 = true;
				Adq_SelTask0011=6;
			}
		case 6:
//Lectura de datos sensores de rotacion
RotLIN1:
			Proceso.B.fAdqRot1 = false;
			for(;SenB1ID<8;)
			{
				Id = 0x40 + SenB1ID;
//!Rotacio[SenB1ID].Sts.B.Bus indica que esta en el bus 1
				if(!Rotacion[SenB1ID].Sts.B.Bus)
				{
					if(Rotacion[SenB1ID].Sts.B.Hab && Rotacion[SenB1ID].Sts.B.Det )
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Rotacion[SenB1ID].Med = 0;
						Rotacion[SenB1ID].Sts.B.Con = false;
						Rotacion[SenB1ID].Sts.B.FMin = false;
						Rotacion[SenB1ID].Sts.B.FMax = false;
						Rotacion[SenB1ID].Sts.B.FDs = false;	
					}
				}
				SenB1ID++;
			}
			if(SenB1ID>=8)
			{
				SenB1ID = 0;
				Adq_SelTask0011=8;
				Proceso.B.fAdqRot1 = true;
				goto TRBLIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();
			
		break;
		case 7:
			Id = 0x40 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				Rotacion[SenB1ID].Med = *(unsigned int*)&SW1.buf[0];
				Rotacion[SenB1ID].Sts.B.Con = true;
				Rotacion[SenB1ID].Sts.B.FDs = false;
				Rotacion[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Rotacion[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					if(Rotacion[SenB1ID].Sts.B.AxDesc)
					{
						ErrorB1=0;					
						Rotacion[SenB1ID].Med = 0;
						Rotacion[SenB1ID].Sts.B.Con = false;
						Rotacion[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						Rotacion[SenB1ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<8)
			{
				Adq_SelTask0011=6;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011=8;
				Proceso.B.fAdqRot1 = true;
			}
		case 8:
//Lectura de datos sensores de rotacion
TRBLIN1:
			Proceso.B.fAdqTRB1 = false;
			for(;SenB1ID<3;)
			{
				Id = 0xD3 + SenB1ID;
//!Turbina[SenB1ID].Sts.B.Bus indica que esta en el bus 1
				if( !Turbina[SenB1ID].Sts.B.Bus)
				{
					if(Turbina[SenB1ID].Sts.B.Hab && Turbina[SenB1ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Turbina[SenB1ID].Med = 0;
						Turbina[SenB1ID].Sts.B.Con = false;
						Turbina[SenB1ID].Sts.B.FMin = false;
						Turbina[SenB1ID].Sts.B.FMax = false;
						Turbina[SenB1ID].Sts.B.FDs = false;
					}
				}
				SenB1ID++;
			}
			
			if(SenB1ID>=3)
			{
				SenB1ID = 0;
				Adq_SelTask0011=10;
				Proceso.B.fAdqTRB1 = true;
				goto ModLIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();
		break;
		case 9:
			Id = 0xD3 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				Turbina[SenB1ID].Med = *(unsigned int*)&SW1.buf[0];
				Turbina[SenB1ID].Sts.B.Con = true;
				Turbina[SenB1ID].Sts.B.FDs = false;
				Turbina[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Turbina[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					if(Turbina[SenB1ID].Sts.B.AxDesc)
					{
						ErrorB1=0;					
						Turbina[SenB1ID].Med = 0;
						Turbina[SenB1ID].Sts.B.Con = false;
						Turbina[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						Turbina[SenB1ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<8)
			{
				Adq_SelTask0011=8;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011=10;
				Proceso.B.fAdqTRB1 = true;
			}
		case 10:
//Lectura de datos de la moduladora
ModLIN1:
			Proceso.B.fAdqMod1 = false;
			for(;SenB1ID<16;)
			{		
//!Moduladora[SenB1ID].Sts.B.Bus indica que esta en el bus 1
				if(!Moduladora[SenB1ID].Sts.B.Bus)
				{
					if(Moduladora[SenB1ID].Sts.B.Hab && Moduladora[SenB1ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						Id = ModDirId(SenB1ID);
						break;
					}
					else 
					{
						Moduladora[SenB1ID].Vel = 0;
						Moduladora[SenB1ID].Dis = 0;
						Moduladora[SenB1ID].Pul = 0;
						Moduladora[SenB1ID].Al.Val = 0;
						Moduladora[SenB1ID].Sts.B.Con = false;
						Moduladora[SenB1ID].Sts.B.FDs = false;
					}
				}
				SenB1ID++;
			}
			if(SenB1ID>=16)
			{
				SenB1ID = 0;
				Adq_SelTask0011=12;
				Proceso.B.fAdqMod1 = true;
				goto escModLIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();
		break;
		case 11:
			if (SW1PortSys.Sts.B.fOk)
			{
//		Moduladora[SenB1ID].Al.Val = *(unsigned char*) &SW1.buf[0];
				if (SW1.buf[1] > 1 && !Moduladora[SenB1ID].Aux.B.FLecB1)
				{
					Moduladora[SenB1ID].Aux.B.FLecB1 = true;
					Id = ModDirId(SenB1ID);
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
				else if(SW1.buf[1] <= 1)
				{
					Moduladora[SenB1ID].Aux.B.FLecB1 = false;
				}
				Moduladora[SenB1ID].Vel = *(unsigned int*)&SW1.buf[0];
				Moduladora[SenB1ID].Sts.B.Con = true;
				Moduladora[SenB1ID].Sts.B.FDs = false;
				Moduladora[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Moduladora[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					if(Moduladora[SenB1ID].Sts.B.AxDesc)
					{
						ErrorB1=0;					
						Moduladora[SenB1ID].Al.Val = 0;
						Moduladora[SenB1ID].Vel = 0;
						Moduladora[SenB1ID].Sts.B.Con = false;
						Moduladora[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						Moduladora[SenB1ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB1++;
					Id = ModDirId(SenB1ID);
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<16)
			{
				Adq_SelTask0011=10;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Proceso.B.fAdqMod1 = true;
				Adq_SelTask0011=12;
			}
//Escribe SP kD una vez por segundo
		case 12:
		case 13:
escModLIN1:
			SenB1ID = 0;
			Adq_SelTask0011=14;
			Proceso.B.fAdqMod1 = true;
			Sts_Tmr.TMRModB1 = 0;
				
		case 14:
//Lectura de datos sensores de Nivel de tolva
TolLIN1:
			Proceso.B.fAdqNTL1 = false;
			for(;SenB1ID<16;)
			{
				Id = 0x48 + SenB1ID;
				if(!Tolva[SenB1ID].Sts.B.Bus)
				{
					if(Tolva[SenB1ID].Sts.B.Hab && Tolva[SenB1ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Tolva[SenB1ID].Alcont = 0;
						Tolva[SenB1ID].Sts.B.Con = false;
						Tolva[SenB1ID].Sts.B.SNV = false;
						Tolva[SenB1ID].Sts.B.FDs = false;
					}
				}
				SenB1ID++;
			}
			if(SenB1ID>=16)
			{
				SenB1ID = 0;
				Adq_SelTask0011=16;
				Proceso.B.fAdqNTL1 = true;
				goto PRELIN1;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,1);
			SW1_PortSysSend();
		break;
		case 15:
			Id = 0x48 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				if(SW1.buf[0]== 0xFF)
				{
					if(Tolva[SenB1ID].Alcont < 9)
					{
						Tolva[SenB1ID].Alcont ++;
					}
					else
					{
						Tolva[SenB1ID].Sts.B.SNV = true;
					}	
				}
				else
				{
					Tolva[SenB1ID].Alcont = 0;
					Tolva[SenB1ID].Sts.B.SNV = false;
				}
				Tolva[SenB1ID].Sts.B.FDs = false;
				Tolva[SenB1ID].Sts.B.Con = true;
				Tolva[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Tolva[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					if(Tolva[SenB1ID].Sts.B.AxDesc)
					{
						ErrorB1=0;	
						Tolva[SenB1ID].Alcont = 0;				
						Tolva[SenB1ID].Sts.B.SNV = false;
						Tolva[SenB1ID].Sts.B.Con = false;
						Tolva[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						Tolva[SenB1ID].Sts.B.AxDesc = true;
					}	
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,1);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<16)
			{
				Adq_SelTask0011=14;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011++;
				Proceso.B.fAdqNTL1 = true;
			}
		break;
		case 16:
//Lectura de datos sensores de rotacion
PRELIN1:
			Proceso.B.fAdqPRE1 = false;
			for(;SenB1ID<9;)
			{
				Id = 0x80 + SenB1ID;
//!Turbina[SenB1ID].Sts.B.Bus indica que esta en el bus 1
				if( !Presion[SenB1ID].Sts.B.Bus)
				{
					if(Presion[SenB1ID].Sts.B.Hab && Presion[SenB1ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Presion[SenB1ID].Med = 0;
						Presion[SenB1ID].Sts.B.Con = false;
						Presion[SenB1ID].Sts.B.FMin = false;
						Presion[SenB1ID].Sts.B.FMax = false;
						Presion[SenB1ID].Sts.B.FDs = false;
					}
				}
				SenB1ID++;
			}
			
			if(SenB1ID>=9)
			{
				SenB1ID = 0;
				Adq_SelTask0011=0;
				Proceso.B.fAdqPRE1 = true;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
			SW1_PortSysSend();
		break;
		case 17:
			Id = 0x80 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				Presion[SenB1ID].Med = *(unsigned int*)&SW1.buf[0];
				Presion[SenB1ID].Sts.B.Con = true;
				Presion[SenB1ID].Sts.B.FDs = false;
				Presion[SenB1ID].Sts.B.AxDesc = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Presion[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					if(Presion[SenB1ID].Sts.B.AxDesc)
					{
						ErrorB1=0;					
						Presion[SenB1ID].Med = 0;
						Presion[SenB1ID].Sts.B.Con = false;
						Presion[SenB1ID].Sts.B.FDs = true;
					}
					else
					{
						Presion[SenB1ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			SenB1ID++;
			if(SenB1ID<9)
			{
				Adq_SelTask0011=16;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011=0;
				Proceso.B.fAdqPRE1 = true;
			}
	}		
}

