/******************************************************************************
*
*
*
*
*
*
*
*
*
******************************************************************************/
#include "Correccion.h"

void Correccion(float Lat, float Lon, float Dly)
{

	
	LatAct = Lat - LatAnt;
	LonAct = Lon - LonAnt;
	
	LatAct = LatAct * Dly;
	LonAct = LonAct * Dly;
	
	LatAct = LatAct + Lat;
	LonAct = LonAct + Lon;
	
	LatAnt = Lat;
	LonAnt = Lon;
}
	 