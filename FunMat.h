 /*****************************************************************************
 *	FUNCIONES MATEMATICAS VARIAS
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - 2013
 *	
 *****************************************************************************/
 
 extern unsigned char *itos(unsigned int Valor,unsigned char *S, unsigned int Digitos);
// extern unsigned char *itos2(unsigned int Valor,unsigned char *S);
 extern unsigned char *sitos(int Valor, unsigned char *S);
 extern unsigned char *ultos(unsigned long Valor,unsigned char *S);
 extern unsigned char * ultos10d(unsigned long Valor, unsigned char *S);
 extern unsigned char *uitos(unsigned int Valor,unsigned char *S);
 extern unsigned char *ftos(float Valor,unsigned char *S, unsigned int Ent,unsigned int Dec);
 extern unsigned char ArrtoHex(unsigned char *P);
 extern unsigned long ArrtoLongHex(unsigned char *P);
 extern void HextoArr(unsigned char D, unsigned char *P,unsigned int Cant);
 extern unsigned char *InHextoArr(unsigned int D, unsigned char *P);
 extern unsigned char *LongHextoArr(unsigned long D, unsigned char *P);
 extern unsigned long ArrtoLongHex2(unsigned char *P,unsigned long Dato);