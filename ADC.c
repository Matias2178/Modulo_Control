/*****************************************************************************
 *	FUNCIONES DE LECTURA ANALOGICA
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
 
	#include 	<p24Fxxxx.h>
	#include 	"ADC.h"	
	#include	"VarGlob.h"
	#include 	"Timer.h"
	#include 	"ES_Conf.h"
	#include	"Constantes.h"
	
/******************************************************************************
*	Funcion:		ADConf()	
*	Descricpion:	Realiza la medicion del puerto analogico, raliza la conver-
*					sion a tension y lo guarda en la variable asignada.
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/		
void ADConf(void)
{
	int ADCValue;
//	LED1 = true;
	AD1PCFGL = 0xFFFE; // AN0 as analog, all other pins are digital
	AD1CON1  = 0x0000; // SAMP bit = 0 ends sampling and starts converting
	AD1CHS   = 0x0000; // Connect AN2 as S/H+ input
	// in this example AN2 is the input
	AD1CSSL = 0;
	AD1CON3 = 0x0002; // Manual Sample, Tad = 3Tcy
	AD1CON2 = 0;
	AD1CON1bits.ADON = 1; // turn ADC ON
	
	AD1CON1bits.SAMP = 1; // start sampling...
	Dly_10_MiliSec(5); 			// Ensure the correct sampling time has elapsed
							// before starting conversion.
	AD1CON1bits.SAMP = 0; // start converting
	while (!AD1CON1bits.DONE)
	{	
		ExeTask(); // conversion done?
	}
	Tension = ADC1BUF0; // yes then get ADC value
//	Valor = ADC1BUF0;

	Tension = (Tension * 33)/1024;
	Tension = Tension + (Tension * 9 /100); // 9 Factor de correccion de tension
}

/******************************************************************************
*	Funcion:		ADConvInt()	
*	Descricpion:	Inicializa los pines para la lectura analogica de la tension
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void ADConvInt(void)
{
	AD1PCFGL = 0xFFFE; 		// Configure A/D port
							// AN0 input pin is analog
	AD1PCFGH = 0x0000;
	
	AD1CON1 = 0x0000; 	
	AD1CON2 = 0x003C; 	
	AD1CON3 = 0x0100; 
	AD1CHS = 0; 	
	AD1CSSL = 0x0001; 
	
	IFS0bits.AD1IF = 0; 
	IEC0bits.AD1IE = 1; 
	
	
	AD1CON1bits.SAMP = 1; 
	Dly_10_MiliSec(5);  
	AD1CON1bits.SAMP = 0; 
	AD1CON1bits.ADON = 1; 
}
	
	
