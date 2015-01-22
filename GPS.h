/*****************************************************************************
 *	VARIABLES ESTRUCTURAS Y FUNCIONES PARA LA LECTURA DEL GPS
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00 - MAYO 2013
 *	
 *****************************************************************************/
 #define kMsgDat	4					// Tamaño del encabezado
 #define kMsgID1	"GPRMC"
 #define kMsgID2	"GPGGA"
 #define kMsgID3	"GPGSV" 
 
 
 union _GPSsts {
	 unsigned int Reg;
	 struct{
		 unsigned char	fOn		:1;	//Gps Encendido
		 unsigned char	fMsgOk	:1;	//String valido
		 unsigned char	fLec	:1; //Leyendo datos
		 unsigned char	xGPSfail:1;	//Falla en la lectura GPS
		 unsigned char	xEnc	:1;	//Comienzo lectura encabezado
		 unsigned char	xRxOn	:1;	//Comienzo de la recepcion 
		 
		 unsigned char 	x5Hz	:1;	//Indica si se reciben los datos cada 5 Hz
		 
		 unsigned char	fLec5hz	:1; //
		 unsigned char	xicoger	:1;
		 unsigned char	fBaud	:1;	// Para saber si esta 19200 baudios
		 unsigned char 	zAX1	:1;	//
		 unsigned char 	zAX2	:1;	//
		 unsigned char 	zAX3	:1;	//
		 unsigned char 	zAXI3	:1;	//
		 unsigned char 	zAxu1	:1;
		 unsigned char 	zAxu2	:1;
		 }B;
	};

struct _GPSdts {
	struct{
		unsigned int 	h;		//horas
		unsigned int 	m;		//Minutos
		unsigned int 	s;		//Segundos
	  	}hora;
	struct{
		unsigned int 	a;		//Años
		unsigned int 	m;		//Meses
		unsigned int 	d;		//Dias
		}fecha;
	struct{
		float 			lat;	//Latitud
		unsigned char  	NS;		//Norte Sur
		float 			lon;	//Longitud
		unsigned char  	EO;		//Este Oeste
		float 			vel;	//Velocidad
		unsigned int	rmb;	//Rumbo
		float			dist;	//Distancia recorrida
		unsigned char	latc[20];
		unsigned char	lonc[20];
		}pos;
	struct{
		
		unsigned char 	act;	//Datos validos	
		unsigned int 	sat;	//Cantidad de satelites
		unsigned int	gan;	//Ganancia Promedio
		unsigned int	tvida;	//Tiempo de vida
		unsigned int	tvid;	//Tiempo de vida actual
		unsigned int	satview;
		float		 	pres;	//Presicion horizontal
		unsigned int 	t5hrz;
		unsigned int	tGPSon;
		unsigned char	GPSrst;
		unsigned char	Baud;
		}sys;
	};

struct _SatInfo {
	unsigned int	Sat;
	unsigned int	Gan;
	};
	
struct _SatInfo2 {
	unsigned int	Elv;
	unsigned int	Azt;
	unsigned int	Gan;
	};


extern struct _SatInfo2 SatInfo2[32];	
extern struct _SatInfo SatInfo[32];
extern union _GPSsts 	GPSsts;
extern struct _GPSdts	GPSdts;
extern unsigned char GPSmsg[128];
extern unsigned int GPSInd;


extern unsigned char RMC[128];
extern unsigned int PRMC;
extern unsigned char GGA[128];
extern unsigned int PGGA;
//extern union _GPSsts 	GPSs;
//extern struct _GPSdts	GPSd;
//extern unsigned char GPSm[128];
//extern unsigned int GPSI;
//-----------------------------------------------------------------------------
// 

	extern float artoflt(const char *s);
	extern int	MsgCheck(unsigned char *Msg, char *Enc);
	extern unsigned char GPSCheckSum(unsigned char *S);
	extern void GPSInfo(unsigned char *S);
	extern char  MovtoInt(unsigned char *Dest, unsigned char *Orig, int lDest);
	extern void GPSbaud(void);	
	extern void GPS5hrz(void);
		 
		 