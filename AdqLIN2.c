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
	#include 	"sw2master.h"
	#include	"Perifericos.h"
	#include	"ES_Conf.h"
	#include 	<p24Fxxxx.h>
	
_NEAR char ErrorB2;
_NEAR int SenB2ID;

/******************************************************************************
*	Funcion:		Adq_Proc_Lin2()	
*	Descricpion:	Adquiere los datos de proceso de los sensores conectados
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void Adq_Proc_Lin2(void)
{
	unsigned int Id;
	unsigned int Medicion;
	union _UInt32 LocUserDW00;
	
	if (SW2PortSys.Sts.B.fPend)	//Hay una comunicacion pendiente de ser atendida
		return;
		
	switch(Adq_SelTask0021){
		default:
			Adq_SelTask0021=0;

		case 0:
//Control del tiempo de vuleta del bus lin
			TLin2.Ult[TLin2.ind] = TLin2.Timer;
			TLin2.Timer = 0;
			TLin2.ind++;
			if(TLin2.ind>=10)
			{
				TLin2.ind = 0;
			}			
//Control de dispositivos en el bus lin
			SenB2ID 		= 	0;
			Adq_SelTask0021	=	2;
		case 2:
//Lectura de datos sensores de semilla
SemLIN2:		
			for(;SenB2ID<32;)
			{
				Id = SenB2ID;
				if(BUS2.Sie[SenB2ID].Sts.B.Hab && BUS2.Sie[SenB2ID].Sts.B.Det )
				{
					//Sensor habilitado para lectura
					break;
				}
				else
				{
					Med_Sen_Bus2(Id,0);					
					BUS2.Sie[SenB2ID].Med = 0;
					BUS2.Sie[SenB2ID].tMed = 0;
//					BUS2.Sie[SenB2ID].Sts.C.Aux = 0;
					BUS2.Sie[SenB2ID].Sts.B.Con = false;
					BUS2.Sie[SenB2ID].Sts.B.Act = false;
					BUS2.Sie[SenB2ID].Sts.B.AvTs = false;
					BUS2.Sie[SenB2ID].Sts.B.FDs = false;
				}
				SenB2ID++;
			}
			if(SenB2ID>=32)
			{
				SenB2ID = 0;
				Adq_SelTask0021=4;
				goto FerLIN2;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
			SW2_PortSysSend();
		break;
		case 3:
			Id = SenB2ID;
			if (SW2PortSys.Sts.B.fOk)
			{
				Medicion = *(unsigned int*)&SW2.buf[0];
				Med_Sen_Bus2(Id,Medicion);
				BUS2.Sie[SenB2ID].Sts.B.Con = true;
				BUS2.Sie[SenB2ID].Sts.B.FDs = false;
				BUS2.Sie[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !BUS2.Sie[SenB2ID].Sts.B.FDs)
			{
				if(ErrorB2>=2)
				{
					ErrorB2=0;
					if(BUS2.Sie[SenB2ID].Sts.B.AxDesc)
					{
						Med_Sen_Bus2(Id,0);
						BUS2.Sie[SenB2ID].Sts.B.Con = false;
						BUS2.Sie[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						BUS2.Sie[SenB2ID].Sts.B.AxDesc = true;
					}	
				}
				else
				{
					ErrorB2++;
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<32)
			{
				Adq_SelTask0021=2;
				goto SemLIN2;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Adq_SelTask0021=4;
			}	
		case 4:
//Lectura de datos sensores de fertilizante
FerLIN2:
			for(;SenB2ID<32;)
			{
				Id = 0x20 + SenB2ID;
				if(BUS2.Fer[SenB2ID].Sts.B.Hab && BUS2.Fer[SenB2ID].Sts.B.Det)
				{
					//Sensor habilitado para lectura
					break;
				}
				else
				{
				//	Med_Sen_Bus2(Id,0);					
					BUS2.Fer[SenB2ID].Med = 0;
					BUS2.Fer[SenB2ID].tMed = 0;
				//	BUS2.Fer[SenB2ID].Sts.C.Aux = 0;
					BUS2.Fer[SenB2ID].Sts.B.Con = false;
					BUS2.Fer[SenB2ID].Sts.B.Act = false;
					BUS2.Fer[SenB2ID].Sts.B.AvTs = false;
					BUS2.Fer[SenB2ID].Sts.B.FDs = false;
				}
				SenB2ID++;
			}
			if(SenB2ID>=32)
			{
				SenB2ID = 0;
				Adq_SelTask0021=6;
				Proceso.B.fAdqSie2 = true;
				goto RotLIN2;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
			SW2_PortSysSend();
		break;
		case 5:
			Id = 0x20 + SenB2ID;
			if (SW2PortSys.Sts.B.fOk)
			{
				Medicion = *(unsigned int*)&SW2.buf[0];
				Med_Sen_Bus2(Id,Medicion);
				BUS2.Fer[SenB2ID].Sts.B.Con = true;
				BUS2.Fer[SenB2ID].Sts.B.FDs = false;
				BUS2.Fer[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !BUS2.Fer[SenB2ID].Sts.B.FDs) 
			{
				if(ErrorB2>=2)
				{
					ErrorB2=0;	
					if(BUS2.Fer[SenB2ID].Sts.B.AxDesc)
					{				
						Med_Sen_Bus2(Id,0);
						BUS2.Fer[SenB2ID].Sts.B.Con = false;
						BUS2.Fer[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						BUS2.Fer[SenB2ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB2++;
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<32)
			{
				Adq_SelTask0021=4;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Proceso.B.fAdqSie2 = true;
				Adq_SelTask0021=6;
			}
		case 6:
//Lectura de datos sensores de rotacion
RotLIN2:
			Proceso.B.fAdqRot2 = false;
			for(;SenB2ID<8;)
			{
				Id = 0x40 + SenB2ID;
//!Rotacio[SenB2ID].Sts.B.Bus indica que esta en el bus 1
				if(Rotacion[SenB2ID].Sts.B.Bus)
				{
					if(Rotacion[SenB2ID].Sts.B.Hab && Rotacion[SenB2ID].Sts.B.Det )
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Rotacion[SenB2ID].Med = 0;
						Rotacion[SenB2ID].Sts.B.Con = false;
						Rotacion[SenB2ID].Sts.B.FMin = false;
						Rotacion[SenB2ID].Sts.B.FMax = false;
						Rotacion[SenB2ID].Sts.B.FDs = false;	
					}
				}
				SenB2ID++;
			}
			if(SenB2ID>=8)
			{
				SenB2ID = 0;
				Adq_SelTask0021=8;
				Proceso.B.fAdqRot2 = true;
				goto TRBLIN2;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
			SW2_PortSysSend();
			
		break;
		case 7:
			Id = 0x40 + SenB2ID;
			if (SW2PortSys.Sts.B.fOk)
			{
				Rotacion[SenB2ID].Med = *(unsigned int*)&SW2.buf[0];
				Rotacion[SenB2ID].Sts.B.Con = true;
				Rotacion[SenB2ID].Sts.B.FDs = false;
				Rotacion[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !Rotacion[SenB2ID].Sts.B.FDs)
			{
				if(ErrorB2>=2)
				{
					if ( Rotacion[SenB2ID].Sts.B.AxDesc)
					{
						ErrorB2=0;					
						Rotacion[SenB2ID].Med = 0;
						Rotacion[SenB2ID].Sts.B.Con = false;
						Rotacion[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						Rotacion[SenB2ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB2++;
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<8)
			{
				Adq_SelTask0021=6;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Adq_SelTask0021=8;
				Proceso.B.fAdqRot2 = true;
			}
		case 8:
//Lectura de datos sensores de rotacion
TRBLIN2:
			Proceso.B.fAdqTRB2 = false;
			for(;SenB2ID<3;)
			{
				Id = 0xD3 + SenB2ID;
//!Turbina[SenB2ID].Sts.B.Bus indica que esta en el bus 1
				if(Turbina[SenB2ID].Sts.B.Bus)
				{
					if(Turbina[SenB2ID].Sts.B.Hab && Turbina[SenB2ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Turbina[SenB2ID].Med = 0;
						Turbina[SenB2ID].Sts.B.Con = false;
						Turbina[SenB2ID].Sts.B.FMin = false;
						Turbina[SenB2ID].Sts.B.FMax = false;
						Turbina[SenB2ID].Sts.B.FDs = false;
					}
				}
				SenB2ID++;
			}
			
			if(SenB2ID>=3)
			{
				SenB2ID = 0;
				Adq_SelTask0021=10;
				Proceso.B.fAdqTRB2 = true;
				goto ModLIN2;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
			SW2_PortSysSend();
		break;
		case 9:
			Id = 0xD3 + SenB2ID;
			if (SW2PortSys.Sts.B.fOk)
			{
				Turbina[SenB2ID].Med = *(unsigned int*)&SW2.buf[0];
				Turbina[SenB2ID].Sts.B.Con = true;
				Turbina[SenB2ID].Sts.B.FDs = false;
				Turbina[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !Turbina[SenB2ID].Sts.B.FDs)
			{
				if(ErrorB2>=2)
				{
					if( Turbina[SenB2ID].Sts.B.AxDesc)
					{
						ErrorB2=0;					
						Turbina[SenB2ID].Med = 0;
						Turbina[SenB2ID].Sts.B.Con = false;
						Turbina[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						Turbina[SenB2ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB2++;
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<8)
			{
				Adq_SelTask0021=8;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Adq_SelTask0021=10;
				Proceso.B.fAdqTRB2 = true;
			}
		case 10:
//Lectura de datos de la moduladora
ModLIN2:
			Proceso.B.fAdqMod2 = false;
			for(;SenB2ID<16;)
			{
//Moduladora[SenB2ID].Sts.B.Bus indica que esta en el bus 2
				if(Moduladora[SenB2ID].Sts.B.Bus)
				{
					if(Moduladora[SenB2ID].Sts.B.Hab && Moduladora[SenB2ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						Id = ModDirId(SenB2ID);	
						break;
					}
					else 
					{
						Moduladora[SenB2ID].Vel = 0;
						Moduladora[SenB2ID].Dis = 0;
						Moduladora[SenB2ID].Pul = 0;
						Moduladora[SenB2ID].Al.Val = 0;
						Moduladora[SenB2ID].Sts.B.Con = false;
						Moduladora[SenB2ID].Sts.B.FDs = false;
					}
				}
				SenB2ID++;
			}
			if(SenB2ID>=16)
			{
				SenB2ID = 0;
				Adq_SelTask0021=12;
				Proceso.B.fAdqMod2 = true;
				goto escModLIN2;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
			SW2_PortSysSend();
		break;
		case 11:
			if (SW2PortSys.Sts.B.fOk)
			{
//		Moduladora[SenB2ID].Al.Val = *(unsigned char*) &SW2.buf[0];
//				if(SW2.buf[1] <= 1)
//				{
//					Moduladora[SenB2ID].Vel = *(unsigned int*)&SW2.buf[0];
//				}
//				else
				if (SW2.buf[1] > 1 && !Moduladora[SenB2ID].Aux.B.FLecB2)
				{
					LED_CAN = LED_CAN ? 0 : 1;
					Moduladora[SenB2ID].Aux.B.FLecB2 = true;
					Id = ModDirId(SenB2ID);
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
				else if(SW2.buf[1] <= 1)
				{
					Moduladora[SenB2ID].Aux.B.FLecB2 = false;
				}
				Moduladora[SenB2ID].Vel = *(unsigned int*)&SW2.buf[0];
				Moduladora[SenB2ID].Sts.B.Con = true;
				Moduladora[SenB2ID].Sts.B.FDs = false;
				Moduladora[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !Moduladora[SenB2ID].Sts.B.FDs)
			{
				if(ErrorB2>=2)
				{
					if( Moduladora[SenB2ID].Sts.B.AxDesc)
					{
						ErrorB2=0;					
						Moduladora[SenB2ID].Al.Val = 0;
						Moduladora[SenB2ID].Vel = 0;
						Moduladora[SenB2ID].Sts.B.Con = false;
						Moduladora[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						Moduladora[SenB2ID].Sts.B.AxDesc = true;
					}
				}
				else
				{
					ErrorB2++;
					Id = ModDirId(SenB2ID);
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<16)
			{
				Adq_SelTask0021=10;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Proceso.B.fAdqMod2 = true;
				Adq_SelTask0021=12;
			}
//Escribe SP kD una vez por segundo
		case 12:
		case 13:
escModLIN2:
			SenB2ID = 0;
			Adq_SelTask0021=14;
			Proceso.B.fAdqMod2 = true;
			Sts_Tmr.TMRModB2 = 0;

		case 14:
//Lectura de datos sensores de Nivel de tolva
TolLIN2:
			Proceso.B.fAdqNTL2 = false;
			for(;SenB2ID<16;)
			{
				Id = 0x48 + SenB2ID;
				if(Tolva[SenB2ID].Sts.B.Bus)
				{
					if(Tolva[SenB2ID].Sts.B.Hab && Tolva[SenB2ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Tolva[SenB2ID].Alcont = 0;
						Tolva[SenB2ID].Sts.B.Con = false;
						Tolva[SenB2ID].Sts.B.SNV = false;
						Tolva[SenB2ID].Sts.B.FDs = false;
					}
				}
				SenB2ID++;
			}
			if(SenB2ID>=16)
			{
				SenB2ID = 0;
				Adq_SelTask0021=16;
				Proceso.B.fAdqNTL2 = true;
				break;
			}
			ErrorB2=0;
			Adq_SelTask0021++;
			
			SW2_PortSysStart(Id,0x00 | SW2_cmdRd,1);
			SW2_PortSysSend();
		break;
		case 15:
			Id = 0x48 + SenB2ID;
			if (SW2PortSys.Sts.B.fOk)
			{
				if(SW2.buf[0]== 0xFF)
				{
					if(Tolva[SenB2ID].Alcont < 9)
					{
						Tolva[SenB2ID].Alcont ++;
					}
					else
					{
						Tolva[SenB2ID].Sts.B.SNV = true;
					}
				}
				else
				{
					Tolva[SenB2ID].Alcont = 0;
					Tolva[SenB2ID].Sts.B.SNV = false;
				}
				Tolva[SenB2ID].Sts.B.FDs = false;
				Tolva[SenB2ID].Sts.B.Con = true;
				Tolva[SenB2ID].Sts.B.AxDesc = false;
			}
			else if(SW2PortSys.Sts.B.fErr && !Tolva[SenB2ID].Sts.B.FDs)
			{
				if(ErrorB2>=2)
				{
					if( Tolva[SenB2ID].Sts.B.AxDesc)
					{
						ErrorB2=0;	
						Tolva[SenB2ID].Alcont = 0;				
						Tolva[SenB2ID].Sts.B.SNV = false;
						Tolva[SenB2ID].Sts.B.Con = false;
						Tolva[SenB2ID].Sts.B.FDs = true;
					}
					else
					{
						Tolva[SenB2ID].Sts.B.AxDesc = true;
					}	
				}
				else
				{
					ErrorB2++;
					SW2_PortSysStart(Id,0x00 | SW2_cmdRd,1);
					SW2_PortSysSend();
					break;
				}
			}
			SenB2ID++;
			if(SenB2ID<16)
			{
				Adq_SelTask0021=14;
				break;	
			}
			else 
			{
				SenB2ID = 0;
				Adq_SelTask0021=16;
				Proceso.B.fAdqNTL2 = true;
			}
		break;
	}		
}

