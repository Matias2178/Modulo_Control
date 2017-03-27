/*****************************************************************************
 *	FUNCIONES DE MANEJO DE DATOS DE LOS SENSORES SE SEMILLA Y FERTILIZANTE?
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 
 *	FECHA		:	2013
 *	
 *****************************************************************************/
 	#include	"Constantes.h"
 	#include	"VarGlob.h"
 	#include 	"GPS.h"
 	#include 	"ES_Conf.h"
 	#include 	"Perifericos.h"
 	#include	"sw1master.h"
 	#include	"sw2master.h"
 
 
/******************************************************************************
*	Funcion:		Med_Sen_Bus1()
*	Descricpion:	Procesa los datos se semillas del sensor leido en el bus 1
*					y los almacena en estructuras para su posterior uso
*					tambien se generan varios bits de estados
*	Ingreso Datos:	ID del sensor
*					Medicion leida del sensor
*	Salida Datos:	Ninguno
******************************************************************************/
 void Med_Sen_Bus1(unsigned char ID,unsigned int Med)
 {
	unsigned char mID;
	unsigned char tMed;
	unsigned char MedId;		//Sensor activo y cantidad de mediciones
	unsigned int MedValue;		//Cantidad se semillas leidas
	unsigned long SemSeg, SemMts;
	double	Factor;
	
	MedValue=((unsigned int)Med >> 12);
	MedId=MedValue & 0x000F;
	MedValue=Med & 0x0FFF;
	mID = ID;	
	if (mID <= 0x1F)
	{
		//SENSORES DE SIEMBRA
		if(MedId & 0x08)
		{
			
//SENSOR ACTIVO
//Activar configuracion para fina y gruesa
//y hacer lo mismo para fertilizante y para el bus2
//Falta la parte de correccion por alta densidad
			tMed = MedId & 0x07;
			if(BUS1.Sie[mID].tMed != tMed)
			{
				BUS1.Sie[mID].tMed = tMed;
				Sensores.STS.B.fSAct1 = true;
			}
			else 
				return;
			if(!MedValue)
			{
				if(!BUS1.Sie[mID].Sts.B.AuxTT)
				{
					BUS1.Sie[mID].Sts.B.AuxTT = true;
					return;
				}
			}
			else
			{
				BUS1.Sie[mID].Sts.B.AuxTT = false;
			}
			MedValue = MedValue * 100;
//-------------------------------------------------
//Modo de Siembra en Gruesa		
//-------------------------------------------------
			if(BUS1.Sie[mID].Sts.B.Mod)
			{
				
				if(MedValue && VSiembra)
				{	
				//	Factor = ADQ_AjAltaDensFact((unsigned long) MedValue,k_GruesaTM);
				//	SemSeg = (unsigned long)ADQ_AjAltaDensValue((unsigned long) MedValue,Factor);
					SemSeg = (unsigned long) MedValue / (unsigned long) k_GruesaTM;
					SemMts = (SemSeg * 36) / (unsigned long) VSiembra;
				//	BUS1.Sie[mID].Med = (int) SemMts;
					BUS1.Sie[mID].Sts.B.Act = true;
					MedValue = (int) SemMts;
					if(!MedValue && !BUS1.Sie[mID].Sts.B.AuxTT3)
					{
						BUS1.Sie[mID].Sts.B.AuxTT3 = true;
					}
					else if(!MedValue && BUS1.Sie[mID].Sts.B.AuxTT3)
					{
						BUS1.Sie[mID].Med = MedValue;
					}
					else
					{
						BUS1.Sie[mID].Med = MedValue;
						BUS1.Sie[mID].Sts.B.AuxTT3 = false;
					}		
				}
				else
				{
					BUS1.Sie[mID].Med = 0;
					BUS1.Sie[mID].Sts.B.Act = false;
				}
			}
//-------------------------------------------------
//Modo de siembra Fina
//-------------------------------------------------
			else if(VSiembra)
			{
				SemSeg = (unsigned long) MedValue / (unsigned long) k_FinaTM;
		//Promedio las mediciones en fina
				SemMts	= (unsigned long) BUS1.Sie[mID].AxMed;
				BUS1.Sie[mID].AxMed =  (int) SemSeg;
				SemSeg = (SemSeg + SemMts)/ 2 ;
				BUS1.Sie[mID].Med = (int) SemSeg;
				BUS1.Sie[mID].Sts.B.Act = true; 
			}
			else
			{
				BUS1.Sie[mID].Med = 0;
				BUS1.Sie[mID].Sts.B.Act = false;	
			}
			BUS1.Sie[mID].Sts.B.AuxTT2 = false;
		}
		else
		{
			//SENSOR INACTIVO
			 if(!BUS1.Sie[mID].Sts.B.AuxTT2)
			{
				BUS1.Sie[mID].Sts.B.AuxTT2 = true;
				return;
			}
			BUS1.Sie[mID].Med = 0;
			BUS1.Sie[mID].Sts.B.Act = false;
			BUS1.Sie[mID].Sts.B.AuxTT3 = false;
		}
	}
	else if (mID <= 0x3F)
	{
		//SENSORES DE FERTILIZANTE
		mID -= 0x20;
		if(MedId & 0x08) 
		{	
//SENSOR ACTIVO
//Activar configuracion para fina y gruesa
//y hacer lo mismo para fertilizante y para el bus2
//Falta la parte de correccion por alta densidad
			tMed = MedId & 0x07;
			if(BUS1.Fer[mID].tMed != tMed)
			{
				BUS1.Fer[mID].tMed = tMed;
				Sensores.STS.B.fFAct1 = true;
			}
			else
				return;
			if(!MedValue)
			{
				if(!BUS1.Fer[mID].Sts.B.AuxTT)
				{
					BUS1.Fer[mID].Sts.B.AuxTT = true;
					return;
				}
			}
			else
			{
				BUS1.Fer[mID].Sts.B.AuxTT = false;
			}
//-------------------------------------------------
//Modo de Siembra en Gruesa		
//-------------------------------------------------
			MedValue = MedValue * 100;	
			if(BUS1.Fer[mID].Sts.B.Mod)
			{
				if(MedValue && VSiembra)
				{
			//		Factor = ADQ_AjAltaDensFact((unsigned long) MedValue,k_GruesaTM);
			//		SemSeg = (unsigned long)ADQ_AjAltaDensValue((unsigned long) MedValue,Factor);
					SemSeg = (unsigned long) MedValue / (unsigned long) k_GruesaTM;
					SemMts = (SemSeg * 36) / (unsigned long) VSiembra;
			//		BUS1.Fer[mID].Med = (int) SemMts;
					BUS1.Fer[mID].Sts.B.Act = true; 
					MedValue = (int) SemMts;
					if(!MedValue && !BUS1.Fer[mID].Sts.B.AuxTT3)
					{
						BUS1.Fer[mID].Sts.B.AuxTT3 = true;
					}
					else if(!MedValue && BUS1.Fer[mID].Sts.B.AuxTT3)
					{
						BUS1.Fer[mID].Med = MedValue;
					}
					else
					{
						BUS1.Fer[mID].Med = MedValue;
						BUS1.Fer[mID].Sts.B.AuxTT3 = false;
					}
				}
				else
				{
					BUS1.Fer[mID].Med = 0;
					BUS1.Fer[mID].Sts.B.Act = false;
				}
			}
//-------------------------------------------------
//Modo de siembra Fina
//-------------------------------------------------
			else if(VSiembra)
			{
				SemSeg = (unsigned long) MedValue / (unsigned long) k_FinaTM;
				
			//Promedio las mediciones en fina
				SemMts	= (unsigned long) BUS1.Fer[mID].AxMed;
				BUS1.Fer[mID].AxMed =  (int) SemSeg;
				SemSeg = (SemSeg + SemMts)/ 2 ;
				
				BUS1.Fer[mID].Med = (int) SemSeg;
				BUS1.Fer[mID].Sts.B.Act = true; 
			}
			else
			{
				BUS1.Sie[mID].Med = 0;
				BUS1.Sie[mID].Sts.B.Act = false;
			}
			BUS1.Fer[mID].Sts.B.AuxTT2 = false;
		}
		else
		{
			//SENSOR INACTIVO
			if(!BUS1.Fer[mID].Sts.B.AuxTT2)
			{
				BUS1.Fer[mID].Sts.B.AuxTT2 = true;
				return;
			}
			BUS1.Fer[mID].Med = 0;
			BUS1.Fer[mID].Sts.B.Act = false;
			BUS1.Fer[mID].Sts.B.AuxTT3 = false;	
		}
	}
}


/******************************************************************************
*	Funcion:		Bus1aDtsCom()
*	Descricpion:	Transfiere los datos del Bus2 LIN a bus de comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void Bus1aDtsCom(void)
{
	int i;
	for(i=0;i<=31;i++)
	{
		//SENSORES DE SIEMBRA
		DtsComBus1.Sie.Med[i] = BUS1.Sie[i].Med;
		DtsComBus1.Sie.Sts[i] = BUS1.Sie[i].Sts.C.Val;
		
//		if(BUS1.Sie[i].Sts.B.Con && BUS1.Sie[i].Med)
//		{
//			Medicion ++;
//		}
		
		//SENSORES DE FERTILIZANTE
		DtsComBus1.Fer.Med[i] = BUS1.Fer[i].Med;
		DtsComBus1.Fer.Sts[i] = BUS1.Fer[i].Sts.C.Val;
		
//		if(BUS1.Fer[i].Sts.B.Con && BUS1.Fer[i].Med)
//		{
//			Medicion ++;
//		}		
	}
	
}		

//-----------------------------------------------------------------------------
//	FUNCIONES PARA EL BUS 2
/******************************************************************************
*	Funcion:		Med_Sen_Bus2()
*	Descricpion:	Procesa los datos se semillas del sensor leido en el bus 2
*					y los almacena en estructuras para su posterior uso
*					tambien se generan varios bits de estados
*	Ingreso Datos:	ID del sensor
*					Medicion leida del sensor
*	Salida Datos:	Ninguno
******************************************************************************/
 void Med_Sen_Bus2(unsigned char ID,unsigned int Med)
 {
	unsigned char mID;
	unsigned char tMed;
	unsigned char MedId;		//Sensor activo y cantidad de mediciones
	unsigned int MedValue;		//Cantidad se semillas leidas
	unsigned long SemSeg, SemMts;
	double	Factor;
	
	MedValue=((unsigned int)Med >> 12);
	MedId=MedValue & 0x000F;
	MedValue=Med & 0x0FFF;
	mID = ID;	
	if (mID <= 0x1F)
	{
		//SENSORES DE SIEMBRA
		if(MedId & 0x08)
		{

//SENSOR ACTIVO
//Activar configuracion para fina y gruesa
//y hacer lo mismo para fertilizante y para el bus2
//Falta la parte de correccion por alta densidad
			tMed = MedId & 0x07;
			if(BUS2.Sie[mID].tMed != tMed)
			{
				BUS2.Sie[mID].tMed = tMed;
				Sensores.STS.B.fSAct2 = true;
			}
			else
				return;
			if(!MedValue)
			{
				if(!BUS2.Sie[mID].Sts.B.AuxTT)
				{
					BUS2.Sie[mID].Sts.B.AuxTT = true;
					return;
				}
			}
			else
			{
				BUS2.Sie[mID].Sts.B.AuxTT = false;
				BUS2.Sie[mID].Sts.B.AuxTT2 = false;
				BUS2.Sie[mID].Sts.B.AuxTT3 = false;
			}
			MedValue = MedValue * 100;		
//-------------------------------------------------
//Modo de Siembra en Gruesa		
//-------------------------------------------------
			if(BUS2.Sie[mID].Sts.B.Mod)
			{
				if(MedValue && VSiembra)
				{	
				//	Factor = ADQ_AjAltaDensFact((unsigned long) MedValue,k_GruesaTM);
				//	SemSeg = (unsigned long)ADQ_AjAltaDensValue((unsigned long) MedValue,Factor);
					SemSeg = (unsigned long) MedValue / (unsigned long) k_GruesaTM;
					SemMts = (SemSeg * 36) / (unsigned long) VSiembra;
			//		BUS2.Sie[mID].Med = (int) SemMts;
					BUS2.Sie[mID].Sts.B.Act = true;
					
					MedValue = (int) SemMts;
					if(!MedValue && !BUS2.Sie[mID].Sts.B.AuxTT3)
					{
						BUS2.Sie[mID].Sts.B.AuxTT3 = true;
					}
					else if(!MedValue && BUS2.Sie[mID].Sts.B.AuxTT3)
					{
						BUS2.Sie[mID].Med = MedValue;
					}
					else
					{
						BUS2.Sie[mID].Med = MedValue;
						BUS2.Sie[mID].Sts.B.AuxTT3 = false;
					}						
				}
				else
				{
					BUS2.Sie[mID].Med = 0;
					BUS2.Sie[mID].Sts.B.Act = false;
				}
			}
//-------------------------------------------------
//Modo de siembra Fina
//-------------------------------------------------
			else if(VSiembra)
			{
				SemSeg = (unsigned long) MedValue / (unsigned long) k_FinaTM;		
			//Promedio las mediciones en fina
				SemMts	= (unsigned long) BUS2.Sie[mID].AxMed;
				BUS2.Sie[mID].AxMed =  (int) SemSeg;
				SemSeg = (SemSeg + SemMts)/ 2 ;
				BUS2.Sie[mID].Med = (int) SemSeg;
				BUS2.Sie[mID].Sts.B.Act = true; 
			}
			else
			{
				BUS2.Sie[mID].Med = 0;
				BUS2.Sie[mID].Sts.B.Act = false;
			}
			BUS2.Sie[mID].Sts.B.AuxTT2 = false;
		}
		else
		{
			//SENSOR INACTIVO
			if(!BUS2.Sie[mID].Sts.B.AuxTT2)
			{
				BUS2.Sie[mID].Sts.B.AuxTT2 = true;
				return;
			}
			BUS2.Sie[mID].Med = 0;
			BUS2.Sie[mID].Sts.B.Act = false;		
			BUS2.Sie[mID].Sts.B.AuxTT3 = false;
		}	
	}
	else if (mID <= 0x3F)
	{
		//SENSORES DE FERTILIZANTE
		mID -= 0x20;
		if(MedId & 0x08)
		{
//SENSOR ACTIVO
//Activar configuracion para fina y gruesa
//y hacer lo mismo para fertilizante y para el bus2
//Falta la parte de correccion por alta densidad
			tMed = MedId & 0x07;
			if(BUS2.Fer[mID].tMed != tMed)
			{
				BUS2.Fer[mID].tMed = tMed;
				Sensores.STS.B.fFAct2 = true;
			}
			else
				return;
			if(!MedValue)
			{
				if(!BUS2.Fer[mID].Sts.B.AuxTT)
				{
					BUS2.Fer[mID].Sts.B.AuxTT = true;
					return;
				}
			}
			else
			{
				BUS2.Fer[mID].Sts.B.AuxTT = false;
				BUS2.Fer[mID].Sts.B.AuxTT2 = false;
				BUS2.Fer[mID].Sts.B.AuxTT3 = false;
			}
//-------------------------------------------------
//Modo de Siembra en Gruesa		
//-------------------------------------------------			
			MedValue = MedValue * 100;
			if(BUS2.Fer[mID].Sts.B.Mod)
			{
				if(MedValue && VSiembra)
				{
				//	Factor = ADQ_AjAltaDensFact((unsigned long) MedValue,k_GruesaTM);
				//	SemSeg = (unsigned long)ADQ_AjAltaDensValue((unsigned long) MedValue,Factor);
					SemSeg = (unsigned long) MedValue / (unsigned long) k_GruesaTM;
					SemMts = (SemSeg * 36) / (unsigned long) VSiembra;
	//				BUS2.Fer[mID].Med = (int) SemMts;
					BUS2.Fer[mID].Sts.B.Act = true; 
					
					MedValue = (int) SemMts;
					if(!MedValue && !BUS2.Fer[mID].Sts.B.AuxTT3)
					{
						BUS2.Fer[mID].Sts.B.AuxTT3 = true;
					}
					else if(!MedValue && BUS2.Fer[mID].Sts.B.AuxTT3)
					{
						BUS2.Fer[mID].Med = MedValue;
					}
					else
					{
						BUS2.Fer[mID].Med = MedValue;
						BUS2.Fer[mID].Sts.B.AuxTT3 = false;
					}
				}
				else
				{
					BUS2.Fer[mID].Med = 0;
					BUS2.Fer[mID].Sts.B.Act = false;
				}
			}
//-------------------------------------------------
//Modo de siembra Fina
//-------------------------------------------------
			else if (VSiembra)
			{
				SemSeg = (unsigned long) MedValue / (unsigned long) k_FinaTM;
			
			//Promedio las mediciones en fina
				SemMts	= (unsigned long) BUS2.Fer[mID].AxMed;
				BUS2.Fer[mID].AxMed =  (int) SemSeg;
				SemSeg = (SemSeg + SemMts)/ 2 ;
				
				BUS2.Fer[mID].Med = (int) SemSeg;
				BUS2.Fer[mID].Sts.B.Act = true; 
			}
			else
			{
				BUS2.Fer[mID].Med = 0;
				BUS2.Fer[mID].Sts.B.Act = false;
			}
			BUS2.Fer[mID].Sts.B.AuxTT2 = false;
		}
		else
		{
			//SENSOR INACTIVO
			if(!BUS2.Fer[mID].Sts.B.AuxTT2)
			{
				BUS2.Fer[mID].Sts.B.AuxTT2 = true;
				return;
			}
			BUS2.Fer[mID].Med = 0;
			BUS2.Fer[mID].Sts.B.Act = false;
			BUS2.Fer[mID].Sts.B.AuxTT3 = false;	
		}
	}
}	


/******************************************************************************
*	Funcion:		Bus2aDtsCom()
*	Descricpion:	Transfiere los datos del Bus2 LIN a bus de comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void Bus2aDtsCom(void)
{
	int i;
	float Med;
	int Valor;

	for(i=0;i<=31;i++)
	{
		//SENSORES DE SIEMBRA
		DtsComBus2.Sie.Med[i] = BUS2.Sie[i].Med;
		DtsComBus2.Sie.Sts[i] = BUS2.Sie[i].Sts.C.Val;
		
//		if(BUS2.Sie[i].Sts.B.Con && BUS2.Sie[i].Med)
//		{
//			Medicion ++;
//		}
		
		//SENSORES DE FERTILIZANTE
		DtsComBus2.Fer.Med[i] = BUS2.Fer[i].Med;
		DtsComBus2.Fer.Sts[i] = BUS2.Fer[i].Sts.C.Val;
		
//		if(BUS2.Fer[i].Sts.B.Con && BUS2.Fer[i].Med)
//		{
//			Medicion ++;
//		}
	}
}
/******************************************************************************
*	Funcion:		ADQ_AjAltaDensFact(unsigned long CantSem,unsigned char TimMed,unsigned char CantSurcos)
*	Descricpion:	Realiza la correccion por alta densidad de las semillas
*					por metro
*	Ingreso Datos:	Cantidad de semillas medidas
*					Tiempo de Medicion
*	Salida Datos:	Factor de correccion por alta densidad
******************************************************************************/
double ADQ_AjAltaDensFact(unsigned long CantSem,unsigned char TimMed)
{
	long Long00;
	double Double00,Double01;
	double RetVal;
	
	RetVal=1;
	if (CorrAlDen.B.V[0])
	{
		if (CorrAlDen.B.V[1] < CorrAlDen.B.V[2])
		{
			if (CorrAlDen.B.V[2] < CorrAlDen.B.V[3])
			{
				CantSem=CantSem * 10;
				Double00=(double)CantSem/(double)Long00;
				//Double00 = CANTIDAD DE SEMILLAS POR SEGUNDO
				Double01=(double)CorrAlDen.B.V[1];
				if (Double00 > Double01)
				{
					Double00=Double00-Double01;
					RetVal=(double)CorrAlDen.B.V[2]-Double01;
					Double01=(double)CorrAlDen.B.V[3] / (double)CorrAlDen.B.V[2];
					Double01=(Double01 * 100) - 100;
					//Double01=PORCENTAJE DE CORRECCION MAXIMO
					RetVal=Double01/RetVal;
					RetVal=RetVal*Double00;
					if (RetVal > Double01) RetVal=Double01;
					//RetVal=PORCENTAJE DE CORRECCION
					RetVal=(RetVal + 100) / 100;
					//RetVal=FACTOR DE CORRECCION
				}		
			}
		}
	}
	return(RetVal);
}

/******************************************************************************
*	Funcion:		ADQ_AjAltaDensValue(unsigned long CantSem,double Factor)
*	Descricpion:	Realiza la correccion por alta densidad de las semillas
*					por metro
*	Ingreso Datos:	Camtidad de Semillas leidas desde el sensor
*					Factor de correccion
*	Salida Datos:	Valor de las semillas corregidos
******************************************************************************/	
unsigned long ADQ_AjAltaDensValue(unsigned long CantSem,double Factor)
{
double Double00;
	Double00=(double)CantSem* Factor;
	return((unsigned long)Double00);
}


/******************************************************************************
*	Funcion:		CargaConfSen()
*	Descricpion:	Carga las configuraciones y los estados de los sensores
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void CargaConfSen(void)
{
int i;
unsigned long Mask;
	for(i=0;i<32;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		BUS1.Sie[i].Sts.B.Hab = (Mask & SenDtsHab.SemB1)>>i;
		BUS1.Sie[i].Sts.B.Det = (Mask & SenDtsCon.SemB1)>>i;
		BUS1.Sie[i].Sts.B.Mod = (Mask & SenDtsMod.SemB1)>>i;
		
		BUS1.Fer[i].Sts.B.Hab = (Mask & SenDtsHab.FerB1)>>i;
		BUS1.Fer[i].Sts.B.Det = (Mask & SenDtsCon.FerB1)>>i;
		BUS1.Fer[i].Sts.B.Mod = (Mask & SenDtsMod.FerB1)>>i;
		
		BUS2.Sie[i].Sts.B.Hab = (Mask & SenDtsHab.SemB2)>>i;
		BUS2.Sie[i].Sts.B.Det = (Mask & SenDtsCon.SemB2)>>i;
		BUS2.Sie[i].Sts.B.Mod = (Mask & SenDtsMod.SemB2)>>i;
		
		BUS2.Fer[i].Sts.B.Hab = (Mask & SenDtsHab.FerB2)>>i;
		BUS2.Fer[i].Sts.B.Det = (Mask & SenDtsCon.FerB2)>>i;
		BUS2.Fer[i].Sts.B.Mod = (Mask & SenDtsMod.FerB2)>>i;
	}
}



/******************************************************************************
*	Funcion:		GuardaConfSen()
*	Descricpion:	Carga las configuraciones y los estados de los sensores
*					Si necesito hacer una redeteccion borrar previamente los 
*					registros SenDtsHab y SenDtsCon
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GuardaConfSen(void)
{
int i;
unsigned long Mask;
		SenDtsHab.SemB1 = 0;
		SenDtsCon.SemB1 = 0;
		SenDtsMod.SemB1 = 0;
		
		SenDtsHab.FerB1 = 0;
		SenDtsCon.FerB1 = 0;
		SenDtsMod.FerB1 = 0;
		
		SenDtsHab.SemB2 = 0;
		SenDtsCon.SemB2 = 0;
		SenDtsMod.SemB2 = 0;
		
		SenDtsHab.FerB2 = 0;
		SenDtsCon.FerB2 = 0;
		SenDtsMod.FerB2 = 0;
	for(i=0;i<32;i++)
	{
		Mask = 1;
	//	Mask = Mask << i;
		
		SenDtsHab.SemB1 |= (BUS1.Sie[i].Sts.B.Hab & Mask)<<i;
		SenDtsCon.SemB1 |= (BUS1.Sie[i].Sts.B.Det & Mask)<<i;
		SenDtsMod.SemB1 |= (BUS1.Sie[i].Sts.B.Mod & Mask)<<i;
		
		SenDtsHab.FerB1 |= (BUS1.Fer[i].Sts.B.Hab & Mask)<<i;
		SenDtsCon.FerB1 |= (BUS1.Fer[i].Sts.B.Det & Mask)<<i;
		SenDtsMod.FerB1 |= (BUS1.Fer[i].Sts.B.Mod & Mask)<<i;
		
		SenDtsHab.SemB2 |= (BUS2.Sie[i].Sts.B.Hab & Mask)<<i;
		SenDtsCon.SemB2 |= (BUS2.Sie[i].Sts.B.Det & Mask)<<i;
		SenDtsMod.SemB2 |= (BUS2.Sie[i].Sts.B.Mod & Mask)<<i;
		
		SenDtsHab.FerB2 |= (BUS2.Fer[i].Sts.B.Hab & Mask)<<i;
		SenDtsCon.FerB2 |= (BUS2.Fer[i].Sts.B.Det & Mask)<<i;
		SenDtsMod.FerB2 |= (BUS2.Fer[i].Sts.B.Mod & Mask)<<i;	
	}

	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_HAB_SEN,(unsigned char *)&SenDtsHab,sizeof(struct _SenDts));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_CON_SEN,(unsigned char *)&SenDtsCon,sizeof(struct _SenDts));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_MOD_SEN,(unsigned char *)&SenDtsMod,sizeof(struct _SenDts));
}

/******************************************************************************
*	Funcion:		CargaConfPer()
*	Descricpion:	Carga las configuraciones y los estados de los perifericos
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void CargaConfPer(void)
{
int i;
unsigned long Mask;
	for(i=0;i<16;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		Moduladora[i].Sts.B.Hab = (Mask & HabPer.MOD)>>i;
		Moduladora[i].Sts.B.Det = (Mask & ConPer.MOD)>>i;		
		Moduladora[i].Sts.B.Bus = (Mask & BusPer.MOD)>>i;		
	}
	for(i=0;i<8;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		Rotacion[i].Sts.B.Hab = (Mask & HabPer.ROT)>>i;
		Rotacion[i].Sts.B.Det = (Mask & ConPer.ROT)>>i;		
		Rotacion[i].Sts.B.Bus = (Mask & BusPer.ROT)>>i;		
	}
	for(i=0;i<3;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		Turbina[i].Sts.B.Hab = (Mask & HabPer.TRB)>>i;
		Turbina[i].Sts.B.Det = (Mask & ConPer.TRB)>>i;		
		Turbina[i].Sts.B.Bus = (Mask & BusPer.TRB)>>i;		
	}
	for(i=0;i<16;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		Tolva[i].Sts.B.Hab = (Mask & HabPer.TOL)>>i;
		Tolva[i].Sts.B.Det = (Mask & ConPer.TOL)>>i;		
		Tolva[i].Sts.B.Bus = (Mask & BusPer.TOL)>>i;		
	}
	for(i=0;i<9;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		Presion[i].Sts.B.Hab = (Mask & HabPer.PRE)>>i;
		Presion[i].Sts.B.Det = (Mask & ConPer.PRE)>>i;		
		Presion[i].Sts.B.Bus = (Mask & BusPer.PRE)>>i;		
	}
}

/******************************************************************************
*	Funcion:		GrabaConfPer()
*	Descricpion:	Guarda los datos de configuracion de los prerifericos 
*					en la memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfPer(void)
{
	int i;
	unsigned long Mask;
//Moduladoras
	for(i=0;i<16;i++)
	{
		Mask = 1;
		
		HabPer.MOD |= ((Mask & Moduladora[i].Sts.B.Hab)<< i);
		ConPer.MOD |= ((Mask & Moduladora[i].Sts.B.Det)<< i);
		BusPer.MOD |= ((Mask & Moduladora[i].Sts.B.Bus)<< i);			
	}
//Rotacion
	for(i=0;i<8;i++)
	{
		Mask = 1;
		
		HabPer.ROT |= ((Mask & Rotacion[i].Sts.B.Hab)<< i);
		ConPer.ROT |= ((Mask & Rotacion[i].Sts.B.Det)<< i);
		BusPer.ROT |= ((Mask & Rotacion[i].Sts.B.Bus)<< i);			
	}
//Turbina
	for(i=0;i<3;i++)
	{
		Mask = 1;
		
		HabPer.TRB |= ((Mask & Turbina[i].Sts.B.Hab)<< i);
		ConPer.TRB |= ((Mask & Turbina[i].Sts.B.Det)<< i);
		BusPer.TRB |= ((Mask & Turbina[i].Sts.B.Bus)<< i);			
	}
	HabPer.TRB &= 0x07;
	ConPer.TRB &= 0x07;
	BusPer.TRB &= 0x07;
//Tolva
	for(i=0;i<16;i++)
	{
		Mask = 1;
		
		HabPer.TOL |= ((Mask & Tolva[i].Sts.B.Hab)<< i);
		ConPer.TOL |= ((Mask & Tolva[i].Sts.B.Det)<< i);
		BusPer.TOL |= ((Mask & Tolva[i].Sts.B.Bus)<< i);			
	}
	
//Presion
	for(i=0;i<9;i++)
	{
		Mask = 1;
		
		HabPer.PRE |= ((Mask & Presion[i].Sts.B.Hab)<< i);
		ConPer.PRE |= ((Mask & Presion[i].Sts.B.Det)<< i);
		BusPer.PRE |= ((Mask & Presion[i].Sts.B.Bus)<< i);			
	}
	EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_CON_PER,(unsigned char *)&ConPer,sizeof(struct _DtsPer));
	Dly_1_MiliSec(12);
	EepromWRBuf(M_STS_BUS_PER,(unsigned char *)&BusPer,sizeof(struct _DtsPer));	
}


/******************************************************************************
*	Funcion:		GrabaConfMod()
*	Descricpion:	Guarda los datos de configuracion de la moduladora en la 
*					memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfMod(void)
{
	int i;
	unsigned long Mask;

	for(i=0;i<16;i++)
	{
		Mask = 1;
		
		HabPer.MOD |= ((Mask & Moduladora[i].Sts.B.Hab)<< i);
		ConPer.MOD |= ((Mask & Moduladora[i].Sts.B.Det)<< i);
		BusPer.MOD |= ((Mask & Moduladora[i].Sts.B.Bus)<< i);			
	}
}

/******************************************************************************
*	Funcion:		GrabaConfROT()
*	Descricpion:	Guarda los datos de configuracion del sensor de rotacion  
*					en la memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfROT(void)
{
	int i;
	unsigned long Mask;

	for(i=0;i<8;i++)
	{
		Mask = 1;
		
		HabPer.ROT |= ((Mask & Rotacion[i].Sts.B.Hab)<< i);
		ConPer.ROT |= ((Mask & Rotacion[i].Sts.B.Det)<< i);
		BusPer.ROT |= ((Mask & Rotacion[i].Sts.B.Bus)<< i);			
	}
}

/******************************************************************************
*	Funcion:		GrabaConfTRB()
*	Descricpion:	Guarda los datos de configuracion del sensor de TRB  
*					en la memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfTRB(void)
{
	int i;
	unsigned long Mask;

	for(i=0;i<3;i++)
	{
		Mask = 1;
		
		HabPer.TRB |= ((Mask & Turbina[i].Sts.B.Hab)<< i);
		ConPer.TRB |= ((Mask & Turbina[i].Sts.B.Det)<< i);
		BusPer.TRB |= ((Mask & Turbina[i].Sts.B.Bus)<< i);			
	}
	HabPer.TRB &= 0x07;
	ConPer.TRB &= 0x07;
	BusPer.TRB &= 0x07;
}

/******************************************************************************
*	Funcion:		GrabaConfPRE()
*	Descricpion:	Guarda los datos de configuracion del sensor de Presion  
*					en la memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfPRE(void)
{
	int i;
	unsigned long Mask;

	for(i=0;i<9;i++)
	{
		Mask = 1;
		
		HabPer.PRE |= ((Mask & Presion[i].Sts.B.Hab)<< i);
		ConPer.PRE |= ((Mask & Presion[i].Sts.B.Det)<< i);
		BusPer.PRE |= ((Mask & Presion[i].Sts.B.Bus)<< i);			
	}
	HabPer.PRE&= 0x1FF;
	ConPer.PRE &= 0x1FF;
	BusPer.PRE &= 0x1FF;
}
/******************************************************************************
*	Funcion:		GrabaConfTol()
*	Descricpion:	Guarda los datos de configuracion de la moduladora en la 
*					memoria
*					RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void GrabaConfTol(void)
{
	int i;
	unsigned long Mask;

	for(i=0;i<16;i++)
	{
		Mask = 1;
		
		HabPer.TOL |= ((Mask & Tolva[i].Sts.B.Hab)<< i);
		ConPer.TOL |= ((Mask & Tolva[i].Sts.B.Det)<< i);
		BusPer.TOL |= ((Mask & Tolva[i].Sts.B.Bus)<< i);			
	}
}

/******************************************************************************
*	Funcion:		Bus1aDtsCom()
*	Descricpion:	Transfiere los datos del Bus2 LIN a bus de comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void Mod2DtsCom(void)
{
	int i;
	int cont = 0;

	for(i=0;i<16;i++)
	{
		DtsPerCom.MOD.Al[i]	= Moduladora[i].Al.Val ;
		DtsPerCom.MOD.Sts[i]= Moduladora[i].Sts.Val ;
		if( Moduladora[i].Sts.B.Det)
			cont ++;
	}
	if(cont)
	{
		Sensores.STS.B.TX_MOD = true;
		Sensores.tMOD = 0;
	}
	else if( Sensores.tMOD >= kMaxEscan)
	{
		Sensores.STS.B.TX_MOD = true;
		Sensores.tMOD = 0;	
	}
	
}

/******************************************************************************
*	Funcion:		Bus1aDtsCom()
*	Descricpion:	Transfiere los datos del Bus2 LIN a bus de comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void ROT2DtsCom(void)
{
	int i;
	int cont = 0;
		
	for(i=0;i<8;i++)
	{
		DtsPerCom.ROT.Med[i]	= Rotacion[i].Med ;
		DtsPerCom.ROT.Sts[i]	= Rotacion[i].Sts.Val ;
		if(Rotacion[i].Sts.B.Det)
			cont ++;
	}
	if(cont)
	{
		Sensores.STS.B.TX_ROT = true;
		Sensores.tROT = 0;
	}
	else if( Sensores.tROT >= kMaxEscan)
	{
		Sensores.STS.B.TX_ROT = true;
		Sensores.tROT = 0;	
	}	
}

/******************************************************************************
*	Funcion:		TRB2DtsCom()
*	Descricpion:	Transfiere los datos de los sensores de turbina al bus de 
*					comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void TRB2DtsCom(void)
{
	int i;
	int cont = 0;

	for(i=0;i<3;i++)
	{
		
		DtsPerCom.TRB.Med[i]= Turbina[i].Med ;
		DtsPerCom.TRB.Sts[i]= Turbina[i].Sts.Val ;
		
		TRBChekAl00(i);
		
		if(Turbina[i].Sts.B.Det)
			cont ++;
	}
	if(cont)
	{
		Sensores.STS.B.TX_TRB = true;
		Sensores.tTRB = 0;
	}
	else if(Sensores.tTRB >= kMaxEscan)
	{
		Sensores.STS.B.TX_TRB = true;
		Sensores.tTRB = 0;	
	}
	
}

/******************************************************************************
*	Funcion:		PRE2DtsCom()
*	Descricpion:	Transfiere los datos de los sensores de presion al bus de 
*					comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void PRE2DtsCom(void)
{
	int i;
	int cont = 0;

	for(i=0;i<kPREMax;i++)
	{
		PREChekAl00(i);
		DtsPerCom.PRE.Med[i]= Presion[i].Med ;
		DtsPerCom.PRE.Sts[i]= Presion[i].Sts.Val ;
		if(Presion[i].Sts.B.Det)
			cont ++;
	}
	if(cont)
	{
		Sensores.STS.B.TX_PRE = true;
		Sensores.tPRE = 0;
	}
	else if(Sensores.tPRE >= kMaxEscan)
	{
		Sensores.STS.B.TX_PRE = true;
		Sensores.tPRE = 0;	
	}
}
/******************************************************************************
*	Funcion:		TOL2DtsCom()
*	Descricpion:	Transfiere los datos del LIN a bus de comunicaciones RS232
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void TOL2DtsCom(void)
{
	int i;
	int cont = 0;
		

	for(i=0;i<16;i++)
	{
		DtsPerCom.TOL.Sts[i]= Tolva[i].Sts.Val;
		if( Tolva[i].Sts.B.Det)
			cont ++;
	}
	if(cont)
	{
		Sensores.STS.B.TX_TOL = true;
		Sensores.tTOL = 0;
	}
	else if(Sensores.tTOL >= kMaxEscan)
	{
		Sensores.STS.B.TX_TOL = true;
		Sensores.tTOL = 0;	
	}
}
/******************************************************************************
*	Funcion:		CargaId()
*	Descricpion:	Lee el bus LIN1 y esperar que no halla ninguno conectado
*					luego espera a que se conecte uno lee el ID, detecta el tipo
*					de sensor y los limites del ID y permite cambiar de ID
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void CargaId(void)
{
//Hay una comunicacion pendiente de ser atendida
	if (SW1PortSys.Sts.B.fPend)
		return;
//Esta el bus ocupado
	if(SetId.StsBus.Ocupado)
	{
		if (!SetId.StsBus.SenLec)
		{
			SW1_PortSysStart(0xFF,0x07 | SW1_cmdRd,1);
			SW1_PortSysSend();
			SetId.StsBus.SenLec = true;	
		}
		else if (SW1PortSys.Sts.B.fOk)
		{
			SetId.StsBus.SenLec = false;	
			SetId.StsBus.Ocupado = true;
			SetId.IdMax = 0xFF;
			SetId.IdMin = 0xFF;	
			SetId.NuevoId = 0;	
		}
		else
		{
			SetId.NuevoId ++;
			if(SetId.NuevoId > 5)
			{
				SetId.StsBus.Ocupado = false;
				SetId.StsBus.SenLec = false;	
				SetId.StsBus.SenEsc = false;
				SetId.StsBus.Libre = true;
				SetId.IdMax = 0x00;
				SetId.IdMin = 0x00;
				SetId.NuevoId = 0x00;
			}
		}
	}
//A ver para cuando conecta el nuevo sensor
	else if(SetId.StsBus.Libre)
	{
		if (!SetId.StsBus.SenLec)
		{
			SW1_PortSysStart(0xFF,0x07 | SW1_cmdRd,1);
			SW1_PortSysSend();
			SetId.StsBus.SenLec = true;	
		}
		else if (SW1PortSys.Sts.B.fOk)
		{
			SetId.StsBus.Libre = false;
			SetId.StsBus.SenLec = false;
			SetId.StsBus.Sensor = true;	
			SetId.Id = SW1.buf[0];
			MaxMinId(SetId.Id);
//			SetId.NuevoId =  SW1.buf[0];		
		}
		else
		{
			SetId.StsBus.Libre = true;
			SetId.StsBus.SenLec = false;
			SetId.StsBus.Sensor = false;
			SetId.IdMax = 0x00;
			SetId.IdMin = 0x00;
			SetId.Id =  0x00;
		}	
	}
//Porfin conectaste un sensor
	else if(SetId.StsBus.Sensor)
	{
		if(SetId.StsBus.GrabaID)
		{
			if(SetId.StsBus.SenEsc)
			{
				SW1_PortSysStart(SetId.Id,0x07 | SW1_cmdWr,0);
				SW1_PortSysWr2(SetId.NuevoId);
				SW1_PortSysSend();
				SetId.StsBus.SenEsc = false;	
			}
			else if (SW1PortSys.Sts.B.fOk)
			{
				SetId.StsBus.Libre = false;
				SetId.StsBus.SenLec = false;
				SetId.StsBus.Sensor = true;	
				SetId.StsBus.GrabaID = false;		
				SetId.Id = SetId.NuevoId;
				MaxMinId(SetId.Id);		
			}
			else
			{
				SetId.StsBus.Libre = true;
				SetId.StsBus.SenLec = false;
				SetId.StsBus.SenEsc = false;
				SetId.StsBus.Sensor = false;
			}
		}
		else
		{
//Lectura del sensor conectado
			if (!SetId.StsBus.SenLec)
			{
				SW1_PortSysStart(SetId.Id,0x07 | SW1_cmdRd,1);
				SW1_PortSysSend();
				SetId.StsBus.SenLec = true;	
			}
			else if (SW1PortSys.Sts.B.fOk)
			{
				SetId.StsBus.Libre = false;
				SetId.StsBus.SenLec = false;
				SetId.StsBus.Sensor = true;	
				SetId.Id = SW1.buf[0];
				MaxMinId(SetId.Id);
			}
			else
			{
				SetId.StsBus.Libre = true;
				SetId.StsBus.SenLec = false;
				SetId.StsBus.Sensor = false;
			}
		}		
	}
}

/******************************************************************************
*	Funcion:		MaxMinId()
*	Descricpion:	Con el ID del sensor carga en la estructura SetID los 
*					valores de ID maximo, minimo y el de ID virgen
*					
*	Ingreso Datos:	ID del sensor
*	Salida Datos:	Ninguno
******************************************************************************/
void MaxMinId(unsigned char Id)
{	
//Sensores de semillas
	if((Id>=0x00 && Id<=0x1F) || Id == 0xF0 )
	{	
		SetId.IdMin = 0x00;
		SetId.IdMax = 0x1F;
		SetId.VoidId = 0xF0;
	}
//Sensores de Fertilizantes	
	else if((Id>=0x20 && Id<=0x3F) || Id == 0xF0 )
	{	
		SetId.IdMin = 0x20;
		SetId.IdMax = 0x3F;
		SetId.VoidId = 0xF0;
	}	
//Sensor de Rotacion
	else if((Id>=0x40 && Id<=0x47) || Id == 0xF1 )
	{	
		SetId.IdMin = 0x40;
		SetId.IdMax = 0x47;	
		SetId.VoidId = 0xF1;
	}
//Sensor de nivel de tolva
	else if(((Id>=0x48) && (Id<=0x57)) || Id == 0xF2 )
	{	
		SetId.IdMin = 0x48;
		SetId.IdMax = 0x57;
		SetId.VoidId = 0xF2;	
	}
//Sensor Turbina
	else if((Id>=0xD3 && Id<=0xD5))
	{	
		SetId.IdMin = 0xD3;
		SetId.IdMax = 0xD5;	
		SetId.VoidId = 0xD3;
	}
//Sensor de Presion
	else if(Id>=0x80 && Id<=0x88)
	{	
		SetId.IdMin = 0x80;
		SetId.IdMax = 0x88;	
		SetId.VoidId = 0x80;
	}
//Sensor conectado no modifica ID
	else
	{
		SetId.IdMin = 0x00;
		SetId.IdMax = 0x00;
		SetId.VoidId = 0x00;
	}
}

/******************************************************************************
*	Funcion:		Adq_Rotacion()	
*	Descricpion:	Adquiere los datos de proceso de los sensores de rotacion
*					conectados, esta funcion se utiliza cuando se hace un 
*					cambio de dosis
*	Nota:			Cuando esta funcion esta activa no leer los datos desde 
*					Adq_Proc_Lin1()	y Adq_Proc_Lin2()	
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	

void Adq_Rotacion(void)
{
	unsigned int Id;
	unsigned int Medicion;
	switch(RotRLect)
	{
		default:
			RotRLect=0;
			RotID = 0;
		case 0:
//Lectura de datos sensores de rotacion
			Proceso.B.fAdqRot2 = false;
			Proceso.B.fAdqRot1 = false;
			for(;RotID<8;)
			{
				Id = 0x40 + RotID;
				//Sensor habilitado para lectura
				if(Rotacion[RotID].Sts.B.Hab && Rotacion[RotID].Sts.B.Det )
				{	break;	}
				RotID++;
			}
			if(RotID>=8)
			{
				RotID = 0;
				RotRLect=200;
				Proceso.B.fAdqRot2 = true;
// Poner el reset del llamado a asta funcion
				Sts_Tmr.B.ROTPls = false;
//				RLectCnt ++;
//				if(RLectCnt >20)
//				{	Proceso.B.fDosis = false;	}
				break;
			}
			ErrorRot=0;
			RotRLect++;
		case 1:
			Id = 0x40 + RotID;
			if(!Rotacion[RotID].Sts.B.Bus)
			{
				if (SW1PortSys.Sts.B.fPend)	//Hay una comunicacion pendiente de ser atendida
					return;
				SW1_PortSysStart(Id,0x00 | SW1_cmdRd,2);
				SW1_PortSysSend();
			}
			else
			{
				if (SW2PortSys.Sts.B.fPend)	//Hay una comunicacion pendiente de ser atendida
					return;
				SW2_PortSysStart(Id,0x00 | SW2_cmdRd,2);
				SW2_PortSysSend();
			}
			RotRLect++;
		break;
		case 2:
			if(!Rotacion[RotID].Sts.B.Bus)
			{
				if (SW1PortSys.Sts.B.fOk)
				{
					Rotacion[RotID].Med = *(unsigned int*)&SW1.buf[0];
					Rotacion[RotID].Sts.B.Con = true;
					Rotacion[RotID].Sts.B.FDs = false;
				}
				else if(SW1PortSys.Sts.B.fErr && !Rotacion[RotID].Sts.B.FDs)
				{
					if(ErrorRot>=2)
					{
						ErrorRot=0;					
						Rotacion[RotID].Med = 0;
						Rotacion[RotID].Sts.B.Con = false;
						Rotacion[RotID].Sts.B.FDs = true;
					}
					else
					{
						RotRLect = 1;
						break;
					}
				}
			}
			else
			{
				if (SW2PortSys.Sts.B.fOk)
				{
					Rotacion[RotID].Med = *(unsigned int*)&SW2.buf[0];
					Rotacion[RotID].Sts.B.Con = true;
					Rotacion[RotID].Sts.B.FDs = false;
				}
				else if(SW2PortSys.Sts.B.fErr && !Rotacion[RotID].Sts.B.FDs)
				{
					if(ErrorRot>=2)
					{
						ErrorRot=0;					
						Rotacion[RotID].Med = 0;
						Rotacion[RotID].Sts.B.Con = false;
						Rotacion[RotID].Sts.B.FDs = true;
					}
					else
					{
						RotRLect = 1;
						break;
					}
				}
			}	
			RotID++;
			if(RotID<8)
			{
				RotRLect=200;
				Sts_Tmr.B.ROTPls = false;
//Poner el reseteo de la funcion
//				RLectCnt ++;
//				if(RLectCnt >20)
//				{	Proceso.B.fDosis = false;	}
				break;	
			}
			else 
			{
				RotID = 0;
				RotRLect=0;
				Proceso.B.fAdqRot2 = true;
			}
			break;
	}
}			
			

