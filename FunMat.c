/*****************************************************************************
 *	FUNCIONES MATEMATICAS VARIAS
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
	#include	"VarGlob.h"
	#include	"Constantes.h"
/******************************************************************************
*	Funcion:		itos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, segun la candidad de digitos que se indican 
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*					Cantidad de digitos a imprimir
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char *itos(unsigned int Valor,unsigned char *S, unsigned int Digitos)
{
	unsigned long int Dec = 10;
	int i;
	for(i=1;i<Digitos;i++)
	{
		Dec = Dec * 10;
	}
	Valor = Valor - (Valor/Dec)*Dec;
	Dec = Dec / 10;
	for(i=Digitos;i;i--)
	{
		*S = (Valor/Dec+'0');
		
    	Valor = Valor - (Valor/Dec)*Dec;
    	Dec = Dec / 10;
		S++;
	}
	return (unsigned char *)S;
}
/******************************************************************************
*	Funcion:		ditos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, segun la candidad de digitos que se indican 
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*					Cantidad de digitos a imprimir
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char *ditos(unsigned int Valor,unsigned char *S, unsigned int Digitos, unsigned int Decimales)
{
	Decimales ++;
	unsigned long int Dec = 10;
	int i;
	for(i=1;i<Digitos;i++)
	{
		Dec = Dec * 10;
	}
	Valor = Valor - (Valor/Dec)*Dec;
	Dec = Dec / 10;
	Digitos ++;
	for(i=Digitos;i;i--)
	{
		if(i==Decimales)
		{
			*S = '.';
			S++;
			continue;
		}
		*S = (Valor/Dec+'0');
		
    	Valor = Valor - (Valor/Dec)*Dec;
    	Dec = Dec / 10;
		S++;
	}
	return (unsigned char *)S;
}
///******************************************************************************
//*	Funcion:		itos2()
//*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
//*					array, segun la candidad de digitos que tiene la variable
//*	Ingreso Datos:	El valor entrero a convertir
//*					Puntero al array a cargar
//*					
//*	Salida Datos:	Puntero a la posicion siguiente en el array
//******************************************************************************/	
//unsigned char *itos2(unsigned int Valor,unsigned char *S)
//{
//	unsigned long int Dec = 10;
//	int i;
//	int ind = 10;
//	unsigned char Aux[10];
//	
//	while(Dec <= Valor)
//	{
//		Aux[ind] = Valor % Dec;
//		ind--;
//		Valor /=Dec;
//	}
//	Aux[ind] = Valor;
//	while (ind < 11)
//	{
//		*S = Aux[ind]+ '0';
//		ind++;
//		S++;
//	}	
//
//	return (unsigned char *)S;
//}
/******************************************************************************
*	Funcion:		sitos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, segun la candidad de digitos que se indican 
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*					Cantidad de digitos a imprimir
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char * sitos(int Valor,unsigned char *S)
{
	unsigned long int Dec = 10;
	int i;
	int a,b,c;
	
	if(!Valor)
	{
		b=1;
		goto ImpCero;
	}
	
	if(Valor < 0)
	{
		*S = '-';
		S++;
		Valor = Valor * -1;
	}
	b=1;
	i= Valor;
	Dec = 1;
	do
	{
		if(!(i = i/10))
			break;
		Dec = Dec * 10;
		b++;
	}while(i>0);
	do{
		
		i = (Valor/Dec);
		*S = i + '0';
		a = i *Dec;
		Valor = Valor - (int)a;
		Dec = Dec / 10;
		S++;
		b--;
	}
	while(Valor);
ImpCero:
	while(b)
	{
		*S = '0';
		S++;
		b--;
	}	
		
	return (unsigned char *)S;
}
/******************************************************************************
*	Funcion:		uitos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, imprime la canditad de digitos que hay
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char * uitos(unsigned int Valor, unsigned char *S)
{
	unsigned int Dec;
	unsigned int i;
	unsigned int a;
	int b,c;
	
	if(!Valor)
	{
		b=1;
		goto ImpCero;
	}
	
	if(Valor < 0)
	{
		*S = '-';
		S++;
		Valor = Valor * -1;
	}
	b=1;
	i= Valor;
	Dec = 1;
	do
	{
		if(!(i = i/10))
			break;
		Dec = Dec * 10;
		b++;
	}while(i>0);
	do{
		i = (Valor/Dec);
		*S = i + '0';
		a = i *Dec;
		Valor = Valor - a;
		Dec = Dec / 10;
		S++;
		b--;
	}
	while(Valor);
ImpCero:
	while(b>0)
	{
		*S = '0';
		S++;
		b--;
	}	
		
	return  (unsigned char *)S;
}
/******************************************************************************
*	Funcion:		sitos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, imprime la canditad de digitos que hay
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char * ultos(unsigned long Valor, unsigned char *S)
{
	unsigned long int Dec;
	unsigned long i;
	unsigned long a;
	int b,c;
	
	if(!Valor)
	{
		b=1;
		goto ImpCero;
	}
	
	if(Valor < 0)
	{
		*S = '-';
		S++;
		Valor = Valor * -1;
	}
	b=1;
	i= Valor;
	Dec = 1;
	do
	{
		if(!(i = i/10))
			break;
		Dec = Dec * 10;
		b++;
	}while(i>0);
	do{
		i = (Valor/Dec);
		*S = i + '0';
		a = i *Dec;
		Valor = Valor - a;
		Dec = Dec / 10;
		S++;
		b--;
	}
	while(Valor);
ImpCero:
	while(b>0)
	{
		*S = '0';
		S++;
		b--;
	}	
		
	return (unsigned char*)S;
}

/******************************************************************************
*	Funcion:		sitos()
*	Descricpion:	Convierte un enteros en caracteres ASCCII y los carga en 
*					array, imprime la canditad de digitos que hay
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char * ultos10d(unsigned long Valor, unsigned char *S)
{
	unsigned long int Dec = 10;
	int i;
	int ind = 9;
	unsigned char Aux[10]="0000000000";
//	Aux = 
	
	while(Dec <= Valor)
	{
		Aux[ind] = (Valor % Dec) + '0';
		ind--;
		Valor /=Dec;
	}
	Aux[ind] = Valor  + '0';
	ind = 0;
	while (ind < 10)
	{
		*S = Aux[ind];
		ind++;
		S++;
	}	

	return (unsigned char *)S;
	
}

/******************************************************************************
*	Funcion:		ArrtoHex()
*	Descricpion:	Lee dos caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo char
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor en Hexa
******************************************************************************/	
unsigned char ArrtoHex(unsigned char *P)
{
	unsigned char i;
	Proceso.B.fDataErr = false;
	if(*P>='A' && *P<='F')
	{
		i = *P - 'A'+ 10;
	}
	else if (*P>='0' && *P<='9')
	{
		i = *P - '0';
	}
	else if(*P>='a' && *P<='f')
	{
		i = *P - 'a'+ 10;
	}
	else
	{
		Proceso.B.fDataErr = true;
		i= 0;
	}
	i<<=4;
	P++;
	if(*P>='A' && *P<='F')
	{
		i = i + (*P - 'A'+ 10);
	}
	
	else if (*P>='0' && *P<='9')
	{
		i = i + (*P - '0');
	}
	else if(*P>='a' && *P<='f')
	{
		i = i + (*P - 'a'+ 10);
	}
	else
	{
		Proceso.B.fDataErr = true;
		i= 0;
	}
	return i;
}

/******************************************************************************
*	Funcion:		ArrtoLongHex(unsigned char *P)
*	Descricpion:	Lee 8 caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo Long
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor long en Hexa
******************************************************************************/	
unsigned long ArrtoLongHex(unsigned char *P)
{
	unsigned char i;
	unsigned long Dato = 0;
	Proceso.B.fDataErr = false;
	for(i=0;i<8;i++)
	{
		Dato<<=4;
		if(*P>='A' && *P<='F')
		{
			Dato = Dato + (*P - 'A'+ 10);
		}
	
		else if (*P>='0' && *P<='9')
		{
			Dato = Dato + (*P - '0');
		}
		else if(*P>='a' && *P<='f')
		{
			Dato = Dato + (*P - 'a'+ 10);
		}
		else
		{
			Proceso.B.fDataErr = true;
			Dato= 0;
		}
		P++;
	}
	return Dato;
}
/******************************************************************************
*	Funcion:		ArrtoLongHex(unsigned char *P)
*	Descricpion:	Lee 8 caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo Long
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor long en Hexa
******************************************************************************/	
unsigned int ArrtoLongHex2(unsigned char *P,unsigned char *Dato)
{
	unsigned char i;
	unsigned char a;
//	*Dato = 0;
	Dato = Dato + 3;
	for(i=0;i<4;i++)
	{
		a = 0;
		if(*P>='A' && *P<='F')
		{
			a = *P - 'A'+ 10;
		}
		else if (*P>='0' && *P<='9')
		{
			a = *P - '0';
		}
		else if(*P>='a' && *P<='f')
		{
			a = *P - 'a'+ 10;
		}
		else
		{
			a= 0;
		}
		a<<=4;
		P++;
		if(*P>='A' && *P<='F')
		{
			a = a + (*P - 'A'+ 10);
		}
	
		else if (*P>='0' && *P<='9')
		{
			a = a + (*P - '0');
		}
		else if(*P>='a' && *P<='f')
		{
			a = a + (*P - 'a'+ 10);
		}
		else
		{
	
			a= 0;
		}
		*Dato = a;
		Dato--;
		P++;
		 
	}
	return i;
}
/******************************************************************************
*	Funcion:		HextoArr()
*	Descricpion:	Lee dos caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo char
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor en Hexa
******************************************************************************/	
void HextoArr(unsigned char *D, unsigned char *P,unsigned int Cant)
{
	unsigned char Dato;
	unsigned int i;
	Checksum += *D;
	for(i=Cant;i;i--)
	{
		Dato = *D>>4;
		if(Dato>9)
		{
			Dato = Dato - 10;
			*P = Dato + 'a';
		}
		else
		{
			*P = Dato + '0';
		}
		P++;
		Dato = *D & 0x0F;
		if(Dato>9)
		{
			Dato = Dato - 10;
			*P = Dato + 'a';
		}
		else
		{
			*P = Dato + '0';
		}
		P++;
		D++;
	}
}
/******************************************************************************
*	Funcion:		InHextoArr()
*	Descricpion:	Lee dos caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo char
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor en Hexa
******************************************************************************/	
unsigned char *InHextoArr(unsigned int D, unsigned char *P)
{
	unsigned char Dato;
	unsigned int i;
	unsigned int a;
	for(i=16;i;i=i-4)
	{
		a = i - 4;
		Dato = D>>a;
		Dato = Dato & 0x0F;
		if(Dato>9)
		{
			Dato = Dato - 10;
			*P = Dato + 'a';
		}
		else
		{
			*P = Dato + '0';
		}
		P++;
	}
	return (unsigned char *)P; 
}
/******************************************************************************
*	Funcion:		LongHextoArr()
*	Descricpion:	Lee dos caracters en ASCII que corresponden a un valor en
*					Hexa y lo devuelve en una variable tipo char
*
*	Ingreso Datos:	Puntero al array donde estan los caracteres
*	Salida Datos:	Valor en Hexa
******************************************************************************/	
unsigned char *LongHextoArr(unsigned long D, unsigned char *P)
{
	unsigned long Dato;
	unsigned int i;
	unsigned long a;
	for(i=32;i;i=i-4)
	{
		Dato = 0;
		a = i - 4;
		Dato = D>>a;
		Dato = Dato & 0x0F;
		if(Dato>9)
		{
			Dato = Dato - 10;
			*P = Dato + 'a';
		}
		else
		{
			*P = Dato + '0';
		}
		P++;
	}
	return (unsigned char *)P; 
}
/******************************************************************************
*	Funcion:		ftos()
*	Descricpion:	Convierte un flotante en caracteres ASCCII y los carga en 
*					array, segun la candidad de digitos que se indican 
*	Ingreso Datos:	El valor entrero a convertir
*					Puntero al array a cargar
*					Cantidad de digitos a imprimir
*	Salida Datos:	Puntero a la posicion siguiente en el array
******************************************************************************/	
unsigned char *ftos(float Valor,unsigned char *S, unsigned int Ent,unsigned int Dec)
{
	unsigned long int dig = 1;
	unsigned long int Num;
	int i;
	for(i=1;i<=Dec;i++)
	{
		dig = dig * 10;
	}
	Num =  Valor * dig;
	
	for(i=1;i<=Ent;i++)
	{
		dig = dig * 10;
	}	
	
	Num = Num - (Num/dig)*dig;
	dig = dig / 10;
	
	for(i=Ent;i;i--)
	{
		*S = (Num/dig+'0');
		
    	Num = Num - (Num/dig)*dig;
    	dig = dig / 10;
		S++;
	}
	*S = '.';
	S++;
	for(i=Dec;i;i--)
	{
		*S = (Num/dig+'0');
		
    	Num = Num - (Num/dig)*dig;
    	dig = dig / 10;
		S++;
	}	
	return   S;
}