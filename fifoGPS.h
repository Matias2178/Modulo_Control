/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
//#include "Compiler.h"


#define BUFFER_SIZE 1024
#define INPUT_REG   U1RXREG

#define buffer GPSFIFOBufferRX
#define cursor GPSFIFOCursorRX
#define pos    GPSFIFOPosRX
#define flags  GPSFIFOFlagsRX
#define FIFOInit  GPSFIFOInitRX
#define FIFOISR   GPSFIFOISRRX
#define FIFORead  GPSFIFOReadRX
#define FIFOEmpty GPSFIFOEmptyRX
#define FIFOFull  GPSFIFOFullRX
#define FIFOOverflow  GPSFIFOOverflowRX
#define FIFOClearOverflow  GPSFIFOClearOverflowRX

/* Prototipos */
void GPSFIFOInitRX (void);
int GPSFIFOISRRX(void);
unsigned char GPSFIFOReadRX(void);
unsigned char GPSFIFOEmptyRX(void);
unsigned char GPSFIFOFullRX(void);
unsigned char GPSFIFOOverflowRX(void);
void GPSFIFOClearOverflowRX(void);
