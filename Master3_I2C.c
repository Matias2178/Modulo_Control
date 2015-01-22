////Comunicacion I2C
	#include 	<p24Fxxxx.h>
	#include	"VarGlob.h"
 	#include	"ES_Conf.h"

	
/******************************************************************************
*	Funcion:		InitI2C3()
*	Descricpion:	Inicializa el puerto I2C numero3
*	Ingreso Datos:	Ninguno
*	Salida Datos:	Ninguno
******************************************************************************/
void InitI2C3(void)
{
	I2C3BRG = 0x0089; //100k
//	I2C3BRG = 0x0019; //400k
	
	I2C3CON = 0x1200;
	I2C3RCV = 0x0000;
	I2C3TRN = 0x0000;
	I2C3CON = 0x9200;
}

/******************************************************************************
*	Funcion:		StartRdI2C3()
*	Descricpion:	Realiza todo el procedimiento para el comienzo de la lectura
*					de la memoria en la direccion que se ingresa
*					si se pudo realizar la comuinicacion retorna verdadero 
*					si no se pudo realizar la comunicacion retorna falso
*	Ingreso Datos:	Direccion de memoria a leer
*	Salida Datos:	Verdadero o Falso
******************************************************************************/
 int StartRdI2C3(unsigned int ADDR)
{
	union _localInt Local00;

	Local00.I = ADDR;
	I2C3CONbits.PEN = 1;		//Generate Stop Condition
	while (I2C3CONbits.PEN);
	I2C3CONbits.SEN = 1;
	while (I2C3CONbits.SEN);
	I2C3TRN = 0xA0;					//Codigo de inicio de transmicion y direccion de memeoria
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	
	I2C3TRN = Local00.C.High;					//Byte alto direccion de memoria
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	I2C3TRN = Local00.C.Low;					//Byte bajo direccion de memoria
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);
	I2C3CONbits.RSEN = 1;
	while (I2C3CONbits.RSEN);	//Wait for restart	
	I2C3TRN = 0xA1;					//Codigo de inicio de transmicion y direccion de memeoria
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);	
	return 1;
}

/******************************************************************************
*	Funcion:		BufferRdI2C3()
*	Descricpion:	Lee los datos enviados desde la memoria.
*					Cuando se completo la cantidad genera la parada de la 
*					transmicion
*	Ingreso Datos:	Direccion donde se cargaran los datos leidos
*					Cantidad de datos a leer
*	Salida Datos:	Ninguno
******************************************************************************/
void BufferRdI2C3(unsigned char *pBuf,unsigned char Size)
{
	unsigned char C;
	C = Size;
	do{
		I2C3CONbits.RCEN = 1;
		Nop();
		while(!I2C3STATbits.RBF);
		*pBuf =I2C3RCV;
		 pBuf++;
		 C--;
		 if(!C){
			I2C3CONbits.ACKDT = 1;			//Set for ACk
			I2C3CONbits.ACKEN = 1;
		}
		else
		{
			I2C3CONbits.ACKDT = 0;			//Set for ACk
			I2C3CONbits.ACKEN = 1;
		}
			while(I2C3CONbits.ACKEN);		//wait for ACK to complete	
			I2C3CONbits.ACKDT = 0;
	}while(C);
}

/******************************************************************************
*	Funcion:		StartWrI2C3()
*	Descricpion:	Realiza todo el procedimiento para el comienzo de la escritura
*					de la memoria en la direccion que se ingresa
*					si se pudo realizar la comuinicacion retorna verdadero 
*					si no se pudo realizar la comunicacion retorna falso
*	Ingreso Datos:	Direccion de memoria a leer
*	Salida Datos:	Verdadero o Falso
******************************************************************************/
 int StartWrI2C3(unsigned int ADDR)
{
	union _localInt Local00;

	Local00.I = ADDR;
	HabMen = 0;
	//Genero el Start Bit
	I2C3CONbits.SEN = 1;
	while (I2C3CONbits.SEN);

	//Envio Direccion del dispositivo
	I2C3TRN = 0xA0;					//Codigo de inicio de transmicion y direccion de memeoria
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);


	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	I2C3TRN = Local00.C.High;					//Byte alto direccion de memoria
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);

	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	I2C3TRN = Local00.C.Low;					//Byt1
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);
	return 1;
}


/******************************************************************************
*	Funcion:		BufferWrI2C3()
*	Descricpion:	Escribe los datos en la memoria.
*					Cuando se completo la cantidad de datos a escribir genera 
*					la parada de la transmicion
*	Ingreso Datos:	Direccion estan los datos a ser transmitidos
*					Cantidad de datos a escribir
*	Salida Datos:	Ninguno
******************************************************************************/
void BufferWrI2C3(unsigned char *pBuf,unsigned char Size)
{
	unsigned char C;
	C = Size;
	do{
		I2C3TRN = *pBuf;					
		Nop();
		while (I2C3STATbits.TBF);
		while (I2C3STATbits.TRSTAT);
		
		pBuf++;
		C--;
		 
	}while(C);
}


/******************************************************************************
*	Funcion:		WriteI2C3()
*	Descricpion:	Escribe un solo dato en memoria
*					Espera hasta que este escrito
*					
*	Ingreso Datos:	Dato a escribir
*					
*	Salida Datos:	Ninguno
******************************************************************************/
void WriteI2C3(unsigned char Dato)
{
	I2C3TRN = Dato;					
	Nop();
	while (I2C3STATbits.TBF);
	while (I2C3STATbits.TRSTAT);
}

/******************************************************************************
*	Funcion:		WriteI2C3(void)
*	Descricpion:	Termia la escritura en el puerto I2C
*					Espera hasta que este escrito
*					
*	Ingreso Datos:	Dato a escribir
*					
*	Salida Datos:	Ninguno
******************************************************************************/
void StopI2C3(void)
{
	//Envio el fin de transimicion
	I2C3CONbits.PEN = 1;
	while (I2C3CONbits.PEN);
	HabMen = 1;
}
