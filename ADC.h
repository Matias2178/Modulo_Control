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
  //Constantes para la configuracion de los AD
  // AD1CHS
 	#define	ADCH0	0b0000000000000000
 	#define	ADCH1	0b0000000100000001
 	#define	ADCH2	0b0000001000000010
 	#define	ADCH3	0b0000001100000011
 	#define	ADCH4	0b0000010000000100
 	#define	ADCH5	0b0000010100000101
 	#define	ADCH6	0b0000011000000110
 	#define	ADCH7	0b0000011100000111
 	#define	ADCH8	0b0000100000001000
 	#define	ADCH9	0b0000100100001001
 	#define	ADCH10	0b0000101000001010
 	#define	ADCH11	0b0000101100001011
 	#define	ADCH12	0b0000110000001100
 	#define	ADCH13	0b0000110100001101
 	#define	ADCH14	0b0000111000001110
 	#define	ADCH15	0b0000111100001111
 	
 //
 	
 extern void ADConf(void);
 extern void ADConvInt(void);
/**
 End of File
*/
 