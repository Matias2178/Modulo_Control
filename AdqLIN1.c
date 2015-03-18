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
	
	
_NEAR char SelTask0010;
_NEAR char SelTask0011;	
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
			TDispLin1	=	DispLin1;
			TDispActLin1	=DispActLin1;
			TDispErrLin1	=DispErrLin1;			
			DispLin1=0;
			DispActLin1=0;
			DispErrLin1=0;			
			SenB1ID = 0;
			Adq_SelTask0011=2;
//		break;
//		case 1:
//		
//		break;
		case 2:
		//Lectura de datos sensores de semilla
SemLIN1:
//			Proceso.B.fAdqSie1 = false;
			for(;SenB1ID<32;)
			{
				Id = SenB1ID;
			//	if(BUS1.Sie[SenB1ID].Sts.B.Hab && BUS1.Sie[SenB1ID].Sts.B.Con)
			//Usar despues de que se configure el sensor
				if(BUS1.Sie[SenB1ID].Sts.B.Hab && BUS1.Sie[SenB1ID].Sts.B.Det)
				{
					//Sensor habilitado para lectura
					DispLin1++;
					break;			
				}
				else
				{
					Med_Sen_Bus1(Id,0);
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
				DispActLin1++;
				Medicion = *(unsigned int*)&SW1.buf[0];
				Med_Sen_Bus1(Id,Medicion);
				BUS1.Sie[SenB1ID].Sts.B.Con = true;
				BUS1.Sie[SenB1ID].Sts.B.FDs = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !BUS1.Sie[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;
					Med_Sen_Bus1(Id,0);
					BUS1.Sie[SenB1ID].Sts.B.Con = false;
					BUS1.Sie[SenB1ID].Sts.B.FDs = true;
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			else	DispErrLin1++;
///*			DtsComBus1.Sie.Sts[SenB1ID] = BUS1.Sie[SenB1ID].Sts.Val;
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
			//Controlar de aca en adelante
FerLIN1:
			for(;SenB1ID<32;)
			{
				Id = 0x20 + SenB1ID;
			//	if(BUS1.Fer[SenB1ID].Sts.B.Hab && BUS1.Fer[SenB1ID].Sts.B.Con)
			//Usar despues de que se configure el sensor
				if(BUS1.Fer[SenB1ID].Sts.B.Hab && BUS1.Fer[SenB1ID].Sts.B.Det )
				{
					DispLin1++;
					//Sensor habilitado para lectura
					break;
				}
				else
				{
					Med_Sen_Bus1(Id,0);
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
				DispActLin1++;
				Medicion = *(unsigned int*)&SW1.buf[0];
				Med_Sen_Bus1(Id,Medicion);
				BUS1.Fer[SenB1ID].Sts.B.Con = true;
				BUS1.Fer[SenB1ID].Sts.B.FDs = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !BUS1.Fer[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;					
					Med_Sen_Bus1(Id,0);
					BUS1.Fer[SenB1ID].Sts.B.Con = false;
					BUS1.Fer[SenB1ID].Sts.B.FDs = true;
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			else DispErrLin1++;
			DtsComBus1.Fer.Sts[SenB1ID] = BUS1.Fer[SenB1ID].Sts.Val;
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
	//	break;
		case 6:
			//Lectura de datos sensores de rotacion
			//Controlar de aca en adelante
RotLIN1:
			Proceso.B.fAdqRot1 = false;
			for(;SenB1ID<8;)
			{
				Id = 0x40 + SenB1ID;
			//Ojo que 	!Rotacio[SenB1ID].Sts.B.Bus indica que esta en el bus 1
			//	if(Rotacion[SenB1ID].Sts.B.Hab && Rotacion[SenB1ID].Sts.B.Con && !Rotacion[SenB1ID].Sts.B.Bus)
			//Usar despues de que se configure el sensor
				if(!Rotacion[SenB1ID].Sts.B.Bus)
				{
					if(Rotacion[SenB1ID].Sts.B.Hab && Rotacion[SenB1ID].Sts.B.Det )
					{
						DispLin1++;
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						Rotacion[SenB1ID].Med = 0;
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
				DispActLin1++;
				Rotacion[SenB1ID].Med = *(unsigned int*)&SW1.buf[0];
				Rotacion[SenB1ID].Sts.B.Con = true;
				Rotacion[SenB1ID].Sts.B.FDs = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Rotacion[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;					
					Rotacion[SenB1ID].Med = 0;
					Rotacion[SenB1ID].Sts.B.Con = false;
					Rotacion[SenB1ID].Sts.B.FDs = true;
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			else DispErrLin1++;
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
//		break;
		case 8:
			//Lectura de datos sensores de rotacion
			//Controlar de aca en adelante
TRBLIN1:
			Proceso.B.fAdqTRB1 = false;
			for(;SenB1ID<3;)
			{
				Id = 0xD3 + SenB1ID;
			//Ojo que 	!Turbina[SenB1ID].Sts.B.Bus indica que esta en el bus 1
			//Usar despues de que se configure el sensor
				if( !Turbina[SenB1ID].Sts.B.Bus)
				{
					if(Turbina[SenB1ID].Sts.B.Hab && Turbina[SenB1ID].Sts.B.Det)
					{
						//Sensor habilitado para lectura
						DispLin1++;
						break;
					}
					else
					{
						Turbina[SenB1ID].Med = 0;
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
				DispActLin1++;
				Turbina[SenB1ID].Med = *(unsigned int*)&SW1.buf[0];
				Turbina[SenB1ID].Sts.B.Con = true;
				Turbina[SenB1ID].Sts.B.FDs = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Turbina[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;					
					Turbina[SenB1ID].Med = 0;
					Turbina[SenB1ID].Sts.B.Con = false;
					Turbina[SenB1ID].Sts.B.FDs = true;
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
					SW1_PortSysSend();
					break;
				}
			}
			else DispErrLin1++;
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
//		break;
		case 10:
			//Lectura de datos de la moduladora
			//Controlar de aca en adelante
ModLIN1:
			Proceso.B.fAdqMod1 = false;
			for(;SenB1ID<16;)
			{
				
					
			//Ojo que 	!Moduladora[SenB1ID].Sts.B.Bus indica que esta en el bus 1
			//	if(Moduladora[SenB1ID].Sts.B.Hab && Moduladora[SenB1ID].Sts.B.Con && !Moduladora[SenB1ID].Sts.B.Bus)
			//Usar despues de que se configure el sensor
				if(!Moduladora[SenB1ID].Sts.B.Bus)
				{
					if(Moduladora[SenB1ID].Sts.B.Hab && Moduladora[SenB1ID].Sts.B.Det)
					{
						DispLin1++;
						Id = ModDirId(SenB1ID);
						//Sensor habilitado para lectura
						break;
					}
					else 
					{
						Moduladora[SenB1ID].Vel = 0;
						Moduladora[SenB1ID].Al.Val = 0;
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
				goto TolLIN1;
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
				DispActLin1++;
		//		Moduladora[SenB1ID].Al.Val = *(unsigned char*) &SW1.buf[0];
				Moduladora[SenB1ID].Vel = *(unsigned int*)&SW1.buf[0];
				Moduladora[SenB1ID].Sts.B.Con = true;
				Moduladora[SenB1ID].Sts.B.FDs = false;
			}
			else if(SW1PortSys.Sts.B.fErr && !Moduladora[SenB1ID].Sts.B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;					
					Moduladora[SenB1ID].Al.Val = 0;
					Moduladora[SenB1ID].Vel = 0;
					Moduladora[SenB1ID].Sts.B.Con = false;
					Moduladora[SenB1ID].Sts.B.FDs = true;
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
			else DispErrLin1++;
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
//		break;
		
		case 12:
			//Lectura de datos sensores de Nivel de tolva
			//Controlar de aca en adelante
TolLIN1:
			Proceso.B.fAdqNTL1 = false;
			for(;SenB1ID<16;)
			{
				Id = 0x48 + SenB1ID;
			//Ojo que 	!Turbina[SenB1ID].Sts.B.Bus indica que esta en el bus 1
			//	if(Turbina[SenB1ID].Sts.B.Hab && Turbina[SenB1ID].Sts.B.Con && !Turbina[SenB1ID].Sts.B.Bus)
			//Usar despues de que se configure el sensor
				if(!SenTol[SenB1ID].B.Bus)
				{
					if(SenTol[SenB1ID].B.Hab && SenTol[SenB1ID].B.Det)
					{
						DispLin1++;
						//Sensor habilitado para lectura
						break;
					}
					else
					{
						SenTol[SenB1ID].C.Alcont = 0;
						SenTol[SenB1ID].B.FDs = false;
						SenTol[SenB1ID].B.SNV = false;
					}
				}
				SenB1ID++;
			}
			
			if(SenB1ID>=16)
			{
				SenB1ID = 0;
				Adq_SelTask0011=0;
				Proceso.B.fAdqNTL1 = true;
				break;
			}
			ErrorB1=0;
			Adq_SelTask0011++;
			
			SW1_PortSysStart(Id,0x00 | SW1_cmdRd,1);
			SW1_PortSysSend();
		break;
		case 13:
			Id = 0x48 + SenB1ID;
			if (SW1PortSys.Sts.B.fOk)
			{
				DispActLin1++;
				if(SW1.buf[0]== 0xFF)
				{
					if(SenTol[SenB1ID].C.Alcont < 9)
					{
						SenTol[SenB1ID].C.Alcont ++;
					}
					else
					{
						SenTol[SenB1ID].B.SNV = true;
					}
					
				}
				else
				{
					SenTol[SenB1ID].C.Alcont = 0;
					SenTol[SenB1ID].B.SNV = false;
				}
				SenTol[SenB1ID].B.FDs = false;
				SenTol[SenB1ID].B.Con = true;
			}
			else if(SW1PortSys.Sts.B.fErr && !SenTol[SenB1ID].B.FDs)
			{
				if(ErrorB1>=2)
				{
					DispErrLin1++;
					ErrorB1=0;	
					SenTol[SenB1ID].C.Alcont = 0;				
					SenTol[SenB1ID].B.SNV = false;
					SenTol[SenB1ID].B.Con = false;
					SenTol[SenB1ID].B.FDs = true;
				}
				else
				{
					ErrorB1++;
					SW1_PortSysStart(Id,0x00 | SW1_cmdRd,1);
					SW1_PortSysSend();
					break;
				}
			}
			else DispErrLin1++;
			SenB1ID++;
			if(SenB1ID<16)
			{
				Adq_SelTask0011=12;
				break;	
			}
			else 
			{
				SenB1ID = 0;
				Adq_SelTask0011=0;
				Proceso.B.fAdqNTL1 = true;
			}
		break;
/*		
		case 14:
		
		break;
		case 15:
		
		break;
		case 16:
		
		break;
*/
	}		
}

