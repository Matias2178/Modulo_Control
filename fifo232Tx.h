/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
//#include "Compiler.h"


#define BUFFER_SIZE 1536
//#define INPUT_REG   U2RXREG
#define OTPUT_REG   U2TXREG

#define buffer RSFIFOBufferTX
#define cursor RSFIFOCursorTX
#define pos    RSFIFOPosTX
#define flags  RSFIFOFlagsTX
#define FIFOInit  RSFIFOInitTX
#define FIFOISR   RSFIFOISRTX
#define FIFORead  RSFIFOReadTX
#define FIFOEmpty RSFIFOEmptyTX
#define FIFOFull  RSFIFOFullTX
#define FIFOOverflow  RSFIFOOverflowTX
#define FIFOClearOverflow  RSFIFOClearOverflowTX

/* Prototipos */
void RSFIFOInitTX (void);
int RSFIFOISRTX(void);
unsigned char RSFIFOReadTX(void);
unsigned char RSFIFOEmptyTX(void);
unsigned char RSFIFOFullTX(void);
unsigned char RSFIFOOverflowTX(void);
void RSFIFOClearOverflowTX(void);
