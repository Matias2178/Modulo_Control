/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
	#include 	<p24Fxxxx.h>
  	#include	"VarGlob.h"
  	#include	"Constantes.h"


#define BUFFER_SIZE 257
//#define INPUT_REG   U3TXREG
#define OTPUT_REG   U4TXREG
//#define FIFO_INT    INT_U4RX

#define buffer U4FIFOBufferTX
#define cursor U4FIFOCursorTX
#define pos    U4FIFOPosTX
#define flags  U4FIFOFlagsTX
#define FIFOInit  U4FIFOInitTX
#define FIFOWrite U4FIFOWRITETX
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

#ifndef BUFFER_SIZE
    #error "BUFFER_SIZE no definido"
#endif
//#ifndef INPUT_REG
#ifndef OTPUT_REG
    #error "INPUT_REG no definido"
#endif
#ifndef buffer
    #error "buffer no definido"
#endif
#ifndef cursor
    #error "cursor no definido"
#endif
#ifndef pos
    #error "pos no definido"
#endif
#ifndef flags
    #error "flags no definido"
#endif
#ifndef FIFOInit
    #error "FIFOInit no definido"
#endif
#ifndef FIFOWrite
    #error "FIFOISR no definido"
#endif
#ifndef FIFORead
    #error "FIFORead no definido"
#endif
#ifndef FIFOEmpty
    #error "FIFOEmpty no definido"
#endif
#ifndef FIFOFull
    #error "FIFOFull no definido"
#endif
#ifndef FIFOOverflow
    #error "FIFOOverflow no definido"
#endif
#ifndef FIFOClearOverflow
    #error "FIFOClearOverflow no definido"
#endif

unsigned char  buffer[BUFFER_SIZE]__attribute__ ((far));
unsigned int cursor;
unsigned int pos;
union {
    struct {
        unsigned empty:1;
        unsigned overflow:1;
        unsigned savegie:1;
        unsigned transmit:1;
    } bits;
    unsigned char val;
} flags;


void FIFOInit (void) {

    cursor = 0;
    pos = 0;
    flags.val = 0x00;
    flags.bits.empty = 1;

}
int FIFOWrite(unsigned char VALUE){

    if (cursor == pos){
        if (!flags.bits.empty){
            char dummy;
            dummy = VALUE;
            flags.bits.overflow = 1;
            return 1;
        }
    }
    
	    buffer[cursor++] = VALUE;
 	   	flags.bits.empty = 0;
 	   	if(!flags.bits.transmit)
 	   	{
 	   		flags.bits.transmit = 1;
 	   		OTPUT_REG = VALUE;	
 	   	}		

    if (cursor == BUFFER_SIZE)
        cursor = 0;
	return 0;
}

unsigned char FIFORead(void){
 //   unsigned char val;
	do{
	pos++;
    if (flags.bits.empty)
        return 0;

    #if defined(FIFO_INT)
    INTEnable(FIFO_INT, INT_DISABLED);
    #endif


    if (pos == BUFFER_SIZE)
        pos = 0;

    if (cursor == pos)
    {
        flags.bits.empty = 1;
        flags.bits.transmit = 0;
        return;
    }
    else
    {
    	OTPUT_REG = buffer[pos];
    }

    #if defined(FIFO_INT)
    INTEnable(FIFO_INT, INT_ENABLED);
    #endif
  }while(!U4STAbits.UTXBF);
  //  return val;
}

unsigned char FIFOEmpty(void){
    return flags.bits.empty;
}

unsigned char FIFOFull(void){
    if (flags.bits.empty)
        return 0;

    if (cursor == pos)
        return 1;
    else
        return 0;

}

unsigned char FIFOOverflow(void){
    return flags.bits.overflow;
}

void FIFOClearOverflow(void){
    flags.bits.overflow = 0;
}

