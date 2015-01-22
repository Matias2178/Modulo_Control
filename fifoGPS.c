/*
 * File:   fifoFT.c
 * Author: Gonzalo Slaboch - Acronex
 *
 * Created on 18 de julio de 2011, 10:48
 */
	#include 	<p24Fxxxx.h>
  	#include	"VarGlob.h"
  	#include	"Constantes.h"


#define BUFFER_SIZE 1024
#define INPUT_REG   U1RXREG
//#define FIFO_INT    INT_U4RX

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

#ifndef BUFFER_SIZE
    #error "BUFFER_SIZE no definido"
#endif

#ifndef INPUT_REG
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
#ifndef FIFOISR
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

unsigned char buffer[BUFFER_SIZE] __attribute__ ((far));
unsigned int cursor;
unsigned int pos;
union {
    struct {
        unsigned empty:1;
        unsigned overflow:1;
        unsigned savegie:1;
    } bits;
    unsigned char val;
} flags;


void FIFOInit (void) {

    cursor = 0;
    pos = 0;
    flags.val = 0x00;
    flags.bits.empty = 1;

}

int FIFOISR(void){
char valor;
    if (cursor == pos){
        if (!flags.bits.empty){
            char dummy;
            dummy = INPUT_REG;
        //   U4TXREG = dummy;
            flags.bits.overflow = 1;
            return 1;
        }
    }
	valor =INPUT_REG;
    buffer[cursor++] = valor;
 //   U4TXREG = valor;
    flags.bits.empty = 0;

    if (cursor == BUFFER_SIZE)
        cursor = 0;

	return 0;
}

unsigned char FIFORead(void){
    unsigned char val;

    if (flags.bits.empty)
        return 0;

    #if defined(FIFO_INT)
    INTEnable(FIFO_INT, INT_DISABLED);
    #endif
    val = buffer[pos++];

    if (pos == BUFFER_SIZE)
        pos = 0;

    if (cursor == pos)
        flags.bits.empty = 1;

    #if defined(FIFO_INT)
    INTEnable(FIFO_INT, INT_ENABLED);
    #endif
    return val;
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

