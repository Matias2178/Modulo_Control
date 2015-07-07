/*****************************************************************************
 *
 * Basic Explorer 16 I/O Mapping functionality for PPS peripherals.
 *
 *****************************************************************************
 * FileName:        iomapping.c
 * Dependencies:    system.h
 * Processor:       PIC24
 * Compiler:       	MPLAB C30
 * Linker:          MPLAB LINK30
 * Company:         Microchip Technology Incorporated
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Brant Ivey			3/14/06    Modified for PIC24FJ64GA004 family
 *****************************************************************************/

	#include <p24Fxxxx.h>
	#include "ES_Conf.h"
	
	
/******************************************************************************
*	Funcion:		Config_ES()
*	Descricpion:	Configura los pines como entrada o salida segun su uso,
*					Asigna los puertos remapeables a los puertos de comunicaciones
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void Config_ES()
{
//-----------------------------------------------------------------------------
	//Mapeo de Entrada Salida Puerto Serie
    //        
		//Uart2 Pin 49 RP24
			RPINR19bits.U2RXR = RP3;
	//		RPINR19bits.U2RXR = RP24;	//Placa Adaptadora
 		//Uart 2 Pin 50 RP23
			RPOR6bits.RP12R  = U2TX_IO;
	//		RPOR5bits.RP11R  = U2TX_IO;	//Placa Adaptadora
  
//----------------------------------------------------------------------              
        //Uart1 Pin 42 RP2
			RPINR18bits.U1RXR = RP2;
	//		RPINR18bits.U1RXR = RP11;	//Puerto Auxiliar
 		//Uart 1 Pin 43 RP4
    		RPOR2bits.RP4R = U1TX_IO; 
    //		RPOR12bits.RP24R = U1TX_IO; //Puerto Auxiliar   
            
        //Uart3 Pin 42 RP2
			RPINR17bits.U3RXR = RPI45;
	//		RPINR21bits.U3CTSR = RP2;
 		//Uart3 Pin 43 RP4
            RPOR15bits.RP30R = U3TX_IO;           
       //     RPOR1bits.RP3R = U3RTS_IO;   
       
         //Uart4 Pin 42 RP2
	//		RPINR27bits.U4RXR = RP24;
 		//Uart 4 Pin 43 RP4
    //        RPOR6bits.RP12R = U4TX_IO;  
//-----------------------------------------------------------------------------
//Mapeo de las entradas de interrupciones para la comunicacion LIN
	
			RPINR0bits.INT1R = RP10;	//ENTRADA LIN1
			
			RPINR1bits.INT2R = RP17;	//ENTRADA LIN2
            

            
//-----------------------------------------------------------------------------
	//Configuracion de los TRIS de los puertos
	
			TRISB = InitSetTrisB;
			TRISC = InitSetTrisC;
			TRISD = InitSetTrisD;
			TRISE = InitSetTrisE;
			TRISF =	InitSetTrisF;
}


