/*****************************************************************************
 *	DEFINICION DE LOS TIPOS DE VARIABLES ULITIZADAS EN EL PROGRAMA
 *  
 * 
 *
 *	PROPIEDAD	:	SIID
 *	DISEÑADO POR:	MNM 
 *	REVISION	:	1.00  2013
 *	
 *****************************************************************************/
//****************************************************************************
//		#include	"VarType1.h"


//****************************************************************************
//****************************************************************************
//		DEFINICION DE TIPOS DE DATOS
//****************************************************************************
//****************************************************************************
	typedef unsigned char BYTE;
	typedef unsigned int WORD;
	typedef unsigned int UINT;
	typedef unsigned long ULONG;
	typedef unsigned long DWORD; 

//-----------------------------------------------------------------------------
//	Estructuraa de los pulos de tiempo
	struct _Sts_Tmr{
		struct{
			BYTE Pls0001	:1;	//Pulso cada 1 milisegundo
			BYTE Pls0010	:1;	//Pulso cada 10 milisegundo
			BYTE Pls0100	:1;	//Pulso cada 100 milisegundo
			BYTE Pls1000	:1;	//Pulso cada 1 segundo
			BYTE GPS5Hz		:1;	//Pulso para mandar el comando de 5Hz al gps
			BYTE ADCInit	:1;
			BYTE LecId		:1;	//Pulso para leer Id sensores
			BYTE SetID0500	:1; //Pulso cada 500 mseg para envio de mensaje del seteo de ID
			BYTE B8:1;
			BYTE B9:1;
			BYTE B10:1;
			BYTE B11:1;
			BYTE B12:1;
			BYTE B13:1;
			BYTE B14:1;
			BYTE B15:1;
			} B;
		BYTE 	Cnt0010;
		BYTE 	Cnt0100;
		BYTE 	Cnt1000;
		BYTE 	Cnt0500;
		BYTE	CntGPS5;
		BYTE	CntWifi;
		
	};
	
struct _DtsSen{
	unsigned int Med;	//Cantidad de semillas leidas	
	unsigned char tMed;	//Temporizador de medicion
	union{
		struct{
			unsigned char Val;
			unsigned char Aux;
		}C;
		struct{
		//Val
			BYTE Con	:1;		//El sensor esta conectado 
			BYTE Act	:1;		//Esta leyendo semillas
			BYTE Hab	:1;		//Esta habilitado por sistema	
			BYTE Det	:1;		//El sensor esta Detectado
			BYTE 		:1;
			BYTE Mod	:1;		//Modo de siembra 1 Gruesa - 0 Fina
			BYTE AvTs	:1;		//Aviso de tubo sucio
			BYTE FDs	:1;		//Falla desconexion
		//Aux
			BYTE AxDesc	:1;		//Auxiliar Falla desconexion
			BYTE AuxTT	:1;		//Auxiliar Falla Tubo Tapado
			BYTE 		:6;		//Auxiliar Falla Tubo Tapado		
		}B;
	}Sts;
};

struct _SenRot{
	unsigned int Med;
	unsigned long AlMax;
	unsigned long AlMin;
	unsigned char TmMax;
	unsigned char TmMin;
	unsigned char TMinAux;
	unsigned char TMaxAux;
	unsigned long FK;
	union{
		unsigned char Val;
		struct{
			BYTE Con	:1;		//El sensor esta conectado 
			BYTE Bus	:1;		//0:Bus0  1:Bus 1
			BYTE Hab	:1;		//Esta habilitado por sistema	
			BYTE Det	:1;		//El sensor esta Detectado
			BYTE B4		:1;		//
			BYTE FMin	:1;		//Alarma medicion por debajo del minimo
			BYTE FMax	:1;		//Alamra medicion por encima del maximo
			BYTE FDs	:1;		//Falla desconexion
		}B;
	}Sts;
};

union _SenTol{
	struct{
		unsigned char Val;
		unsigned char Alcont;
	}C;
	struct{
		BYTE Con	:1;		//El sensor esta conectado 
		BYTE Bus	:1;		//0:Bus0  1:Bus 1
		BYTE Hab	:1;		//Esta habilitado por sistema	
		BYTE Det	:1;		//El sensor esta Detectado
		BYTE SNV	:1;		//0:Desactivado	1: Activado (Detecta semilla)
		BYTE B5		:1;		//
		BYTE B6		:1;		//
		BYTE FDs	:1;		//Falla desconexion
		BYTE 		:8;
		
	}B;
};	

struct _Mod{
	unsigned int	Vel;	//Velocidad
	unsigned long	Dis;	//Distancia Reconrrida
	unsigned long	Pul;	//Contador de pulsos
	unsigned int	SP;		//Set Point	
	unsigned int	KD;		//Relacion anda a saber que corno es esto
	unsigned long	PKm;	//Pulsos por Kilometro
	unsigned int	RDT;	//Relacion de transimicion
	unsigned char	PVT;	//Pulsos por vuelta Traccion
	unsigned char	PVD;	//Pulsos por vuelta Distribucion
	union{
		struct{
			BYTE Con	:1;		//El sensor esta conectado 
			BYTE Bus	:1;		//0:Bus0  1:Bus 1
			BYTE Hab	:1;		//Esta habilitado por sistema	
			BYTE Det	:1;		//El sensor esta Detectado
			BYTE B4		:1;		//
			BYTE B5		:1;
			BYTE B6		:1;
			BYTE FDs	:1;		//Falla desconexion
		}B;
		BYTE Val;
	}Sts;
	union{
		struct{
			BYTE TRC	:1;		//Fallo en la traccion lo calculamos aca!!!!
			BYTE AL1	:1;
			BYTE AL2	:1;
			BYTE AL3	:1;
			BYTE MOT	:1;		//Alarma Fallo Motor
			BYTE DEC	:1;		//Fallo ajuste decremento
			BYTE INC	:1;		//Fallo ajuste incremento
			BYTE DST	:1;		//Falla distribucion
		}B;
		BYTE Val;
	}Al;
};

struct _BUS_MED{
	struct _DtsSen Sie[32];
	struct _DtsSen Fer[32];
};

// Datos de los sensores para transimitir a la pantalla
struct _DtsCom{
	struct{
		unsigned int 	Med[32];
		unsigned char	Sts[32];
	}Sie;
	struct{
		unsigned int 	Med[32];
		unsigned char	Sts[32];
	}Fer;
};

// Datos de los sensores para transimitir a la pantalla
struct _DtsPerCom{
	struct{
		unsigned int 	Al[16];
		unsigned char	Sts[16];
	}MOD;
	struct{
		unsigned int 	Med[8];
		unsigned char	Sts[8];
	}ROT;
	struct{
		unsigned int 	Med[3];
		unsigned char	Sts[3];
	}TRB;
	struct{
		unsigned char	Sts[16];
	}TOL;
};


union _Proc{
	struct{
		//Comunicacion LIN
		BYTE fInicio 	: 1;//Proceso de inicializacion del sistema Transmicion de dados
		BYTE fStopDts 	: 1;	//Detiene la transmicion de datos de proceso
		BYTE fDtsSat  	: 1;	//Transmicion de datos de los satelites
		BYTE fT2xCOM 	: 1;	//Indica el fin de la transmicion del buffer
		BYTE fT3xCOM 	: 1;	//Indica el fin de la transmicion del buffer
		//Comandos y Configuracion
		BYTE fGPSLec	: 1;	//Lectura directa del GPS
		BYTE fWifiConf	: 1;	//Conecta el modulo Wifi con el RS-232 para su configuracion
		BYTE fDataErr	: 1;	//Dato procesado no es Hex codificado en ASCII 
		BYTE fRxCom  	: 1;	//Recepcion de un array	
		BYTE fGPSDtOk	: 1;	//Datos del GPS Actualizados
		BYTE fSetId		: 1;	//Se modifica ID de sensores desde el Modulo
		BYTE fImpSw		: 1;	//Flag para  indicar como esta el implement switch
		BYTE Puto		: 1;
		BYTE fGrabaDts	: 1;	//Flag para enviar el cheksum del reporte de datos
		BYTE fConfPer	: 1;
		BYTE fConfSen	: 1;	//Configuracion Sensores siembra fertilizante
		BYTE Auxiliar	: 1;
	
		BYTE fAdqSie1 	: 1;//Fin de la lectura de los sensores y se los carga en el buffer de transmicion
		BYTE fAdqRot1  	: 1;
		BYTE fAdqMod1  	: 1;
		BYTE fAdqTRB1  	: 1;
		BYTE fAdqNTL1	: 1;
		BYTE fAdqAx11	: 1;
		BYTE fAdqAx12	: 1;
		BYTE fAdqAx13	: 1;
		BYTE fAdqSie2	: 1;//Fin de la lectura de los sensores y se los carga en el buffer de transmicion
		BYTE fAdqRot2  	: 1;
		BYTE fAdqMod2  	: 1;
		BYTE fAdqTRB2  	: 1;
		BYTE fAdqNTL2	: 1;
		BYTE fAdqAx21	: 1;
		BYTE fAdqAx22	: 1;
		BYTE fAdqAx23	: 1;
	}B;
	struct{
	unsigned int Proc;
	unsigned int Disp;
	}Value;
	
};
	

//Estado de los sensores Habilitados-Conectados
struct _SenDts{
	unsigned long SemB1;	//Semilla Bus1
	unsigned long SemB2;	//Semilla Bus1
	unsigned long FerB1;	//Fertilizante Bus1
	unsigned long FerB2;	//Fertilizante Bus2
};

//Estado de los perifericos Habilitados-Conectados-Bus
struct _DtsPer{
	unsigned int	MOD;	//Moduladoras
	unsigned char	ROT;	//Modulos de Rotacion
	unsigned char	TRB;	//Sensores de Turbina/RPM
	unsigned int	TOL;		//Sensores de Nivel
};

struct _ConfSen{
	unsigned char SemTM;	//Tiempo de medicion Semillas
	unsigned char SemTD;	//Tiempo fin de medicion
	unsigned char SemCS;	//Cantidad antes de comenzar la medicion
	unsigned char FerTM;	//Tiempo de medicion fertilizante
	unsigned char FerTD;	//Tiempo fin de medicion
	unsigned char FerCS;	//Cantidad antes de comenzar la medicion
};

struct _DtsSiembra{
	unsigned long	tTrabPar;	//Tiempo de marcha parcial
	unsigned long	tTrabTot;	//Tiempo de marcha total
	unsigned long	DistRecPar;	//Distancia recorrida parcial
	unsigned long	DistRecTot;	//Distancia recorrida total
	unsigned long	SupSemPar;	//Superficie sembrada parcial
	unsigned long	SupSemTot;	//Superficie sembrada total
	unsigned int	VelProm;	//Velocidad promedio de siembra
	unsigned int	VelMax;		//Velocidad maxima de siembra
	unsigned int 	SepSurco;	//Separacion de surcos
	unsigned int	Surcos;		//Cantidad de surcos
	unsigned int	ModSie;		//Modo de siembra
	unsigned int 	Ancho;		//Ancho de siembra
};

struct _SetId
{
	struct{
		BYTE Ocupado:1;
		BYTE Libre	:1;
		BYTE Sensor	:1;
		BYTE GrabaID:1;
		BYTE SenLec	:1;
		BYTE SenEsc	:1;
		BYTE ModCon	:1;
		BYTE Ax1	:1;
	}StsBus;
	unsigned char Id;
	unsigned char IdMax;
	unsigned char IdMin;
	unsigned char VoidId;
	unsigned char NuevoId;
	
};

union _StsEcu{
	unsigned int Val;
	struct{
		BYTE LIN1	:1;		//LIN 1 Conectado 
		BYTE LIN2	:1;		//LIN 2 Conectado
		BYTE WIFI	:1;		//Wifi Encendido	
		BYTE GPS	:1;		//GPS Ecendido
		BYTE CAN	:1;		//CAN Encendido
		BYTE VIB	:1;		//Sensor de Vibracion Activo
		BYTE LED	:1;		//Salida de (Led) Activa
		BYTE TCL	:1;		//Tecla (implement switch) Activa
		BYTE SB1	:1;		//Sensores de Semilla en Bus 1
		BYTE FB1	:1;		//Sensores de Fertilizante en Bus 1
		BYTE SB2	:1;		//Sensores de Semilla en Bus 2
		BYTE FB2	:1;		//Sensores de Fertilizante en Bus 2
		BYTE Mod	:1;		//Moduladoras Conectadas
		BYTE ROT	:1;		//Sensores de Rotacion conectado
		BYTE TRB	:1;		//Sensor de Turbuna conectado
		BYTE TOL	:1;		//Sensores de tolva conectados
	}B;
};
//-----------------------------
union _UInt32
{
	struct {
		BYTE B0:1;
		BYTE B1:1;
		BYTE B2:1;
		BYTE B3:1;
		BYTE B4:1;
		BYTE B5:1;
		BYTE B6:1;
		BYTE B7:1;
		BYTE B8:1;
		BYTE B9:1;
		BYTE B10:1;
		BYTE B11:1;
		BYTE B12:1;
		BYTE B13:1;
		BYTE B14:1;
		BYTE B15:1;
		BYTE B16:1;
		BYTE B17:1;
		BYTE B18:1;
		BYTE B19:1;
		BYTE B20:1;
		BYTE B21:1;
		BYTE B22:1;
		BYTE B23:1;
		BYTE B24:1;
		BYTE B25:1;
		BYTE B26:1;
		BYTE B27:1;
		BYTE B28:1;
		BYTE B29:1;
		BYTE B30:1;
		BYTE B31:1;
	} BIT;
	struct {
		char V[4];
	} C;
	struct {
		unsigned char V[4];
	} B;	
	struct {
		int V[2];
	} I;	
	struct {
		unsigned int V[2];
	} UI;	
	struct {
		unsigned int V[2];
	} W;	
	struct {
		long V;
	} L;	
	struct {
		unsigned long V;
	} UL;
	struct {
		unsigned long V;
	} DW;
};
	
union _localInt{
	struct{
		unsigned char Low;
		unsigned char High;
		}C;
	unsigned int I;
};

union _UInt16
{
	struct {
		BYTE B0:1;
		BYTE B1:1;
		BYTE B2:1;
		BYTE B3:1;
		BYTE B4:1;
		BYTE B5:1;
		BYTE B6:1;
		BYTE B7:1;
		BYTE B8:1;
		BYTE B9:1;
		BYTE B10:1;
		BYTE B11:1;
		BYTE B12:1;
		BYTE B13:1;
		BYTE B14:1;
		BYTE B15:1;
	} BIT;
	
	struct {
		char V[2];
	} C;
	
	struct {
		unsigned char V[2];
	} B;	
	struct {
		int V;
	} I;	
	struct {
		unsigned int V;
	} UI;	
	struct {
		unsigned int V;
	} W;	

};

struct _TLin{

	int Timer;
	int ind;
	int Ult [10];

};

struct _GPS
{
	float Act;
	float Ant;
};
struct _SCom
{
	unsigned char lMod	:1;	//lectura estado modulo
	unsigned char fMod	:1;	//Estado del modulo	
	unsigned char lCom	:1;	//Inicio de lectura del comando
	unsigned char fCom	:1;	//Comando Listo
	unsigned char wf232	:1;	//Comunicacion RS-232 Wifi directo
	unsigned char IniDato:1;	//Dato Falla
	unsigned char EscDato:1;
	unsigned char LeeDato:1;	//Carga dato en memoria
	unsigned char fConectado:1;
	unsigned char f2	:7;
	
};

struct _Sensores{
	union 
	{
		struct {
			BYTE TX_SF1:1;
			BYTE TX_SF2:1;
			BYTE TX_ROT:1;
			BYTE TX_MOD:1;
			BYTE TX_TRB:1;
			BYTE TX_TOL:1;
			BYTE TX_B6:1;
			BYTE TX_B7:1;
			
			BYTE RX_SF1:1;
			BYTE RX_SF2:1;
			BYTE RX_ROT:1;
			BYTE RX_MOD:1;
			BYTE RX_TRB:1;
			BYTE RX_TOL:1;
			BYTE RX_B6:1;
			BYTE DIAG:1;
			
			BYTE fSAct1:1;
			BYTE fFAct1:1;
			BYTE fSAct2:1;
			BYTE fFAct2:1;
			BYTE zzz:1;

			
		}B;
		unsigned long Val;
	}STS;
	BYTE tSIE;
	BYTE tROT;
	BYTE tMOD;
	BYTE tTRB;
	BYTE tTOL;
};

struct _MGPS
{
	unsigned char Puerto;
	unsigned char Baudrate;
};

struct _CGPS
{
	unsigned long Sentencias;
	unsigned long Correctas;
	unsigned long Fallas;
	unsigned long GPRMC;
	unsigned long GPGGA;
	unsigned long GPGSV;
};

struct _Dest
{
	unsigned int Sec;
	unsigned char Duty;
};
