
  	#include	"VarGlob.h"
  	#include	"Constantes.h"
  	#include 	"sw1master.h"
  	#include 	"sw2master.h"
  	#include 	"ExeTask.h"
/******************************************************************************
*	Funcion:		Id = ModDirId(SenID)
*	Descricpion:	Con el numero de Moduladora retorna la direccion que le    
*					correspondes
*	Ingreso Datos:	Numero de Moduladora
*	Salida Datos:	Direccion de la moduladora que le corrersponde
*			
******************************************************************************/	 
char ModDirId(unsigned char ID)
{
	switch(ID)
	{
//		default:
//			return 0x00;
//		break;
		case 0:
		//Siembra Primaria 			#1
			return	0x68;
		break;
		case 1:
		//Siembra Secundaria 		#1
			return	0x6A;
		break;
		case 2:
		//Fertilizante Primario 	#1
			return	0x69;
		break;
		case 3:
		//Fertilizante Secundario 	#1
			return	0x6B;
		break;
		case 4:
		//Siembra Primaria 			#2
			return	0xC8;			
		break;
		case 5:
		//Siembra Secundaria 		#2
			return	0xCA;
		break;
		case 6:
		//Fertilizante Primario 	#2
			return	0xC9;
		break;
		case 7:
		//Fertilizante Secundario 	#2
			return	0xCB;
		break;
		case 8:
		//Siembra Primaria 			#3
			return	0xCC;
		break;
		case 9:
		//Siembra Secundaria 		#3
			return	0xCE;
		break;
		case 10:
		//Fertilizante Primario 	#3
			return	0xCD;
		break;
		case 11:
		//Fertilizante Secundario	#3
			return	0xCF;
		break;
		case 12:
		//Siembra Primaria 			#4
			return	0x64;
		break;
		case 13:
		//Siembra Secundaria 		#4
			return	0x66;
		break;
		case 14:
		//Fertilizante Primario 	#4
			return	0x65;
		break;
		case 15:
		//Fertilizante Secundario	#4
			return	0x67;
		break;
	}
} 
/******************************************************************************
*	Funcion:		Id = ModDirId(SenID)
*	Descricpion:	Con una direccion ingresada indica a que numero de moduladora    
*					corresponde, si no tiene correspondencia retorna cero
*	Ingreso Datos:	ID del dispositivo  Moduladora
*	Salida Datos:	Numero de moduladora o Cero si no corresponde
*			
******************************************************************************/	 
char ModIdNum (unsigned char ID)
{
	switch(ID)
	{
		default:
			return 0;
		break;
		case 0x68:
		//Siembra Primaria 			#1
			return	1;
		break;
		case 0x6A:
		//Siembra Secundaria 		#1
			return	2;
		break;
		case 0x69:
		//Fertilizante Primario 	#1
			return	3;
		break;
		case 0x6B:
		//Fertilizante Secundario 	#1
			return	4;
		break;
		case 0xC8:
		//Siembra Primaria 			#2
			return	5;			
		break;
		case 0xCA:
		//Siembra Secundaria 		#2
			return	6;
		break;
		case 0xC9:
		//Fertilizante Primario 	#2
			return	7;
		break;
		case 0xCB:
		//Fertilizante Secundario 	#2
			return	8;
		break;
		case 0xCC:
		//Siembra Primaria 			#3
			return	9;
		break;
		case 0xCE:
		//Siembra Secundaria 		#3
			return	10;
		break;
		case 0xCD:
		//Fertilizante Primario 	#3
			return	11;
		break;
		case 0xCF:
		//Fertilizante Secundario	#3
			return	12;
		break;
		case 0x64:
		//Siembra Primaria 			#4
			return	13;
		break;
		case 0x66:
		//Siembra Secundaria 		#4
			return	14;
		break;
		case 0x65:
		//Fertilizante Primario 	#4
			return	15;
		break;
		case 0x67:
		//Fertilizante Secundario	#4
			return	16;
		break;
	}
}  	
/******************************************************************************
*	Funcion:		ModStart000()
*	Descricpion:	Escanea los dos buses en busqueda de Moduladoras
*					Realiza la lectura de un dispositivo en los dos buses al
*					mismo tiempo
*					Realiza la lectura de los parametros de SP y KP 
*					Configura el bit que indica en que bus esta conectado
*					Configura el bit que alguna vez se conecto
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	  	
void ModStart000(void)
{
	unsigned char Id;
	unsigned char SenId;
	
	Proceso.B.fConfPer = true;
	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
	
	for(SenId=0;SenId<16;SenId++)
	{
		Id = ModDirId(SenId);
		
		SW1_PortUserStart(Id,0x04 | SW1_cmdRd,4);
		SW2_PortUserStart(Id,0x04 | SW2_cmdRd,4);
		SW1PortUser.Sts.B.fPend=true;
		SW2PortUser.Sts.B.fPend=true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Moduladora[SenId].SP = *(unsigned int*)&SW1PortUser.Data[0];
			Moduladora[SenId].KD = *(unsigned int*)&SW1PortUser.Data[2];

			Moduladora[SenId].Sts.B.Con = true;
			Moduladora[SenId].Sts.B.Bus = false;
//			Moduladora[SenId].Sts.B.Det = true;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Moduladora[SenId].SP = *(unsigned int*)&SW2PortUser.Data[0];
			Moduladora[SenId].KD = *(unsigned int*)&SW2PortUser.Data[2];
			Moduladora[SenId].Sts.B.Con = true;
			Moduladora[SenId].Sts.B.Bus = true;
//			Moduladora[SenId].Sts.B.Det = true;
		}
		else
		{
			Moduladora[SenId].Sts.B.Con = false;
			Moduladora[SenId].Sts.B.Bus = false;
	//		Moduladora[SenId].Sts.B.Det = false;
		}
	}
	Proceso.B.fConfPer = false;
	GrabaConfMod();
}


/******************************************************************************
*	Funcion:		ModAjuste000()
*	Descricpion:	Graba los parametros de SP y KP en una moduladora y luego 
*					controla los parametros grabados
*					Devuelve 1 si el proceso fue exitoso
*					Transimite solamente en el bus en el que esta configurada
*					la moduladora
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char ModAjtSPKD0(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Local1;
	char Error;
	
	SenId = ID;
	Id = ModDirId(SenId);
	
	LocUserDW00.UI.V[1] = Moduladora[SenId].KD;
	LocUserDW00.UI.V[0] = Moduladora[SenId].SP;
	
	Proceso.B.fConfPer = true;
	Proceso.B.fAjMod = true;// Se esta realizando una lectura o un ajuste
							// de parametros de la moduladora
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	Error = 0;
	do{
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			SW1_PortUserStart(Id,0x04 | SW1_cmdWr,4);
			SW1_PortUserWrBuf(&LocUserDW00.UL.V,sizeof(LocUserDW00.UL.V));
			Local1 = SW1_PortUserSend(true);
			SW1PortSys.Sts.Value = 0;
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			SW2_PortUserStart(Id,0x04 | SW2_cmdWr,4);
			SW2_PortUserWrBuf(&LocUserDW00.UL.V,sizeof(LocUserDW00.UL.V));
			Local1 = SW2_PortUserSend(true);
			SW2PortSys.Sts.Value = 0;
		}
		if (Local1)
			break;
		else
			Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	return Local1;
}


/******************************************************************************
*	Funcion:		ModAjuste000()
*	Descricpion:	Lee los parametros de SP y KP en una moduladora y luego 
*					controla los parametros grabados
*					Devuelve 1 si el proceso fue exitoso
*					Transimite solamente en el bus en el que esta configurada
*					la moduladora
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char ModLecSPKD0(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Error;
	
	SenId = ID;
	Id = ModDirId(SenId);		
	Proceso.B.fConfPer = true;
	Proceso.B.fAjMod = true;// Se esta realizando una lectura o un ajuste
							// de parametros de la moduladora
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	Error = 0;
	do{
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			SW1_PortUserStart(Id,0x04 | SW1_cmdRd,4);
			if(SW1_PortUserSend(true))
			{
				LocUserDW00.UL.V=*(unsigned long *)SW1PortUser.Data;
				SW1PortSys.Sts.Value = 0;
				break;
			} 
		}
		else
		{	
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			SW2_PortUserStart(Id,0x04 | SW2_cmdRd,4);
			if(SW2_PortUserSend(true))
			{
				LocUserDW00.UL.V=*(unsigned long *)SW2PortUser.Data;
				SW2PortSys.Sts.Value = 0;
				break;
			}
		}
		Error ++;
	}while(Error<3);
	Proceso.B.fConfPer = false;
	
	if(Error >=3)
		return false;
	Moduladora[SenId].KD = LocUserDW00.UI.V[1];
	Moduladora[SenId].SP = LocUserDW00.UI.V[0];
	Proceso.B.fConfPer = false;
	return true;
}


/******************************************************************************
*	Funcion:		ModConfPul00()
*	Descricpion:	Graba los parametros de Pulsos totales en una moduladora  
*					Transimite solamente en el bus en el que esta configurada
*					la moduladora
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) 
******************************************************************************/	 
char ModConfPul0(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Local1;
	char Error;
		
	SenId = ID;
	Id = ModDirId(SenId);	
	LocUserDW00.UL.V = Moduladora[SenId].Pul;
	
	Proceso.B.fConfPer = true;
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
	Error = 0;
	do{
			
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			SW1_PortUserStart(Id,0x03 | SW1_cmdWr,4);
			SW1_PortUserWrBuf(&LocUserDW00.UL.V,sizeof(LocUserDW00.UL.V));
			Local1 = SW1_PortUserSend(true);
		//	Proceso.B.fConfPer = false;
		//	return Local1;
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			SW2_PortUserStart(Id,0x03 | SW2_cmdWr,4);
			SW2_PortUserWrBuf(&LocUserDW00.UL.V,sizeof(LocUserDW00.UL.V));
			Local1 = SW2_PortUserSend(true);	
		//	Proceso.B.fConfPer = false;
		//	return 	Local1;
		}
			if (Local1)
				break;
			else
				Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	return Local1;
}

/******************************************************************************
*	Funcion:		ModLectura00()
*	Descricpion:	Realiza la lectura de los Metros Recorridos, Pulsos Totales 
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char ModLectPul0(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	
	SenId = ID;
	Id = ModDirId(SenId);	
	Proceso.B.fConfPer = true;
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
		
	if(!Moduladora[SenId].Sts.B.Bus)
	{
		while(SW1PortUser.Sts.B.fPend) ExeTask();	
		
		if(!Sw1_RdPV(Id,0x02,4))
		{
			Proceso.B.fConfPer = false;
			return false;
		}
		Moduladora[SenId].Dis = *(unsigned long *)SW1PortUser.Data;
		
		if(!Sw1_RdPV(Id,0x03,4))
		{
			Proceso.B.fConfPer = false;
			return false;
		}
		Moduladora[SenId].Pul = *(unsigned long *)SW1PortUser.Data;
	}
	else
	{	
		while(SW2PortUser.Sts.B.fPend) ExeTask();
		
		if(!Sw2_RdPV(Id,0x02,4))
		{
			Proceso.B.fConfPer = false;
			return false;
		}
		Moduladora[SenId].Dis = *(unsigned long *)SW2PortUser.Data;
		
		if(!Sw2_RdPV(Id,0x03,4))
		{
			Proceso.B.fConfPer = false;
			return false;
		}
		Moduladora[SenId].Pul = *(unsigned long *)SW2PortUser.Data;	
	}
	Proceso.B.fConfPer = false;
	return true;
}



/******************************************************************************
*	Funcion:		ModConfPulKm()
*	Descricpion:	Graba los parametros de Pulsos por Kilometro para poder   
*					Calcular la distancia y velocidad que lee la moduladora   
*					la moduladora
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true comunicacion OK
*					false fallo comunicacion 
******************************************************************************/	 
char ModConfPulKm(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Local1;
	char Error;
	
	SenId = ID;
	Id = ModDirId(SenId);	
	Proceso.B.fConfPer = true;
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
	LocUserDW00.UL.V = Moduladora[SenId].PKm;
	Error = 0;
	do{
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw1_WrReg(Id,0x80,&LocUserDW00.UL.V);
	//		Proceso.B.fConfPer = false;
	//		return Local1;
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw2_WrReg(Id,0x80,&LocUserDW00.UL.V);
	//		Proceso.B.fConfPer = false;
	//		return 	Local1;	
		}

		if (Local1)
			break;
		else
			Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	return Local1;	
	}


/******************************************************************************
*	Funcion:		ModLectPulKm()
*	Descricpion:	Lee los parametros de Pulsos por Kilometro para poder   
*					Calcular la distancia y velocidad que lee la moduladora   
*					la moduladora
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true comunicacion OK
*					false fallo comunicacion 
******************************************************************************/	 
char ModLectPulKm(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Local1;
	char Error;
	
	SenId = ID;
	Id = ModDirId(SenId);		
	Proceso.B.fConfPer = true;
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	Error = 0;
	do{
		
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw1_RdReg(Id,0x80,&Moduladora[SenId].PKm);
		//	Proceso.B.fConfPer = false;
		//	return Local1;
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw2_RdReg(Id,0x80,&Moduladora[SenId].PKm);
		//	Proceso.B.fConfPer = false;
		//	return 	Local1;
		}
		if (Local1)
			break;
		else
			Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	return Local1;
	}

/******************************************************************************
*	Funcion:		ModConfPulKm()
*	Descricpion:	Graba los parametros de 
*						Dientes Traccion
*						Dientes Distribucion
*						Relacion de Transmicion
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true comunicacion OK
*					false fallo comunicacion 
*******************************************************************************/	 
char ModConfParam(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Local1;
	char Error= 0;
	
	SenId = ID;
	Id = ModDirId(SenId);		
	Proceso.B.fConfPer = true;
	
//	while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();
	
	LocUserDW00.B.V[0] = Moduladora[SenId].PVT;
	LocUserDW00.B.V[1] = Moduladora[SenId].PVD;
	LocUserDW00.UI.V[1] = Moduladora[SenId].RDT;
	Error = 0;
	do{	
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw1_WrReg(Id,0x81,&LocUserDW00.UL.V);
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			
			Local1 = Sw2_WrReg(Id,0x81,&LocUserDW00.UL.V);
		}
		if (Local1)
			break;
		else
			Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	return Local1;
}


/******************************************************************************
*	Funcion:		ModLectPulKm()
*	Descricpion:	Lee los parametros de   
*						Dientes Traccion
*						Dientes Distribucion
*						Relacion de Transmicion
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true comunicacion OK
*					false fallo comunicacion 
******************************************************************************/	 
char ModLectParam(unsigned char ID)
{
	union _UInt32 LocUserDW00;
	unsigned char SenId;
	unsigned char Id;
	char Error;
	
	SenId = ID;
	Id = ModDirId(SenId);	
	Proceso.B.fConfPer = true;
	Error = 0;
	do{
		if(!Moduladora[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
				
			if(Sw1_RdReg(Id,0x81,&LocUserDW00.UL.V))
			{
			//	Proceso.B.fConfPer = false;
			//	return false;
				break;
			}
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x81,&LocUserDW00.UL.V))
			{
			//	Proceso.B.fConfPer = false;
			//	return false;
				break;
			}		
		}

		Error++;
	}while(Error < 3);
	Proceso.B.fConfPer = false;
	if(Error >= 3)
		return false;
				
	if(	Moduladora[SenId].PVT != LocUserDW00.B.V[0] ||
		Moduladora[SenId].PVD != LocUserDW00.B.V[1] ||
		Moduladora[SenId].RDT != LocUserDW00.UI.V[1])
	{
		Moduladora[SenId].PVT = LocUserDW00.B.V[0];
		Moduladora[SenId].PVD = LocUserDW00.B.V[1];
		Moduladora[SenId].RDT = LocUserDW00.UI.V[1];
		return 2;
	}

	Moduladora[SenId].PVT = LocUserDW00.B.V[0];
	Moduladora[SenId].PVD = LocUserDW00.B.V[1];
	Moduladora[SenId].RDT = LocUserDW00.UI.V[1];

	Proceso.B.fConfPer = false;
	return true;
}

/******************************************************************************
*	Funcion:		ModSistKpKd()
*	Descricpion:	Escribe y lee los SP y KD de una moduladora y si lo   
*						escribio y leyo sin errores devuelve un True sino False
*						Dientes Distribucion
*						Relacion de Transmicion
*	Ingreso Datos:	ID del sensor
*					SP set point requerido
*					KD Constante de la placa
*					Esc flag para determinar si tiene que escribir datos o no
*	Salida Datos:	true lectura/escritura OK
*					false fallo en la lectura escritura 
******************************************************************************/	 
char ModSistKpKd(unsigned char ID,unsigned int SP, unsigned int KD, unsigned char Esc)
{
	unsigned char SenId;
	unsigned int i;
	unsigned int error;
	
	SenId = ID;
	error = 0;
	if(Esc)
	{
		Moduladora[SenId].SP = SP;
		Moduladora[SenId].KD = KD;

		for(i=0;i<3;i++)
		{
			if(ModAjtSPKD0(SenId))
			{
				error = 0;
				break;
			}
			error++;
		}
		if(error)
			return false;
	}		
		
	Moduladora[SenId].SP = 0;
	Moduladora[SenId].KD = 0;
	
	for(i=0;i<3;i++)
	{
		if(ModLecSPKD0(SenId))
		{
			error = 0;
			break;
		}
		error++;				
	}
	if (error)
		return false;
	else if(Esc)
	{
		if((Moduladora[SenId].SP == SP)&&(Moduladora[SenId].KD == KD))
			return true;
	}
	else
		return false;
}