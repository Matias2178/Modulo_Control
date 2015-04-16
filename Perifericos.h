/*****************************************************************************
 *	DECLARACION DE FUNCIONES PARA EL MANEJO DE TODOS LOS SENSORES EN 
 *  TODOS LOS BUSES DE TRABAJO
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 
 *	FECHA		:	2013
 *	
 *	ARCHIVOS QUE MANEJA
 *		SENSORES.c -> Manejo de los sensores de siembra y fertilizante
 *****************************************************************************/
 //----------------------------------------------------------------------------
 //	Sensores.c
 // Manejo de los sensores de semillas
 extern void Med_Sen_Bus1(unsigned char Id,unsigned int Med);
 extern void Bus1aDtsCom(void);
  
  
 extern void Med_Sen_Bus2(unsigned char Id,unsigned int Med);
 extern void Bus2aDtsCom(void);
 
 extern void CargaConfSen(void);
 extern void GuardaConfSen(void);
 
 extern void CargaId(void);
 extern void MaxMinId(unsigned char Id);
 //----------------------------------------------------------------------------
 //Manejo de los sensores 
 extern double ADQ_AjAltaDensFact(unsigned long CantSem,unsigned char TimMed);
 extern unsigned long ADQ_AjAltaDensValue(unsigned long CantSem,double Factor);
 
 //----------------------------------------------------------------------------
 //Manejo de los sensores 
 extern void Mod2DtsCom(void);
 extern void ROT2DtsCom(void);
 extern void TRB2DtsCom(void);
 extern void TOL2DtsCom(void);
 
 extern void CargaConfPer(void);
 extern void GrabaConfPer(void);
 extern void GrabaConfMod(void);
 extern void GrabaConfROT(void);
 extern void GrabaConfRPM(void);
 extern void GrabaConfTol(void);


//----------------------------------------------------------------------------
//	StartUp.c
// Sensores de Semilla y Fertilizante 
 extern void SenStart000(void);
 extern void SenTuboSucio(void);
 extern void SenRedetecion(void);
  
//----------------------------------------------------------------------------
//	RPM_Rot.c
 extern void RotStart000(void);
 extern char RotLecPar00(unsigned char ID);
 extern char RotEscPar00(unsigned char ID);
 
 extern void TRBStart000(void);
 extern char TRBLecPar00(unsigned char ID);
 extern char TRBEscPar00(unsigned char ID);
 extern char TRBChekAl00(unsigned char ID); 
 
 extern void TOLStart000(void);
 
//----------------------------------------------------------------------------
//	Moduladora.c
 extern void ModStart000(void);
 extern char ModAjtSPKD0(unsigned char ID);
 extern char ModLecSPKD0(unsigned char ID);
 extern char ModConfPul0(unsigned char ID);
 extern char ModLectPul0(unsigned char ID);
 extern char ModConfPulKm(unsigned char ID);
 extern char ModLectPulKm(unsigned char ID);
 extern char ModConfParam(unsigned char ID);
 extern char ModLectParam(unsigned char ID);
 extern char ModDirId(unsigned char ID);
 extern char ModIdNum (unsigned char ID);
 extern char ModSistKpKd(unsigned char ID,unsigned int KP, unsigned int KD, unsigned char Esc);
 

 
 