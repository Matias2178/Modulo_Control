/*****************************************************************************
 *	FUNCIONES DE LECTURA ESCRITURA DE LOS PUERTOS DE LA UART
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 
 *	FECHA		:	2013
 *	
 *****************************************************************************/
 //#ifndef UART2_H
//#define UART2_H

	#include	"ES_Conf.h"
	#include	"Constantes.h"

/*****************************************************************************
 * DEFINITIONS
 *****************************************************************************/
extern	unsigned int U1TxInd __attribute__ ((far)); 
extern	unsigned int U2TxInd __attribute__ ((far));
extern	unsigned int U3TxInd __attribute__ ((far));

extern	unsigned char Rx232Buf[256] __attribute__ ((far));
extern	unsigned int Rx232Ind __attribute__ ((far));

extern	unsigned char Rx3Buf[256] __attribute__ ((far));
extern	unsigned int Rx3Ind __attribute__ ((far));


extern	unsigned char QTxBuf[256] __attribute__ ((far));
extern	unsigned char ComBuf[260] __attribute__ ((far));

extern 	unsigned char WFcmd[6] __attribute__ ((far));
extern 	unsigned char WFind __attribute__ ((far));
extern	unsigned char U1RxBuf[80] __attribute__ ((far));
extern	unsigned int U1RxInd __attribute__ ((far));
extern	unsigned int U1RxBf __attribute__ ((far));

extern	unsigned char U1TxBuf[80] __attribute__ ((far));
extern	unsigned char U2TxBuf[260] __attribute__ ((far));
extern	unsigned char U3TxBuf[260] __attribute__ ((far));



extern void UART2Init();
extern void UART1Init();
extern void UART3Init();
extern void UART4Init();

extern void UART2PutChar(char Ch);
extern char UART2IsPressed();
extern char UART2GetChar();
extern void UART2PutDec(unsigned char Dec);

extern int SCI_SetBaud(char Baud);

/*****************************************************************************
 * EOF
 *****************************************************************************/


