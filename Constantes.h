//Constantes del sistema

//-------------------------------------------------------------
//	DE LA CPU

	#define	XTAL1			7372800
	#define	XTAL2			32768
	
	#define	INTCLK			XTAL1	
	  	
//****************************************************************************
//****************************************************************************
//		DEFINICION DE CONSTANTES
//****************************************************************************
//****************************************************************************

//Cantidad Maxima de Dispositivos

	#define kModMax		16
	#define kROTMax		8
	#define kTRBMax		3
	#define kTOLMax		16
	#define kPREMax		9
	#define kMaxEscan 	5
	
//-----------------------------------------------------------------------------
//Constantes del sistema

	#define SoftVerH	1
	#define SoftVerL	6
	#define Relase		2

	#define FVerD		24
	#define FVerM		5
	#define FVerA		2017

	
	#define true	1
	#define True	1
	#define TRUE	1
	#define false	0
	#define False	0
	#define FALSE	0
	#define null	0
	#define FPos	0
	#define FNeg	1


//Parametros de configuracion de sensores de semillas	
	#define k_GruesaTM	50		//Tiempo de medicion	(Gruesa)
	#define k_GruesaTD	20		//Tiempo Stop deteccion	(Gruesa)
	#define k_GruesaCS	5		//Cantidad Start medicion	(Gruesa)
	
	#define k_FinaTM	20		//Tiempo de medicion (Fina / Fertilizante)
	#define k_FinaTD	15		//Tiempo Stop deteccion (Fina / Fertilizante)
	#define k_FinaCS	10		//Cantidad Start medicion	(Fina / Fertilizante)
	
	#define kTempAlTRB 	3		//Lecturas antes de generar o apagar la alarma de turbina
	#define kTempAlPRE	3		//Lecturas antes de generar o apagar la alarma de presion
	
	#define k_MEM_CHECK 53
	
	#define Near __attribute__((near))
	
//-----------------------------------------------------------------------------
//Textos de informacion del modulo
	#define kIdentificacion	"CAS5000 CON MODULO WIFI Y TODO VER:1.0.082013 SN:"
	#define kCopyRight "Modulo de Propiedad de SIID SRL www.controlagro.com.ar en caso de problemas llamar a los tecnicos"
