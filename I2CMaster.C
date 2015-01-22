///

	#include 	<p24Fxxxx.h>
/*********************************************************************
* Function:        InitI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Initialises the I2C(1) peripheral
*
* Note:			Sets up Master mode, No slew rate control, 100Khz
********************************************************************/
void Master3_InitI2C(void)
{	
	//This function will initialize the I2C(1) peripheral.
	//First set the I2C(1) BRG Baud Rate.

	//Baud Rate.

	I2C3BRG = 0x0088; //100k
//	I2C3BRG = 0x0022; //400k

	//Now we will initialise the I2C peripheral for Master Mode, No Slew Rate
	//Control, and leave the peripheral switched off.
	
	I2C3CON = 0x1200;
	//Reset de los registros de tranmicion y recpcion
	I2C3RCV = 0x0000;
	I2C3TRN = 0x0000;
	//Now we can enable the peripheral
	I2C3CON = 0x9200;
}


char StartRdM3_I2C(unsigned int ADDR)
{
	unsigned char HigAddr, LowAddr;
	
	LowAddr = ADDR & 0x00FF;
	HigAddr = (ADDR & 0xFF00)>> 8;
	
	//Genero el Start Bit
	I2C3CONbits.SEN = 1;
	while (I2C3CONbits.SEN);
	//Envio Direccion del dispositivo
	I2C3TRN = 0x00;					//Codigo de inicio de transmicion y direccion de memeoria
	while (I2C3STATbits.TBF);
	Dly_1_useg();
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	I2C3TRN = HigAddr;					//Byte alto direccion de memoria
	while (I2C3STATbits.TBF);
	Dly_1_useg();
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	I2C3TRN = LowAddr;					//Byte bajo direccion de memoria
	while (I2C3STATbits.TBF);
	Dly_1_useg();
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion
	//Envio el ReStart
	I2C3CONbits.RSEN = 1;
	Dly_1_useg();
	I2C3TRN = 0xA1;					//Codigo de inicio de transmicion y direccion de memeoria
	while (I2C3STATbits.TBF);
	Dly_1_useg();
	if(I2C3STATbits.ACKSTAT)
		return 0;					//Error en la comunicacion	
	return 1;
}
/*********************************************************************
* Function:        StartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an I2C Start Condition
*
* Note:			None
********************************************************************/
void Master3_StartI2C(void)
{
	//This function generates an I2C start condition and returns status 
	//of the Start.

	I2C3CONbits.SEN = 1;		//Generate Start COndition
	while (I2C3CONbits.SEN){
		asm("CLRWDT");	//Wait for Start COndition
		}
	//return(I2C1STATbits.S);	//Optionally return status
}
/*********************************************************************
* Function:        RestartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a restart condition and optionally returns status
*
* Note:			None
********************************************************************/
void Master3_RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status 
	//of the Restart.

	I2C3CONbits.RSEN = 1;		//Generate Restart		
	while (I2C3CONbits.RSEN);	//Wait for restart	
	//return(I2C1STATbits.S);	//Optional - return status
}

/*********************************************************************
* Function:        StopI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a bus stop condition
*
* Note:			None
********************************************************************/
void Master3_StopI2C(void)
{
	//This function generates an I2C stop condition and returns status 
	//of the Stop.

	I2C3CONbits.PEN = 1;		//Generate Stop Condition
	while (I2C3CONbits.PEN);	//Wait for Stop
	//return(I2C1STATbits.P);	//Optional - return status
}
/*********************************************************************
* Function:        WriteI2C()
*
* Input:		Byte to write.
*
* Output:		None.
*
* Overview:		Writes a byte out to the bus
*
* Note:			None
********************************************************************/
void Master3_WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	I2C3TRN = byte;					//Load byte to I2C1 Transmit buffer
	while (I2C3STATbits.TBF);		//wait for data transmission

}
/*********************************************************************
* Function:        IdleI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Waits for bus to become Idle
*
* Note:			None
********************************************************************/
void Master3_IdleI2C(void)
{
	while (I2C3STATbits.TRSTAT);		//Wait for bus Idle
}
/*********************************************************************
* Function:        getI2C()
*
* Input:		None.
*
* Output:		contents of I2C1 receive buffer.
*
* Overview:		Read a single byte from Bus
*
* Note:			None
********************************************************************/
unsigned int Master3_getI2C(void)
{
	I2C3CONbits.RCEN = 1;			//Enable Master receive
	Nop();
	while(!I2C3STATbits.RBF){asm("CLRWDT");}		//Wait for receive bufer to be full
	return(I2C3RCV);				//Return data in buffer
}
/*********************************************************************
* Function:        NotAckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a NO Acknowledge on the Bus
*
* Note:			None
********************************************************************/
void Master3_NotAckI2C(void)
{
	I2C3CONbits.ACKDT = 1;			//Set for NotACk
	I2C3CONbits.ACKEN = 1;
	while(I2C3CONbits.ACKEN);		//wait for ACK to complete
	I2C3CONbits.ACKDT = 0;			//Set for NotACk
}


/*********************************************************************
* Function:        AckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an Acknowledge.
*
* Note:			None
********************************************************************/
void Master3_AckI2C(void)
{
	I2C3CONbits.ACKDT = 0;			//Set for ACk
	I2C3CONbits.ACKEN = 1;
	while(I2C3CONbits.ACKEN);		//wait for ACK to complete
}