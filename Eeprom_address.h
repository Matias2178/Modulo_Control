//*****************************************************************************
//			DIRECCIONES DE LA EEPROM
//			24LC512
//*****************************************************************************

//HABILITACION DE LOS DIFERENTES DISPOSITIVOS CONECTADOS AL MODULO
//0x0000 - 0x0017 - (24)
enum{M_INFD=0x0000
	,M_INFD_MOD_0=M_INFD	//12
	,M_INFD_MOD_1
	,M_INFD_MOD_2
	,M_INFD_MOD_3
	,M_INFD_MOD_4
	,M_INFD_MOD_5
	,M_INFD_MOD_6
	,M_INFD_MOD_7
	,M_INFD_MOD_8
	,M_INFD_MOD_9
	,M_INFD_MOD_10
	,M_INFD_MOD_11
				
	,M_INFD_SN_0			//4
	,M_INFD_SN_1
	,M_INFD_SN_2
	,M_INFD_SN_3

	,M_INFD_FFAB_0			//4
	,M_INFD_FFAB_1
	,M_INFD_FFAB_2
	,M_INFD_FFAB_3			
	
	,M_INFD_ID_VER_0		//4
	,M_INFD_ID_VER_1
	,M_INFD_ID_VER_2
	,M_INFD_ID_VER_3					
};
//-----------------------------------------------
//AJUSTES CORRIMIENTO ALTA DENSIDAD
//0x0018- 0x001C (4)
enum{M_FAC_AJS_ALTA_DENS=0x0018									//long
	,M_FAC_AJS_ALTA_DENS_0=M_FAC_AJS_ALTA_DENS
	,M_FAC_AJS_ALTA_DENS_1
	,M_FAC_AJS_ALTA_DENS_2
	,M_FAC_AJS_ALTA_DENS_3
};
//-----------------------------------------------
//CHECK DE MEMORIA DE FABRICA
//0x001C - 0x001D - (2)
enum{M_CHECK_FAB=0x001C
	,M_CHECK_FAB_L=M_CHECK_FAB
	,M_CHECK_FAB_H
};

//-----------------------------------------------
//CHECK DE MEMORIA DE FABRICA
//0x001E - 0x001F - (2)
enum{M_GPS_AUX=0x001E
	,M_GPS_AUX_L=M_GPS_AUX
	,M_GPS_AUX_H
};

//-----------------------------------------------
//AJUSTES DEL SISTEMA
//0x0020 - 0x0023 - (4)
enum{M_AJS_TIME_ZONE=0x0020									//long
	,M_AJS_TIME_ZONE_0=M_AJS_TIME_ZONE
	,M_AJS_TIME_ZONE_1

	
	,M_BAUDRATE				//2
	,M_BAUDRATE_WIFI = M_BAUDRATE
	,M_BAUDRATE_RS232
};
//-----------------------------------------------
//CONTADOR DE RESETEOS RN-171
//0x0024 - 0x0027 - (4)
enum{M_RN171_OFF=0x0024									//long
	,M_RN171_OFF_0=M_RN171_OFF
	,M_RN171_OFF_1
	,M_RN171_OFF_2
	,M_RN171_OFF_3

};

//-----------------------------------------------
//CONTADOR DE RESETEOS DEL FORRO DEL MICRO
//0x0028 - 0x002B - (4)
enum{M_MIC_OFF=0x0028									//long
	,M_MIC_OFF_0=M_RN171_OFF
	,M_MIC_OFF_1
	,M_MIC_OFF_2
	,M_MIC_OFF_3

};
	
//-----------------------------------------------
//PARAMETROS SENSORES
//esto son constantes que no se utilizan
//ver de eliminar
//ademas de agregar para metros para la fina
//0x0044 - 0x0049 - (6)
//enum{M_PAR_SEN=0x0044
//	,M_PAR_SEM_TM=M_PAR_SEN		//tiempo medicion
//	,M_PAR_SEM_TD				//tiempo stop deteccion
//	,M_PAR_SEM_CS				//cantidad start
//	,M_PAR_FER_TM				//tiempo medicion
//	,M_PAR_FER_TD				//tiempo stop deteccion
//	,M_PAR_FER_CS				//cantidad start
//};

//-----------------------------------------------
//ESTADO DE LOS SENSORES DE SEMILLA FERTILIZANTE
//0x0030 - 0x0047 - (24)
enum{M_STS_HAB_PER=0x0030				//unsigned long

	
	,M_STS_HAB_MOD	= M_STS_HAB_PER		//(8)
	,M_STS_HAB_MOD0 = M_STS_HAB_MOD	//1
	,M_STS_HAB_MOD1					//1
	,M_STS_HAB_ROT 					//1
	,M_STS_HAB_TRB					//1	
	,M_STS_HAB_TOL0					//1	
	,M_STS_HAB_TOL1					//1	
	,M_STS_HAB_PRE0					//1	
	,M_STS_HAB_PRE1					//1	

	,M_STS_CON_PER						//(8)
	,M_STS_CON_MOD	= M_STS_CON_PER	
	,M_STS_CON_MOD0 = M_STS_CON_MOD	//1
	,M_STS_CON_MOD1					//1
	,M_STS_CON_ROT 					//1
	,M_STS_CON_TRB					//1
	,M_STS_CON_TOL0					//1
	,M_STS_CON_TOL1					//1
	,M_STS_CON_PRE0					//1
	,M_STS_CON_PRE1					//1

	,M_STS_BUS_PER						//(8)
	,M_STS_BUS_MOD = M_STS_BUS_PER	
	,M_STS_BUS_MOD0 = M_STS_BUS_MOD	//1
	,M_STS_BUS_MOD1					//1
	,M_STS_BUS_ROT 					//1	
	,M_STS_BUS_TRB					//1
	,M_STS_BUS_TOL0					//1	
	,M_STS_BUS_TOL1					//1	
	,M_STS_BUS_PRE0					//1	
	,M_STS_BUS_PRE1					//1		
	

};
//-----------------------------------------------
//PARAMETROS SEMBRADORA
//0x0058 - 0x0075 - (30)
enum{M_DATOS_SEMBRADORA=0x0058
	,M_DATOS_SEMB_TM=M_DATOS_SEMBRADORA
											//Tiempo de siembra	
	,M_DATOS_SEMB_TM0 = M_DATOS_SEMB_TM		//4
	,M_DATOS_SEMB_TM1						
	,M_DATOS_SEMB_TM2
	,M_DATOS_SEMB_TM3
											//Tiempo de siembra total
	,M_DATOS_SEMB_TMT						//4
	,M_DATOS_SEMB_TMT0 = M_DATOS_SEMB_TMT
	,M_DATOS_SEMB_TMT1
	,M_DATOS_SEMB_TMT2
	,M_DATOS_SEMB_TMT3
											//Distancia recorrida sembrando
	,M_DATOS_SEMB_DIST						//4
	,M_DATOS_SEMB_DIST0 = M_DATOS_SEMB_DIST
	,M_DATOS_SEMB_DIST1
	,M_DATOS_SEMB_DIST2
	,M_DATOS_SEMB_DIST3
											//Distancia recorrida sembrando total
	,M_DATOS_SEMB_DISTTOT					//4
	,M_DATOS_SEMB_DISTTOT0 = M_DATOS_SEMB_DISTTOT
	,M_DATOS_SEMB_DISTTOT1
	,M_DATOS_SEMB_DISTTOT2	
	,M_DATOS_SEMB_DISTTOT3
											//Superficie sembranda
	,M_DATOS_SEMB_SUPSEM					//4
	,M_DATOS_SEMB_SUPSEM0 = M_DATOS_SEMB_SUPSEM
	,M_DATOS_SEMB_SUPSEM1
	,M_DATOS_SEMB_SUPSEM2
	,M_DATOS_SEMB_SUPSEM3
											//Superficie sembrada total
	,M_DATOS_SEMB_SUPSEMT					//4
	,M_DATOS_SEMB_SUPSEMT0 = M_DATOS_SEMB_SUPSEMT
	,M_DATOS_SEMB_SUPSEMT1
	,M_DATOS_SEMB_SUPSEMT2
	,M_DATOS_SEMB_SUPSEMT3
											//Velocidad promedio de siembra
	,M_DATOS_SEMB_VELPROM					//2
	,M_DATOS_SEMB_VELPROM_L = M_DATOS_SEMB_VELPROM
	,M_DATOS_SEMB_VELPROM_H
											//Velocidad maxima de siembra
	,M_DATOS_SEMB_VELMAX					//2
	,M_DATOS_SEMB_VELMAX_L = M_DATOS_SEMB_VELMAX
	,M_DATOS_SEMB_VELMAX_H				
											//Distancia entre surcos
	,M_DIST_SURCOS_DEF						//2
	,M_DIST_SURCOS_DEF_L = M_DIST_SURCOS_DEF
	,M_DIST_SURCOS_DEF_H
	
};

//-----------------------------------------------
//ESTADO DE LOS SENSORES DE SEMILLA FERTILIZANTE
//0x0100 - 0x0130 - (48)
enum{M_STS_HAB_SEN=0x0100				//unsigned long
	,M_STS_HAB_SEN_BUS1_SEM0=M_STS_HAB_SEN	//4
	,M_STS_HAB_SEN_BUS1_SEM1
	,M_STS_HAB_SEN_BUS1_SEM2
	,M_STS_HAB_SEN_BUS1_SEM3
	
	,M_STS_HAB_SEN_BUS1_FER0				//4
	,M_STS_HAB_SEN_BUS1_FER1
	,M_STS_HAB_SEN_BUS1_FER2
	,M_STS_HAB_SEN_BUS1_FER3
	
	,M_STS_HAB_SEN_BUS2_SEM0				//4
	,M_STS_HAB_SEN_BUS2_SEM1
	,M_STS_HAB_SEN_BUS2_SEM2
	,M_STS_HAB_SEN_BUS2_SEM3
	
	,M_STS_HAB_SEN_BUS2_FER0				//4
	,M_STS_HAB_SEN_BUS2_FER1
	,M_STS_HAB_SEN_BUS2_FER2
	,M_STS_HAB_SEN_BUS2_FER3
	
	,M_STS_CON_SEN
	,M_STS_CON_SEN_BUS1_SEM0=M_STS_CON_SEN	//4
	,M_STS_CON_SEN_BUS1_SEM1
	,M_STS_CON_SEN_BUS1_SEM2
	,M_STS_CON_SEN_BUS1_SEM3
	
	,M_STS_CON_SEN_BUS1_FER0				//4
	,M_STS_CON_SEN_BUS1_FER1
	,M_STS_CON_SEN_BUS1_FER2
	,M_STS_CON_SEN_BUS1_FER3
	
	,M_STS_CON_SEN_BUS2_SEM0				//4
	,M_STS_CON_SEN_BUS2_SEM1
	,M_STS_CON_SEN_BUS2_SEM2
	,M_STS_CON_SEN_BUS2_SEM3
	
	,M_STS_CON_SEN_BUS2_FER0				//4
	,M_STS_CON_SEN_BUS2_FER1
	,M_STS_CON_SEN_BUS2_FER2
	,M_STS_CON_SEN_BUS2_FER3
	
	,M_STS_MOD_SEN
	,M_STS_MOD_SEN_BUS1_SEM0=M_STS_MOD_SEN	//4
	,M_STS_MOD_SEN_BUS1_SEM1
	,M_STS_MOD_SEN_BUS1_SEM2
	,M_STS_MOD_SEN_BUS1_SEM3
	
	,M_STS_MOD_SEN_BUS1_FER0				//4
	,M_STS_MOD_SEN_BUS1_FER1
	,M_STS_MOD_SEN_BUS1_FER2
	,M_STS_MOD_SEN_BUS1_FER3
	
	,M_STS_MOD_SEN_BUS2_SEM0				//4
	,M_STS_MOD_SEN_BUS2_SEM1
	,M_STS_MOD_SEN_BUS2_SEM2
	,M_STS_MOD_SEN_BUS2_SEM3
	
	,M_STS_MOD_SEN_BUS2_FER0				//4
	,M_STS_MOD_SEN_BUS2_FER1
	,M_STS_MOD_SEN_BUS2_FER2
	,M_STS_MOD_SEN_BUS2_FER3	
};

//NOMBRE SEMBRADORA
//0x1000 - 0x101D - (20)
enum{
	M_NOMBRE_SEMB=0x1000
	,M_NOMBRE_SEMB1	=	M_NOMBRE_SEMB
	,M_NOMBRE_SEMB2
	,M_NOMBRE_SEMB3
	,M_NOMBRE_SEMB4
	,M_NOMBRE_SEMB5
	,M_NOMBRE_SEMB6
	,M_NOMBRE_SEMB7
	,M_NOMBRE_SEMB8
	,M_NOMBRE_SEMB9
	,M_NOMBRE_SEMB10
	,M_NOMBRE_SEMB11
	,M_NOMBRE_SEMB12
	,M_NOMBRE_SEMB13
	,M_NOMBRE_SEMB14
	,M_NOMBRE_SEMB15
	,M_NOMBRE_SEMB16
	,M_NOMBRE_SEMB17
	,M_NOMBRE_SEMB18
	,M_NOMBRE_SEMB19
	,M_NOMBRE_SEMB20
};
enum{
	M_DTS_SEM_B00=0x8000				//Bloque 00 512
/*	,M_DTS_SEM_B01=0x81FF				//Bloque 01 512
	,M_DTS_SEM_B02=0x83FF				//Bloque 02 512
	,M_DTS_SEM_B03=0x85FF				//Bloque 03 512
	,M_DTS_SEM_B04=0x87FF				//Bloque 04 512
	,M_DTS_SEM_B05=0x89FF				//Bloque 05 512
	,M_DTS_SEM_B06=0x8BFF				//Bloque 06 512
	,M_DTS_SEM_B07=0x8DFF				//Bloque 07 512
	,M_DTS_SEM_B08=0x8FFF				//Bloque 08 512
	,M_DTS_SEM_B09=0x91FF				//Bloque 09 512
	,M_DTS_SEM_B10=0x93FF				//Bloque 10 512
	,M_DTS_SEM_B11=0x95FF				//Bloque 11 512
	,M_DTS_SEM_B12=0x97FF				//Bloque 12 512
	,M_DTS_SEM_B13=0x99FF				//Bloque 13 512
	,M_DTS_SEM_B14=0x9BFF				//Bloque 14 512
	,M_DTS_SEM_B15=0x9DFF				//Bloque 15 512
	,M_DTS_SEM_B16=0x9FFF				//Bloque 16 512
	,M_DTS_SEM_B17=0xA1FF				//Bloque 17 512
	,M_DTS_SEM_B18=0xA3FF				//Bloque 18 512
	,M_DTS_SEM_B19=0xA5FF				//Bloque 19 512
	,M_DTS_SEM_B20=0xA7FF				//Bloque 20 512
	,M_DTS_SEM_B21=0xA9FF				//Bloque 21 512
	,M_DTS_SEM_B22=0xABFF				//Bloque 22 512
	,M_DTS_SEM_B23=0xADFF				//Bloque 23 512
	,M_DTS_SEM_B24=0xAFFF				//Bloque 24 512
	,M_DTS_SEM_B25=0xB1FF				//Bloque 25 512
	,M_DTS_SEM_B26=0xB3FF				//Bloque 26 512
	,M_DTS_SEM_B27=0xB5FF				//Bloque 27 512
	,M_DTS_SEM_B28=0xB7FF				//Bloque 28 512
	,M_DTS_SEM_B29=0xB9FF				//Bloque 29 512
	,M_DTS_SEM_B30=0xBBFF				//Bloque 30 512
	,M_DTS_SEM_B31=0xBDFF				//Bloque 31 512
	,M_DTS_SEM_B32=0xBFFF				//Bloque 32 512
	,M_DTS_SEM_B33=0xC1FF				//Bloque 33 512
	,M_DTS_SEM_B34=0xC3FF				//Bloque 34 512
	,M_DTS_SEM_B35=0xC5FF				//Bloque 35 512
	,M_DTS_SEM_B36=0xC7FF				//Bloque 36 512
	,M_DTS_SEM_B37=0xC9FF				//Bloque 37 512
	,M_DTS_SEM_B38=0xCBFF				//Bloque 38 512
	,M_DTS_SEM_B39=0xCDFF				//Bloque 39 512
	,M_DTS_SEM_B40=0xCFFF				//Bloque 40 512
	,M_DTS_SEM_B41=0xD1FF				//Bloque 41 512
	,M_DTS_SEM_B42=0xD3FF				//Bloque 42 512
	,M_DTS_SEM_B43=0xD5FF				//Bloque 43 512
	,M_DTS_SEM_B44=0xD7FF				//Bloque 44 512
	,M_DTS_SEM_B45=0xD9FF				//Bloque 45 512
	,M_DTS_SEM_B46=0xDBFF				//Bloque 46 512
	,M_DTS_SEM_B47=0xDDFF				//Bloque 47 512
	,M_DTS_SEM_B48=0xDFFF				//Bloque 48 512
	,M_DTS_SEM_B49=0xE1FF				//Bloque 49 512
	,M_DTS_SEM_B50=0xE3FF				//Bloque 50 512
	,M_DTS_SEM_B51=0xE5FF				//Bloque 51 512
	,M_DTS_SEM_B52=0xE7FF				//Bloque 52 512
	,M_DTS_SEM_B53=0xE9FF				//Bloque 53 512
	,M_DTS_SEM_B54=0xEBFF				//Bloque 54 512
	,M_DTS_SEM_B55=0xEDFF				//Bloque 55 512
	,M_DTS_SEM_B56=0xEFFF				//Bloque 56 512
	,M_DTS_SEM_B57=0xF1FF				//Bloque 57 512
	,M_DTS_SEM_B58=0xF3FF				//Bloque 58 512
	,M_DTS_SEM_B59=0xF5FF				//Bloque 59 512
	,M_DTS_SEM_B60=0xF7FF				//Bloque 60 512
	,M_DTS_SEM_B61=0xF9FF				//Bloque 61 512
	,M_DTS_SEM_B62=0xFBFF				//Bloque 62 512
	,M_DTS_SEM_B63=0xFDFF				//Bloque 63 512
*/
};
