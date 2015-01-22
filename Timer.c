/*****************************************************************************
 *	MANEJO DE LAS INTERRUPCIONES DE LOS TIMER QUE NO SON UTILIZADAS POR 
 *  LOS MODULOS DE COMUNICACION LIN
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/

//-----------------------------------------------------------------------------
	//	INTERRUPCION DEL TIMER1
	//	INTERRUPCION CADA 1miliSegundo

//		#include 	<p24Fxxxx.h>
//		#include	"VarType.h"
//		#include	"VarGlob.h"

		#include 	"ExeTask.h"
		
 volatile  int TMRdly0001_0;			//TIMER DE RETARDO DE 1/1000 DE Seg.
 volatile  int TMRdly0010_0;			//TIMER DE RETARDO DE 1/100 DE Seg.
 volatile  int TMRdly0100_0;			//TIMER DE RETARDO DE 1/10 DE Seg.
 
 
/******************************************************************************
*	Funcion:		Dly_1_MiliSec()
*	Descricpion:	Genera un retardo con una resolucion de 1 milisegundos
*					puede generar hasta 0.255 segundos de retardo
*	Ingreso Datos:	Multiplicador del retardo
*	Salida Datos:	Ninguno
******************************************************************************/
void Dly_1_MiliSec(unsigned char Dly)
{
	TMRdly0001_0=Dly;
	while(TMRdly0001_0)
		{
		ExeTask();
		}
}


/******************************************************************************
*	Funcion:		Dly_10_MiliSec()
*	Descricpion:	Genera un retardo con una resolucion de 10 milisegundos
*					puede generar hasta 2.55 segundos de retardo
*	Ingreso Datos:	Multiplicador del retardo
*	Salida Datos:	Ninguno
******************************************************************************/
void Dly_10_MiliSec(unsigned char Dly)
{
	TMRdly0010_0=Dly;
	while(TMRdly0010_0)
		{
		ExeTask();
		}
}

/******************************************************************************
*	Funcion:		Dly_100_MiliSec()
*	Descricpion:	Genera un retardo con una resolucion de 100 milisegundos
*					puede generar hasta 25.5 segundos de retardo
*	Ingreso Datos:	Multiplicador del retardo
*	Salida Datos:	Ninguno
******************************************************************************/
void Dly_100_MiliSec(unsigned char Dly)
{
	TMRdly0100_0=Dly;
	while(TMRdly0100_0)
		{
		ExeTask();
		}
}

/******************************************************************************
*	Funcion:		Dly_1_useg()
*	Descricpion:	Genera un retardo de 1 microsegundo
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/	
void Dly_1_useg(void)
{
	__asm__ volatile ("bra		DlyMicroS_0");	
	__asm__ volatile ("DlyMicroS_0: bra		DlyMicroS_1");		
	__asm__ volatile ("DlyMicroS_1:");
}

