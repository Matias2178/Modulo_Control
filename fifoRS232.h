/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
//#include "Compiler.h"


#define BUFFER_SIZE 1024
#define INPUT_REG   U2RXREG

#define buffer RSFIFOBufferRX
#define cursor RSFIFOCursorRX
#define pos    RSFIFOPosRX
#define flags  RSFIFOFlagsRX
#define FIFOInit  RSFIFOInitRX
#define FIFOISR   RSFIFOISRRX
#define FIFORead  RSFIFOReadRX
#define FIFOEmpty RSFIFOEmptyRX
#define FIFOFull  RSFIFOFullRX
#define FIFOOverflow  RSFIFOOverflowRX
#define FIFOClearOverflow  RSFIFOClearOverflowRX

/* Prototipos */
void RSFIFOInitRX (void);
int RSFIFOISRRX(void);
unsigned char RSFIFOReadRX(void);
unsigned char RSFIFOEmptyRX(void);
unsigned char RSFIFOFullRX(void);
unsigned char RSFIFOOverflowRX(void);
void RSFIFOClearOverflowRX(void);
