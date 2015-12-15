/*****************************************************************************
 *	 FUNCIONES DE USO GENERAL 
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - 2013
 *	
 *****************************************************************************/
 	#include 	<p24Fxxxx.h>
	#include	<string.h>
	#include <stdio.h>
	#include <Math.h>
	#include	"VarGlob.h"
//	#include 	"ES_Conf.h"
//	#include 	"UART.h"
//	#include 	"GPS.h"
//	#include 	"Comandos.h"
	#include	"Constantes.h"

	#define PI 3.14159265358979323846
 
double Distance(double lat1, double lon1, double lat2, double lon2, char unit) {
    double deg2radMultiplier = PI / 180;
    lat1 = lat1 * deg2radMultiplier;
    lon1 = lon1 * deg2radMultiplier;
    lat2 = lat2 * deg2radMultiplier;
    lon2 = lon2 * deg2radMultiplier;
 
    double radius = 6378.137; // earth mean radius defined by WGS84
    double dlon = lon2 - lon1;
    double distance = acos( sin(lat1) * sin(lat2) +  cos(lat1) * cos(lat2) * cos(dlon)) * radius; 
    
    if (unit == 'K') {
        return (distance); 
    } else if (unit == 'M') {
        return (distance * 0.621371192);
    } else if (unit == 'N') {
        return (distance * 0.539956803);
    } else {
        return 0;
    }
}
 char Destello(int Sec,char Duty)
 {
	 static char Dty;
	 static int S;
	 
	 if (!S)
	 {
	 	S = 1;
	 	Dty = Duty;
	 }
	 if(!Dty)
	 {
		 Dty = Duty;
		 S= S<<1;
	 }
	 else
	 	Dty--;
	 	
	 if(Sec & S)
		 return true;
	
	 return false;
 }
 
   char Blink(struct _Destello *Dest)
 {
	 if (!Dest->S)
	 {
	 	Dest->S = 1;
	 	Dest->Dty = Dest->Duty;
	 }
	 if(!Dest->Dty)
	 {
		 Dest->Dty = Dest->Duty;
		 Dest->S= Dest->S<<1;
	 }
	 else
	 	Dest->Dty--;
	 	
	 if(Dest->Sec & Dest->S)
		 return true;
	
	 return false;
 }