/*****************************************************************************
 *	FUNCIONES DE ACONDICIONAMIENTO DE DATOS EN LOS BUFFER DE TRANSMICION
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
 extern void DtoTerminal(void);
 extern void Encabezado(char *lb,unsigned char *S);
 extern void Diagnostico(char *lb,unsigned char *S);
 
 extern void MedSensores (char *lb, struct _DtsCom Dts,unsigned char *S,int T);
 extern void StsSensores (char *lb, struct _DtsCom Datos,unsigned char *S,int T);
 
 extern void MedPerifericos (char *lb, struct _DtsPerCom Datos,unsigned char *S,int T);
 extern void StsPerifericos (char *lb, struct _DtsPerCom Datos,unsigned char *S,int T);
 
 extern void GPSDatos(char *lb,unsigned char *S);
 extern void GPSSatGan(char *lb,unsigned char *S);  
 
 extern void TmrBusLin (char *lb, struct _TLin Tmp,unsigned char *S, char Disp, char Act, char Err);
 
 extern void CRNL(unsigned char *S);
 //extern unsigned char * Dispositivo(unsigned char *S,unsigned char Id)

 extern void CheckGrabaMem(char *lb,unsigned int ,unsigned char *S); 
 
 extern void TxCheckSum(unsigned char * S);
 