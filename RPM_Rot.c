  	#include	"VarGlob.h"
  	#include	"Constantes.h"
  	#include 	"sw1master.h"
  	#include 	"sw2master.h"
  	#include 	"ExeTask.h"
  	
  	
  	
/******************************************************************************
*	Funcion:		RotStart000()
*	Descricpion:	Escanea los dos buses en busqueda de sensores de Rotacion
*					Realiza la lectura de un dispositivo en los dos buses al
*					mismo tiempo
*					Realiza la lectura del factor K 
*					Configura el bit que indica en que bus esta conectado
*					Configura el bit que alguna vez se conecto
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	   	
void RotStart000(void)
{
	unsigned char Id;
	unsigned char SenId;
	
	for(SenId=0;SenId<8;SenId++)
	{
		Id = 0x40 + SenId;
		
		SW1_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW1_PortUserWr(0x80);		//REG.ID
		
		SW2_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW2_PortUserWr(0x80);		//REG.ID
		
		SW1PortUser.Sts.B.fPend = true;
		SW2PortUser.Sts.B.fPend = true;
		
		while(SW1PortUser.Sts.B.fPend ||SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Rotacion[SenId].FK	=*(unsigned long *)SW1PortUser.Data;
			Rotacion[SenId].Sts.B.Con = true;
			Rotacion[SenId].Sts.B.Bus = false;
//			Rotacion[SenId].Sts.B.Det = true;
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Rotacion[SenId].FK	=*(unsigned long *)SW2PortUser.Data;
			Rotacion[SenId].Sts.B.Con = true;
			Rotacion[SenId].Sts.B.Bus = true;
//			Rotacion[SenId].Sts.B.Det = true;
		}
		else
		{
			Rotacion[SenId].FK	= 0;
			Rotacion[SenId].Sts.B.Con = false;
//			Rotacion[SenId].Sts.B.Bus = false;
//			Rotacion[SenId].Sts.B.Det = false;
		}
	}
	GrabaConfROT();
}

/******************************************************************************
*	Funcion:		RotLectura00()
*	Descricpion:	Le el numero de dientes por vuelta 
*	Ingreso Datos:	Id del sensor
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char RotLecPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	unsigned long Factork;
	char Error;
	
	SenId = ID;
	Id = 0x40 + SenId;
	Error = 0;
	do{
		if(!Rotacion[SenId].Sts.B.Bus)
		{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_RdReg(Id,0x80,&Factork))
			{
				Error = 0;
				break;
			}	
		}
		else
		{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x80,&Factork))
			{
				Error = 0;
				break;
			}
		}
		Error++;
	}while(Error<3);
	if(Error>=3)
		return false;
		
	if(Rotacion[SenId].FK != Factork)
	{
		Rotacion[SenId].FK = Factork;
		return 2;
	}
	Rotacion[SenId].FK = Factork;
	return true;
}

/******************************************************************************
*	Funcion:		RotEscritura00()
*	Descricpion:	Realiza la lectura de las Niveles de alarmas y factor 
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char RotEscPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	char Error;
	SenId = ID;
	Id = 0x40 + SenId;
	Error = 0;
	do{
		if(!Rotacion[SenId].Sts.B.Bus)
		{
			if(Sw1_WrReg(Id,0x80,&Rotacion[SenId].FK))
			{
				Error = 0;
				break;
			}
		}
		else
		{
			if(Sw2_WrReg(Id,0x80,&Rotacion[SenId].FK))
			{
				Error = 0;
				break;
			}
		}
		Error ++;
	}while(Error<3);
	
	if(Error>=3)	
		return false;
	else
		return true;
}
/******************************************************************************
*	Funcion:		TRBStart000()
*	Descricpion:	Escanea ambos buses en busqueda de sensores de Turbina
*					Realiza la lectura de un dispositivo en los dos buses al
*					mismo tiempo
*					Realiza la lectura del factor K 
*					Configura el bit que indica en que bus esta conectado
*					Configura el bit que alguna vez se conect
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
void TRBStart000(void)
{
	unsigned char Id;
//	unsigned int Medicion;
	unsigned char SenId;
	union _UInt32 LocUserDW00;
	union _UInt32 LocUserDW01;
	for(SenId=0;SenId<3;SenId++)
	{
		Id = 0xD3 + SenId;
		
		SW1_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW1_PortUserWr(0x80);		//REG.ID
		
		SW2_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW2_PortUserWr(0x80);		//REG.ID
		
		SW1PortUser.Sts.B.fPend = true;
		SW2PortUser.Sts.B.fPend = true;
		
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Turbina[SenId].FK = *(unsigned long *)SW1PortUser.Data;
			Turbina[SenId].Sts.B.Con = true;
			Turbina[SenId].Sts.B.Bus = false;
//			Turbina[SenId].Sts.B.Det = true;
			
			Sw1_RdReg(Id,0x06,&LocUserDW00.UL.V);
			
			Sw1_RdReg(Id,0x07,&LocUserDW01.UL.V);
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Turbina[SenId].FK = *(unsigned long *)SW2PortUser.Data;
			Turbina[SenId].Sts.B.Con = true;
			Turbina[SenId].Sts.B.Bus = true;
//			Turbina[SenId].Sts.B.Det = true;
			
			Sw2_RdReg(Id,0x06,&LocUserDW00.UL.V);
			
			Sw2_RdReg(Id,0x07,&LocUserDW01.UL.V);
		}
		else
		{
			Turbina[SenId].Sts.B.Con = false;
		//	Turbina[SenId].Sts.B.Bus = false;
		//	Turbina[SenId].Sts.B.Det = false;
		}
	Turbina[SenId].AlMin = LocUserDW00.UI.V[0];
	Turbina[SenId].TmMin = LocUserDW00.UI.V[1];
	Turbina[SenId].AlMax = LocUserDW01.UI.V[0];
	Turbina[SenId].TmMax = LocUserDW01.UI.V[1];
	}
	GrabaConfTRB();
}

/******************************************************************************
*	Funcion:		TRBLecPar00()
*	Descricpion:	Realiza la lectura de las Niveles de alarmas y factor 
*	Ingreso Datos:	ID del dispositivo
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/
char TRBLecPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	union _UInt32 LocUserDW00;
	union _UInt32 LocUserDW01;
	char Error;
	
	SenId = ID;
	Id = 0xD3 + SenId;
	Error = 0;
	
	if(!Turbina[SenId].Sts.B.Bus)
	{
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_RdReg(Id,0x80,&Turbina[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();	
			if(Sw1_RdReg(Id,0x06,&LocUserDW00.UL.V ))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();	
			if(Sw1_RdReg(Id,0x07,&LocUserDW01.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;

	}
	else
	{
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x80,&Turbina[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();		
			if(Sw2_RdReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x07,&LocUserDW01.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
	}
	if( Turbina[SenId].AlMin != LocUserDW00.UI.V[0] ||
		Turbina[SenId].TmMin != LocUserDW00.UI.V[1] ||
		Turbina[SenId].AlMax != LocUserDW01.UI.V[0] ||
		Turbina[SenId].TmMax != LocUserDW01.UI.V[1])
	{
		Turbina[SenId].AlMin = LocUserDW00.UI.V[0];
		Turbina[SenId].TmMin = LocUserDW00.UI.V[1];
		Turbina[SenId].AlMax = LocUserDW01.UI.V[0];
		Turbina[SenId].TmMax = LocUserDW01.UI.V[1];
		return 2;
	}
	
	Turbina[SenId].AlMin = LocUserDW00.UI.V[0];
	Turbina[SenId].TmMin = LocUserDW00.UI.V[1];
	Turbina[SenId].AlMax = LocUserDW01.UI.V[0];
	Turbina[SenId].TmMax = LocUserDW01.UI.V[1];
	return true;
}

/******************************************************************************
*	Funcion:		TRBEscPar00()
*	Descricpion:	Realiza la escritura de las Niveles de alarmas y factor 
*	Ingreso Datos:	ID del dispositivo
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char TRBEscPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	union _UInt32 LocUserDW00;
	char Error;
	
	SenId = ID;
	Id = 0xD3 + SenId;
	Turbina[SenId].TmMin = kTempAlTRB;
	Turbina[SenId].TmMax = kTempAlTRB;	
	if(!Turbina[SenId].Sts.B.Bus)
	{
		
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x80,&Turbina[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
		LocUserDW00.UI.V[0] = Turbina[SenId].AlMin;
		LocUserDW00.UI.V[1] = Turbina[SenId].TmMin;
		
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
			
		LocUserDW00.UI.V[0] = Turbina[SenId].AlMax;
		LocUserDW00.UI.V[1] = Turbina[SenId].TmMax;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x07,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
	}
	else
	{
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x80,&Turbina[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
		LocUserDW00.UI.V[0] = Turbina[SenId].AlMin;
		LocUserDW00.UI.V[1] = Turbina[SenId].TmMin;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
			
		LocUserDW00.UI.V[0] = Turbina[SenId].AlMax;
		LocUserDW00.UI.V[1] = Turbina[SenId].TmMax;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x07,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
	}
	return true;
}


/******************************************************************************
*	Funcion:		TRBChekAl00()
*	Descricpion:	Controla si estan cargado los valores de alarma si no 
*					retorna sin realizar ningun calculo
*					Se activa la alarma con el valor seteado y se desactiva 
*					pasado el tiempo de activacio y desactivacion de la alarma
*					Ejecutar esta funcion cada 1 segundo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					false (falso) si no estan seteadas las alarmas.
******************************************************************************/	 
char TRBChekAl00(unsigned char ID)
{
	unsigned char SenId;
	unsigned int Max, AuxMax;
	unsigned int Min, AuxMin;

	SenId = ID;	
	if(!Turbina[SenId].AlMax || !Turbina[SenId].TmMax || !Turbina[SenId].AlMin || !Turbina[SenId].TmMin)
	{
		Turbina[SenId].Sts.B.FMin = false;
		Turbina[SenId].Sts.B.FMax = false;
		Turbina[SenId].TMinAux = 0;
		Turbina[SenId].TMaxAux = 0;
		return false;
	}
//Si no Mido RPM		
	if(!Turbina[SenId].Med)
	{
		Turbina[SenId].Sts.B.FMin = false;
		Turbina[SenId].Sts.B.FMax = false;
		Turbina[SenId].TMaxAux = 0;
		Turbina[SenId].TMinAux = 0;
	}
//Si el valor medido es mayor al valor de alarma maximo, y la alarma no esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if((Turbina[SenId].Med > Turbina[SenId].AlMax) && !Turbina[SenId].Sts.B.FMax)
	{
		Turbina[SenId].TMaxAux++;
		if(Turbina[SenId].TMaxAux >= Turbina[SenId].TmMax)
		{
			Turbina[SenId].Sts.B.FMin = false;
			Turbina[SenId].Sts.B.FMax = true;
			Turbina[SenId].TMaxAux = 0;
		}
	}
//Si el valor medido es menor al valor de alarma maximo, y la alarma esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if((Turbina[SenId].Med <= Turbina[SenId].AlMax) && Turbina[SenId].Sts.B.FMax)
	{
		Turbina[SenId].TMaxAux++;
		if(Turbina[SenId].TMaxAux >= Turbina[SenId].TmMax)
		{
			Turbina[SenId].Sts.B.FMax = false;
			Turbina[SenId].TMaxAux = 0;
		}
	}
//Si el valor medido es menor al valor de alarma minimo, y la alarma no esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if ((Turbina[SenId].Med < Turbina[SenId].AlMin) && !Turbina[SenId].Sts.B.FMin)
	{
		Turbina[SenId].TMinAux++;
		if(Turbina[SenId].TMinAux >= Turbina[SenId].TmMin)
		{
			Turbina[SenId].Sts.B.FMin = true;
			Turbina[SenId].Sts.B.FMax = false;
		}
	}
//Si el valor medido es mayor al valor de alarma minimo, y la alarma esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if ((Turbina[SenId].Med >= Turbina[SenId].AlMin) && Turbina[SenId].Sts.B.FMin)
	{
		Turbina[SenId].TMinAux++;
		if(Turbina[SenId].TMinAux >= Turbina[SenId].TmMin)
		{
			Turbina[SenId].Sts.B.FMin = false;
			Turbina[SenId].TMinAux = 0;
		}
	}
//Si el valor medido no esta en alarma reseteo los bits y los temporizadores	
	else 
	{
		Turbina[SenId].TMinAux = 0;
		Turbina[SenId].TMaxAux = 0;
	}
	return true;
}
/******************************************************************************
*	Funcion:		PREStart000()
*	Descricpion:	Escanea ambos buses en busqueda de sensores de Presion
*					Realiza la lectura de un dispositivo en los dos buses al
*					mismo tiempo
*					Realiza la lectura del factor K 
*					Configura el bit que indica en que bus esta conectado
*					Configura el bit que alguna vez se conect
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	 
void PREStart000(void)
{
	unsigned char Id;
	unsigned char SenId;
	union _UInt32 LocUserDW00;
	union _UInt32 LocUserDW01;
	
	for(SenId=0;SenId<9;SenId++)
	{
		Id = 0x80 + SenId;
		
		SW1_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW1_PortUserWr(0x80);		//REG.ID
		
		SW2_PortUserStart(Id,0x06 | SW1_cmdRd,4);
		SW2_PortUserWr(0x80);		//REG.ID
		
		SW1PortUser.Sts.B.fPend = true;
		SW2PortUser.Sts.B.fPend = true;
		
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Presion[SenId].FK = *(unsigned long *)SW1PortUser.Data;
			Presion[SenId].Sts.B.Con = true;
			Presion[SenId].Sts.B.Bus = false;

			
			Sw1_RdReg(Id,0x06,&LocUserDW00.UL.V);
			
			Sw1_RdReg(Id,0x07,&LocUserDW01.UL.V);
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Presion[SenId].FK = *(unsigned long *)SW2PortUser.Data;
			Presion[SenId].Sts.B.Con = true;
			Presion[SenId].Sts.B.Bus = true;
			
			Sw2_RdReg(Id,0x06,&LocUserDW00.UL.V);
			
			Sw2_RdReg(Id,0x07,&LocUserDW01.UL.V);
		}
		else
		{
			Presion[SenId].Sts.B.Con = false;
		}
		Presion[SenId].AlMin = LocUserDW00.UI.V[0];
		Presion[SenId].TmMin = LocUserDW00.UI.V[1];
		Presion[SenId].AlMax = LocUserDW01.UI.V[0];
		Presion[SenId].TmMax = LocUserDW01.UI.V[1];
	}
	GrabaConfPRE();
}
/******************************************************************************
*	Funcion:		PRESLecPar00(ID)
*	Descricpion:	Realiza la lectura de las Niveles de alarmas y factor 
*	Ingreso Datos:	ID del dispositivo a leer
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/
char PRELecPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	union _UInt32 LocUserDW00;
	union _UInt32 LocUserDW01;
	char Error;
	
	SenId = ID;
	Id = 0x80 + SenId;
	Error = 0;
	
	if(!Presion[SenId].Sts.B.Bus)
	{
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_RdReg(Id,0x80,&Presion[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();	
			if(Sw1_RdReg(Id,0x06,&LocUserDW00.UL.V ))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();	
			if(Sw1_RdReg(Id,0x07,&LocUserDW01.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;

	}
	else
	{
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x80,&Presion[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();		
			if(Sw2_RdReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
			
		Error = 0;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_RdReg(Id,0x07,&LocUserDW01.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)
			return false;
	}
	Presion[SenId].AlMin = LocUserDW00.UI.V[0];
	Presion[SenId].TmMin = LocUserDW00.UI.V[1];
	Presion[SenId].AlMax = LocUserDW01.UI.V[0];
	Presion[SenId].TmMax = LocUserDW01.UI.V[1];
	return true;
}

/******************************************************************************
*	Funcion:		PREEscPar00()
*	Descricpion:	Realiza la escritura de las Niveles de alarmas y factor 
*	Ingreso Datos:	ID del dispositivo
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
char PREEscPar00(unsigned char ID)
{
	unsigned char SenId;
	unsigned char Id;
	union _UInt32 LocUserDW00;
	char Error;
	
	SenId = ID;
	Id = 0x80 + SenId;
	Presion[SenId].TmMin = kTempAlPRE;
	Presion[SenId].TmMax = kTempAlPRE; 	
	if(!Presion[SenId].Sts.B.Bus)
	{
		
		Error = 0;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x80,&Presion[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
		LocUserDW00.UI.V[0] = Presion[SenId].AlMin;
		LocUserDW00.UI.V[1] = Presion[SenId].TmMin;
		
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
			
		LocUserDW00.UI.V[0] = Presion[SenId].AlMax;
		LocUserDW00.UI.V[1] = Presion[SenId].TmMax;
		do{
			while(SW1PortUser.Sts.B.fPend) ExeTask();
			if(Sw1_WrReg(Id,0x07,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
	}
	else
	{
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x80,&Presion[SenId].FK))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
		LocUserDW00.UI.V[0] = Presion[SenId].AlMin;
		LocUserDW00.UI.V[1] = Presion[SenId].TmMin;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x06,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
			
		LocUserDW00.UI.V[0] = Presion[SenId].AlMax;
		LocUserDW00.UI.V[1] = Presion[SenId].TmMax;
		do{
			while(SW2PortUser.Sts.B.fPend) ExeTask();
			if(Sw2_WrReg(Id,0x07,&LocUserDW00.UL.V))
			{
				Error = 0;
				break;
			}
			Error++;
		}while(Error<3);
		if(Error>=3)	
			return false;
		Error = 0;
		
	}
	return true;
}



/******************************************************************************
*	Funcion:		PREChekAl00()
*	Descricpion:	Controla si estan cargado los valores de alarma si no 
*					retorna sin realizar ningun calculo
*					Se activa la alarma con el valor seteado y se desactiva 
*					pasado el tiempo de activacio y desactivacion de la alarma
*					Ejecutar esta funcion cada 1 segundo
*	Ingreso Datos:	ID del sensor a controlar
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					false (falso) si no estan seteadas las alarmas.
******************************************************************************/	 
char PREChekAl00(unsigned char ID)
{
	unsigned char SenId;
	unsigned int Max, AuxMax;
	unsigned int Min, AuxMin;

	SenId = ID;	
	if(!Presion[SenId].AlMax || !Presion[SenId].AlMin || !Presion[SenId].Med)
	{
		Presion[SenId].Sts.B.FMin = false;
		Presion[SenId].Sts.B.FMax = false;
		Presion[SenId].TMinAux = 0;
		Presion[SenId].TMaxAux = 0;
	}	
//Si el valor medido es mayor al valor de alarma maximo, y la alarma no esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if((Presion[SenId].Med > Presion[SenId].AlMax) && !Presion[SenId].Sts.B.FMax)
	{
		Presion[SenId].TMaxAux++;
		if(Presion[SenId].TMaxAux >= Presion[SenId].TmMax)
		{
			Presion[SenId].Sts.B.FMin = false;
			Presion[SenId].Sts.B.FMax = true;
			Presion[SenId].TMaxAux = 0;
		}
	}
//Si el valor medido es menor al valor de alarma maximo, y la alarma esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if((Presion[SenId].Med <= Presion[SenId].AlMax) && Presion[SenId].Sts.B.FMax)
	{
		Presion[SenId].TMaxAux++;
		if(Presion[SenId].TMaxAux >= Presion[SenId].TmMax)
		{
			Presion[SenId].Sts.B.FMax = false;
			Presion[SenId].TMaxAux = 0;
		}
	}
//Si el valor medido es menor al valor de alarma minimo, y la alarma no esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if ((Presion[SenId].Med < Presion[SenId].AlMin) && !Presion[SenId].Sts.B.FMin)
	{
		Presion[SenId].TMinAux++;
		if(Presion[SenId].TMinAux >= Presion[SenId].TmMin)
		{
			Presion[SenId].Sts.B.FMin = true;
			Presion[SenId].Sts.B.FMax = false;
		}
	}
//Si el valor medido es mayor al valor de alarma minimo, y la alarma esta activa
//realizo el conteo y despues desactivo la alarma y reseteo el timer
	else if ((Presion[SenId].Med >= Presion[SenId].AlMin) && Presion[SenId].Sts.B.FMin)
	{
		Presion[SenId].TMinAux++;
		if(Presion[SenId].TMinAux >= Presion[SenId].TmMin)
		{
			Presion[SenId].Sts.B.FMin = false;
			Presion[SenId].TMinAux = 0;
		}
	}
//Si el valor medido no esta en alarma reseteo los bits y los temporizadores	
	else 
	{
		Presion[SenId].TMinAux = 0;
		Presion[SenId].TMaxAux = 0;
	}
	return true;
}



/******************************************************************************
*	Funcion:		TOLStart000()
*	Descricpion:	Escanea ambos buses en busqueda de sensores de RPM
*					Realiza la lectura de un dispositivo en los dos buses al
*					mismo tiempo
*					Realiza la lectura del factor K 
*					Configura el bit que indica en que bus esta conectado
*					Configura el bit que alguna vez se conect
*	Ingreso Datos:	Ninguno
*	Salida Datos:	true (verdadero) si la operacio fue correcta
*					falso si hubo algun fallo en la comunicacion
******************************************************************************/	 
void TOLStart000(void)
{
	unsigned char Id;
	unsigned char SenId;
	
	for(SenId=0;SenId<16;SenId++)
	{
		Id = 0x48 + SenId;		
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();
		
		SW1_PortUserStart(Id,0x00 | SW1_cmdRd,1);
		SW1_PortUserSend(false);
		
		SW2_PortUserStart(Id,0x00 | SW2_cmdRd,1);
		SW2_PortUserSend(false);
		
		SW1PortUser.Sts.B.fPend = true;
		SW2PortUser.Sts.B.fPend = true;
		
		while(SW1PortUser.Sts.B.fPend || SW2PortUser.Sts.B.fPend) ExeTask();	
		
		if (SW1PortUser.Sts.B.fOk)
		{
			Tolva[SenId].Sts.B.Con = true;
			Tolva[SenId].Sts.B.Bus = false;
//			Tolva[SenId].Sts.B.Det = true;
			
		}
		else if (SW2PortUser.Sts.B.fOk)
		{
			Tolva[SenId].Sts.B.Con = true;
			Tolva[SenId].Sts.B.Bus = true;
//			Tolva[SenId].Sts.B.Det = true;			
		}
		else
		{
			Tolva[SenId].Sts.B.Con = false;
		//	Turbina[SenId].Sts.B.Bus = false;
		//	Turbina[SenId].Sts.B.Det = false;
		}
	}
	GrabaConfTol();
}
