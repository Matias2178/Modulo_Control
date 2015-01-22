/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
//#include "Compiler.h"


#define BUFFER_SIZE 2048
#define INPUT_REG   U2RXREG

#define buffer U4FIFOBufferTX
#define cursor U4FIFOCursorTX
#define pos    U4FIFOPosTX
#define flags  U4FIFOFlagsTX
#define FIFOInit  U4FIFOInitTX
#define FIFOISR   U4FIFOISRTX
#define FIFORead  U4FIFOReadTX
#define FIFOEmpty U4FIFOEmptyTX
#define FIFOFull  U4FIFOFullTX
#define FIFOOverflow  U4FIFOOverflowTX
#define FIFOClearOverflow  U4FIFOClearOverflowTX

/* Prototipos */
void U4FIFOInitTX (void);
int U4FIFOISRTX(void);
unsigned char U4FIFOReadTX(void);
unsigned char U4FIFOEmptyTX(void);
unsigned char U4FIFOFullTX(void);
unsigned char U4FIFOOverflowTX(void);
void U4FIFOClearOverflowTX(void);
