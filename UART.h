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
extern	unsigned int U1TxInd; 
extern	unsigned int U2TxInd;
extern	unsigned int U3TxInd;
extern	unsigned char RxBuf[256];
extern	unsigned int RxInd;

extern	unsigned char Rx232Buf[256];
extern	unsigned int Rx232Ind;

extern	unsigned char Rx3Buf[256];
extern	unsigned int Rx3Ind;


extern	unsigned char QTxBuf[256];
extern	unsigned char ComBuf[260];

extern 	unsigned char WFcmd[6];
extern 	unsigned char WFind;
extern	unsigned char U1RxBuf[80];
extern	unsigned int U1RxInd;
extern	unsigned int U1RxBf;

extern	unsigned char U1TxBuf[80];
extern	unsigned char U2TxBuf[260];
extern	unsigned char U3TxBuf[260];

extern	unsigned char SAVE[512];

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


