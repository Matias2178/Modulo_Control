/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
//#include "Compiler.h"


#define BUFFER_SIZE 2048
#define INPUT_REG   U2RXREG

#define buffer WifiFIFOBufferRX
#define cursor WifiFIFOCursorRX
#define pos    WifiFIFOPosRX
#define flags  WifiFIFOFlagsRX
#define FIFOInit  WifiFIFOInitRX
#define FIFOISR   WifiFIFOISRRX
#define FIFORead  WifiFIFOReadRX
#define FIFOEmpty WifiFIFOEmptyRX
#define FIFOFull  WifiFIFOFullRX
#define FIFOOverflow  WifiFIFOOverflowRX
#define FIFOClearOverflow  WifiFIFOClearOverflowRX

/* Prototipos */
void WifiFIFOInitRX (void);
int WifiFIFOISRRX(void);
unsigned char WifiFIFOReadRX(void);
unsigned char WifiFIFOEmptyRX(void);
unsigned char WifiFIFOFullRX(void);
unsigned char WifiFIFOOverflowRX(void);
void WifiFIFOClearOverflowRX(void);
