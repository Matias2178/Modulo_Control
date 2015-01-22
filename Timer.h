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
extern void Dly_1_MiliSec(unsigned char Dly);
extern void Dly_10_MiliSec(unsigned char Dly);
extern void Dly_100_MiliSec(unsigned char Dly);
extern void Dly_1_useg(void);

extern volatile int TMRdly0001_0;			//TIMER DE RETARDO DE 1/1000 DE Seg.
extern volatile int TMRdly0010_0;			//TIMER DE RETARDO DE 1/100 DE Seg.
extern volatile  int TMRdly0100_0;			//TIMER DE RETARDO DE 1/10 DE Seg.

