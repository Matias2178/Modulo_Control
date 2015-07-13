
/************************************************************************
*                                                                       *
* Redefinicion del mapeo de los Pines	 			                    *
*                                                                       *
*************************************************************************

************************************************************************/

	#include <p24Fxxxx.h>
//-----------------------------------------------------------------------------
	//Declaracion de Funciones
		extern void Config_ES();
		extern void unlock();
		extern void lockIO();

//-----------------------------------------------------------------------------
	//ADC input mapping
	
	//Habilitacion de energia
	
		#define Pwr_Gral			LATEbits.LATE1
		#define	Pwr_GPS				LATEbits.LATE0
		#define Pwr_Wifi			LATFbits.LATF1
		#define	Hab_GPS				LATFbits.LATF0
		      
	//DEFINICION DE LEDS
 		#define LED_POWER		LATBbits.LATB4
        #define LED_LIN1		LATBbits.LATB8
        #define LED_LIN2		LATBbits.LATB9
        #define LED_CAN			LATBbits.LATB10
        #define LED_UART		LATBbits.LATB11
        #define LED_GPS			LATBbits.LATB12
        #define LED_WIFI		LATBbits.LATB13 
        
//----------------------------------------------------------------------------------
//			SIN PLACA ADAPTADORA AMP SEAL 14VIAS        
//        #define LED_POWER_E		LATDbits.LATD6	//Corregido para placa de leds
//        #define LED_LIN1_E		LATDbits.LATD2	//Corregido para placa de leds
//        #define LED_LIN2_E		LATDbits.LATD5	//Corregido para placa de leds
//        #define LED_UART_E		LATDbits.LATD7
//        #define LED_GPS_E		LATDbits.LATD3	//Corregido para placa de leds
//        #define LED_WIFI_E		LATDbits.LATD4	//Corregido para placa de leds
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//			CON PLACA ADAPTADORA AMP SEAL 14VIAS
		#define LED_POWER_E		LATDbits.LATD2	//Corregido para placa de leds
		#define LED_LIN1_E		LATDbits.LATD6	//Corregido para placa de leds
		#define LED_LIN2_E		LATDbits.LATD3	//Corregido para placa de leds
		#define LED_WIFI_E		LATDbits.LATD4	//Corregido para placa de leds
		#define LED_GPS_E 		LATDbits.LATD5	//Corregido para placa de leds
//-----------------------------------------------------------------------------------

  
    //Tecla de inicio / implement switch
    	#define TECLA_TRAC		PORTBbits.RB3
    	#define IMPLEMENT_SW	PORTBbits.RB3  
        
    //Pines control modulo Wifi 
        #define Wifi_Conf		LATBbits.LATB5	//output
        #define Wifi_Reset		LATGbits.LATG3	//output
        #define Wifi_Rx			LATFbits.LATF6	//???
        #define Wifi_Tx			LATFbits.LATF2	//???
        #define Wifi_CTS		LATFbits.LATF3	//output
        #define Wifi_RTS		LATBbits.LATB2	//input
        
        
	//RELES
		#define	Rele1			LATEbits.LATE2			
		#define	Rele2			LATEbits.LATE3
		
	//Habilitacion de Memoria EEPROM
		#define HabMen			PORTEbits.RE5
	
    //COMUNICACION LIN
    	//PUERTO LIN1
    	#define OUT_SW1_BUS		LATBbits.LATB14
    	#define IN_SW1_BUS		PORTFbits.RF4
    	
    	#define OUT_SW2_BUS		LATBbits.LATB15
    	#define IN_SW2_BUS		PORTFbits.RF5
    	
    
//-----------------------------------------------------------------------------
	//Configuracion de los Tris de los puertos
		#define	InitSetTrisB	0x00CF //b0b0011000011111011
	/* 	RB15 LIN2_Tx	O	RB14 LIN1_Tx	O	RB13 LED_WIFI 	O	RB12 LED_GPS	O	0000 = 0
	//	RB11 LED_RS232	O	RB10 LED_CAN	O	RB09 LED_LIN2 	O	RB08 LED_LIN1	O	0000 = 0	
	//	RB07 CAN-R		?	RB06 CAN-D		?	RB05 Wifi_Conf	O	RB04 LED_POWER	O	1100 = C	
	//	RB03 TEC_PWR 	I	RB02 Wifi_RTS 	I	RB01 Bibration	I	RB00 AN0		I   1111 = F*/			//VER SI TX DEL 232 SE COLOCA EN RB3
	
		#define	InitSetTrisC	0x3000 //b0b0011000011111011	Para la configuracion de los Osc

		#define	InitSetTrisD	0x0F83	//0b1111111011111111
	/* 	RD15 No Disp	0	RD14 No Disp	0	RD13 No Disp	0	RD12 No Disp	0	0000 = 0
	//	RD11 RS-232		1 	RD10 RS-232		1	RD09 GPS Rx		I	RD08 GPS Tx		1	1111 = F
	//	RD07 CONECTOR	?	RD06 CONECTOR	?	RD05 CONECTOR	?	RD04 CONECTOR	?	1111 = F
	//	RD03 CONECTOR	?	RD02 CONECTOR	?	RD01 CONECTOR	?	RD00 CONECTOR	?   1111 = F*/

		#define	InitSetTrisE	0xFFD0	//0b1111111100010000
	/* 	RE15 No Disp	?	RE14 No Disp	?	RE13 No Disp	?	RE12 No Disp	?	1111 = F
	//	RE11 No Disp	?	RE10 No Disp	?	RE09 No Disp	?	RE08 No Disp	?	1111 = F
	//	RE07 24LC SDA 	O	RE06 24LC SCL 	O	RE05 24LC WP 	O	RE04 NC			?	0001 = 1
	//	RE03 RELE 2		O	RE02 RELE 1		O	RE01 PWR GEN	O	RE00 PWR GPS	O	0000 = 0 */

		#define	InitSetTrisF	0X0074	//0b1111111111111101
	/* 	RF15 No Disp	0	RF14 No Disp	0	RF13 No Disp	0 	RF12 No Disp	0	0000 = 0
	//	RF11 No Disp	0	RF10 No Disp	0	RF09 No Disp	0	RF08 No Disp	0	0000 = 0
	//	RF07 No Disp	0	RF06 Wifi_Rx	I? 	RF05 LIN2_Rx 	I	RF04 LIN1_Rx 	I	0111 = 7
	//	RF03 Wifi_CTS	O 	RF02 Wifi_Tx	I? 	RF01 PWR WIFI	O	RF00 GPS HAB	0	0100 = 4*/
					
		#define	InitSetTrisG	0x0304	//"0b1111111100111111"
	/* 	RG15 No Disp	0	RG14 No Disp	0	RG13 No Disp	0 	RG12 No Disp	0	0000 = 0
	//	RG11 No Disp	0	RG10 No Disp	0	RG09 AT26 SC 	0	RG08 AT26 SI 	I	0011 = 1
	//	RG07 AT26 SO 	0 	RG06 AT26 SCK 	0	RG05 No Disp	0	RG04 No Disp	0	0000 = 0
	//	RG03 Wifi_Reset	0 	RG02  			? 	RG01 No Disp	0	RG00 No Disp	0	0100 = 4*/



//-----------------------------------------------------------------------------
	//Estado Inicial de los Puertos


	#define	InitSetPortB		0b0000000000000000
	#define	InitSetPortC		0b0000000000000000
	#define	InitSetPortD		0b0000000000000000
	#define	InitSetPortE		0b0000000000000000
	#define	InitSetPortF		0b0000000000000000
	#define	InitSetPortG		0b0000000000000000
        
//-----------------------------------------------------------------------------
	//Seleccion de perifericos de salida - PPS Outputs 
	
        #define NULL_IO		0
        #define C1OUT_IO	1
        #define C2OUT_IO	2
        #define U1TX_IO		3
        #define U1RTS_IO	4
        #define U2TX_IO		5
        #define U2RTS_IO	6
        #define SDO1_IO		7
        #define SCK1OUT_IO	8
        #define SS1OUT_IO	9
        #define SDO2_IO		10
        #define SCK2OUT_IO	11
        #define SS2OUT_IO	12
        #define OC1_IO		18
        #define OC2_IO		19
        #define OC3_IO		20
        #define OC4_IO		21
        #define OC5_IO		22
        #define OC6_IO      23
        #define OC7_IO      24
        #define OC8_IO      25
        #define U3TX_IO     28
        #define U3RTS_IO    29
        #define U4TX_IO     30
        #define U4RTS_IO    31
        #define SDO3_IO     32
        #define SCK3OUT_IO  33
        #define SS3OUT_IO   34
        #define OC9_IO      35

	//Seleccion de puertos de entrada
		#define	RP0			0
		#define	RP1			1
		#define	RP2			2
		#define	RP3			3
		#define	RP4			4
	//	#define	RP5			5		//No usado en el GA106
		#define	RP6			6
		#define	RP7			7
		#define	RP8			8
		#define	RP9			9
		#define	RP10		10
		#define	RP11		11
		#define	RP12		12
		#define	RP13		13
		#define	RP14		14
	//	#define	RP15		15		//No usado en el GA106
		#define	RP16		16
		#define	RP17		17
		#define	RP18		18
		#define	RP19		19
		#define	RP20		20
		#define	RP21		21
		#define	RP22		22
		#define	RP23		23
		#define	RP24		24
		#define	RP25		25
		#define	RP26		26
		#define	RP27		27
		#define	RP28		28
		#define	RP29		29
		#define	RP30		30
		#define	RPI37		37
		#define	RPI45		45		

		


