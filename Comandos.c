	#include	"Constantes.h"
	#include	"UART.h"
	#include	"VarGlob.h"
	#include 	<stdlib.h>
	#include	<string.h>


char Check(unsigned char *Msg,char *Enc, int i);
unsigned char Movstr(unsigned char *Dest, unsigned char *Orig);
unsigned int ModSiembra(unsigned char *Dest);
unsigned char* ModSiembraTx(unsigned char *S);
unsigned char * TXTError(unsigned char *S);
void StsPer64 (char *lb, unsigned char *S,unsigned long Sen1,unsigned long Sen2);
void StsPer16 (char *lb, unsigned char *S,int Sen1);
unsigned char * FinCmd(unsigned char *S);
unsigned char * BauToStr(unsigned char * S, char Baud);
void CBuffersTx(void);
unsigned char * EstadosCN(char *lb,unsigned char *S);

/******************************************************************************
*	Funcion:		Comando()
*	Descricpion:	Analiza los comandos de entrada y genera las banderas necesarias
*	Ingreso Datos:	Buffer de los comandos de entrada
*	Salida Datos:	Ninguno
******************************************************************************/	
void Comando(unsigned char *S)
{
	unsigned char *P;

	unsigned char Cmd[10];
	unsigned char i = (unsigned char) 0;
	unsigned char a;
	unsigned char Com;
	unsigned char dsh;
	unsigned char KDAux;
	unsigned char SPAux;
	unsigned char ModNum;
	unsigned char Error;
	unsigned char NumId;
	
	unsigned long Mask;
	unsigned int Sen, AuxSen;
	
	
	union _UInt16 LocUserW00;
	unsigned long SN1,SN2;

//	if(*S!='>' || Proceso.B.fWifiConf)
	if(*S!='>')	// || Proceso.B.fWifiConf)
	{
		U2TxBuf[0]=0;
		U3TxBuf[0]=0;
		return;
	}
		
	memset(&QTxBuf,0x00,256); //strlen()
	
	P = QTxBuf;
	*P = '>';
	P++; 
	*P = 'R';
	P++;
	
	S++;
//-----------------------------------------------------------------------------
//Comando Bit de vida del dispositio con el que esta conectado.
	memset(Cmd,0x00,10);
	Movstr(Cmd,S);
	if(Check(Cmd,"KAV",sizeof("KAV")))
	{
		Sts_Tmr.CntWifi = 0;
		P +=Movstr(P,Cmd);
		P++;
		goto lFinComando;
			
	}
//-----------------------------------------------------------------------------
//Control si es un comando de lectura o de escritura
	if(*S=='S')
	{
		Com = true;
		S++;
	}
	else if(*S=='Q')
	{
		Com = false;
		S++;
	}
	else
	{
		P = TXTError(P);
		goto lFinComando;
	}
//-----------------------------------------------------------------------------	
	while(*S!=','&& *S!='<' && *S)
	{
		Cmd[i] = *S;
		*P = *S;
		P++;
		S++;
		i++;
	}
	*P = ',';
	P++; 	
	S++;
	

//------------------------------------------------------------------------------
//             SOLICITUD DE DATOS DE LOS PERIFERICOS A PEDIDO 
	if(Check(Cmd,"DT",sizeof("DT")))
	{

		memset(Cmd,0x00,10);
		P +=Movstr(P,S);
		*P = ',';
		P++;
		S += Movstr(Cmd,S);
		S++;
//-----------------------------------------------------------------------------		
		//Datos de las Moduladoras
		//Lectura de Velocidad Metros Recorridos y Pulsos Totales
		//Reset de los pulsos totales
		if(Check(Cmd,"MOD",sizeof("MOD")))
		{
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			Sen = atoi((char*)Cmd);
			if(Sen == 0 || Sen > kModMax)
			{
				P = TXTError(P);
				goto lFinComando;
			}
				
			Sen--;
			if(Com)
			{
				Moduladora[Sen].Pul	= 0;
				ModConfPul0(Sen);
			}
			ModLectPul0(Sen);
			//Numero de Sensor
			AuxSen = Sen + 1;
			P  = (unsigned char*)uitos(AuxSen,P);
			*P = ',';
			P++;
			//Velocidad
			P  = (unsigned char*)uitos(Moduladora[Sen].Vel,P);
			*P = ',';
			P++;
			//Metros Recorridos
			P  = (unsigned char*)ultos(Moduladora[Sen].Dis,P);
			*P = ',';
			P++;
			//Pulsos Totales
			P  = (unsigned char*)ultos(Moduladora[Sen].Pul,P);
			*P = ',';
			P++;
		}
//-----------------------------------------------------------------------------		
		///Lectura de los datos de los sensores de TRB
		else if (Check(Cmd,"TRB",sizeof("TRB")))
		{
			for(i=0;i<kTRBMax;i++)
			{	
				AuxSen = i + 1;
				P  = (unsigned char*)uitos(AuxSen,P);
				if( Turbina[i].Sts.B.Con)
				{
					if(!TRBLecPar00(i))
					{
						strcpy((char *)P,",ERR,ERR,ERR,");
						P=P + strlen(",ERR,ERR,ERR,");
					}
					else
					{
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Turbina[i].FK,P);	
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Turbina[i].AlMin,P);	
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Turbina[i].AlMax,P);	
						*P = ',';
						P++;
					}
				}
				else
				{
				//	P +=Movstr(P,",,,,");
					strcpy((char *)P,",,,,");
					P=P + strlen(",,,,");	
				}
			}
		}
//-----------------------------------------------------------------------------
// Lectura de los Parametros de todos los sensores de rotación
		else if (Check(Cmd,"ROT",sizeof("ROT")))
		{
			
			for(i=0;i<kROTMax;i++)
			{	
				AuxSen = i + 1;
				P  = (unsigned char*)uitos(AuxSen,P);
				if( Rotacion[i].Sts.B.Con)
				{
					if(!RotLecPar00(i))
					{
						strcpy((char *)P,",ERR,ERR,ERR,");
						P=P + strlen(",ERR,ERR,ERR,");
					}
					else
					{
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Rotacion[i].FK,P);	
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Rotacion[i].AlMin,P);	
						*P = ',';
						P++;
						P  = (unsigned char*)ultos(Rotacion[i].AlMax,P);	
						*P = ',';
						P++;
					}
				}
				else
				{
				//	P +=Movstr(P,",,,,");
					strcpy((char *)P,",,,,");
					P=P + strlen(",,,,");	
				}
			}
		}
//-----------------------------------------------------------------------------
		//Comando erroneo salgo
		else
		{
			P = TXTError(P);
			goto lFinComando;
		}
	}
//------------------------------------------------------------------------------
//                      PARAMETROS DE AJUSTE  
	else if(Check(Cmd,"AJ",sizeof("AJ")))
	{
		memset(Cmd,0x00,10);
		P +=Movstr(P,S);
		*P = ',';
		P++;
		S += Movstr(Cmd,S);
		S++;
//----------CONFIGURACION MODO DE SIEMBRA---------------------------------------
//><	
//>XAJ,MS,MODO DE SIEMBRA<		
		if(Check(Cmd,"MS",sizeof("MS")))
		{
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				DtsSiembra.ModSie = ModSiembra(Cmd);
				EepromWRBuf(M_DATOS_SEMBRADORA,(unsigned char *)&DtsSiembra,sizeof(struct _DtsSiembra));
			}
			EepromRDBuf(M_DATOS_SEMBRADORA,(unsigned char *)&DtsSiembra,sizeof(struct _DtsSiembra));
			P  = ModSiembraTx(P);
			*P = ',';
			P++;
		}
//------------------------------------------------------------------------------
//Programacion de la velocidad de siembra para simulador borrar cuando no se utilice mas
//		else if(Check(Cmd,"VEL",sizeof("VEL")))
//		{
//			if(Com)
//			{
//				memset(Cmd,0x00,10);
//				S += Movstr(Cmd,S);
//				S++;
//				VelTest = (float)(atoi((char*)Cmd))/10;
//				
//			}
//			P  =  ftos(VelTest,S,3,1);	//Velocidad
//			*P = ',';
//			P++;
//			
//		}
//------------------------------------------------------------------------------
// Parametros de Ajuste para Alta densidad
		else if(Check(Cmd,"CAD",sizeof("CAD")))
		{
			if(Com)
			{
				CorrAlDen.B.V[0] = (char)"A";
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				CorrAlDen.B.V[1] = (atoi((char*)Cmd));
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				CorrAlDen.B.V[2] = (atoi((char*)Cmd));
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				CorrAlDen.B.V[3] = (atoi((char*)Cmd));
				EepromWRBuf(M_FAC_AJS_ALTA_DENS,(unsigned char *)&CorrAlDen,sizeof(union _UInt32));
				
			}
			EepromRDBuf(M_FAC_AJS_ALTA_DENS,(unsigned char *)&CorrAlDen,sizeof(union _UInt32));
			P  =  (unsigned char*)uitos((unsigned int)CorrAlDen.B.V[1],P);
			*P = ',';
			P++;
			P  =  (unsigned char*)uitos((unsigned int)CorrAlDen.B.V[2],P);
			*P = ',';
			P++;			
			P  =  (unsigned char*)uitos((unsigned int)CorrAlDen.B.V[3],P);
			*P = ',';
			P++;			
		}					
//------------------PARAMETROS DE SIEMBRA---------------------------------------
//>XMS<	
//>RMS,MODO DE SIEMBRA<		
		else if(Check(Cmd,"EST",sizeof("EST")))
		{
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				if(Check(Cmd,"RSPAR",sizeof("RSPAR")))
				{
					DtsSiembra.tTrabPar	= 0;
					DtsSiembra.DistRecPar	= 0;
					DtsSiembra.SupSemPar	= 0;
				}
				else if(Check(Cmd,"RSTOT",sizeof("RSTOT")))
				{
					DtsSiembra.tTrabTot	= 0;
					DtsSiembra.DistRecTot	= 0;
					DtsSiembra.SupSemTot	= 0;
				}
				else if(Check(Cmd,"RSVELP",sizeof("RSVELP")))
				{
					DtsSiembra.VelProm = 0;
				}
				else if(Check(Cmd,"RSVELM",sizeof("RSVELM")))
				{
					DtsSiembra.VelMax	= 0;
				}
			}
			P  = (unsigned char*)ultos(DtsSiembra.tTrabTot,P);
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(DtsSiembra.DistRecTot,P);
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(DtsSiembra.SupSemTot,P);
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(DtsSiembra.tTrabPar,P);
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(DtsSiembra.DistRecPar,P);
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(DtsSiembra.SupSemPar,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(DtsSiembra.VelProm,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(DtsSiembra.VelMax,P);	
			*P = ',';
			P++;
		}

//-----------------------------------------------------------------------------
//HABILITACION DESHABILITACION DE SENSORES PARA PROCESO

		else if(Check(Cmd,"HAB",sizeof("HAB")) || Check(Cmd,"DSH",sizeof("DSH")))
		{
			dsh = false;			
			if( Check(Cmd,"DSH",sizeof("DSH")))
				dsh = true;
				
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				
				if(Check(Cmd,"SIEM",sizeof("SIEM")))
				{
					if(*S=='<')
					{
						SenDtsHab.SemB1 = 0xFFFFFFFF;
						SenDtsHab.SemB2 = 0xFFFFFFFF;
						
					}
					else
					{
						S++;
						while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
						{
							memset(Cmd,0x00,10);
							S += Movstr(Cmd,S);
							S++;
							Sen = atoi((char*)Cmd);
							if(Sen>64) continue;	
							Sen--;
							Mask = 1;
							
							if(Sen<32)
							{
								Mask = Mask << Sen;
								if(dsh)
								{
									SenDtsHab.SemB1 &= (~Mask);
								}
								else
								{
									SenDtsHab.SemB1 |= Mask;
								}
							}
							else
							{
								Sen = Sen - 32;
								Mask = Mask << Sen;
								if(dsh)
								{
									SenDtsHab.SemB2 &= (~Mask);
								}
								else
								{
									SenDtsHab.SemB2 |= Mask;
								}
							}
						}
					}
					EepromWRBuf(M_STS_HAB_SEN,(unsigned char *)&SenDtsHab,sizeof(struct _SenDts));
				}
				else if(Check(Cmd,"FERT",sizeof("FERT")))
				{
					if(*S=='<')
					{
						SenDtsHab.FerB1 = 0xFFFFFFFF;
						SenDtsHab.FerB2 = 0xFFFFFFFF;	
					}
					else
					{
						S++;
						while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
						{
							memset(Cmd,0x00,10);
							S += Movstr(Cmd,S);
							S++;
							Sen = atoi((char*)Cmd);
							if(Sen>64) continue;	
							Sen--;
							Mask = 1;
							
							if(Sen<32)
							{
								Mask = Mask << Sen;
								if(dsh)
								{
									SenDtsHab.FerB1 &= (~Mask);
								}
								else
								{
									SenDtsHab.FerB1 |= Mask;
								}
							}
							else
							{
								Sen = Sen - 32;
								Mask = Mask << Sen;
								if(dsh)
								{
									SenDtsHab.FerB2 &= (~Mask);
								}
								else
								{
									SenDtsHab.FerB2 |= Mask;
								}
							}
						}
					}
					EepromWRBuf(M_STS_HAB_SEN,(unsigned char *)&SenDtsHab,sizeof(struct _SenDts));
				}			
				else if(Check(Cmd,"MOD",sizeof("MOD")))
				{
					S++;
					while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
					{
						memset(Cmd,0x00,10);
						S += Movstr(Cmd,S);
						S++;
						Sen = atoi((char*)Cmd);
						if(Sen>16) continue;	
						Sen--;
						Mask = 1;				
						Mask = Mask << Sen;
						if(dsh)
						{
							
							HabPer.MOD &= (int)(~Mask);
						}
						else
						{
							HabPer.MOD |= (int) Mask;
						}
					}
					EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
					
				}
				else if(Check(Cmd,"ROT",sizeof("ROT")))
				{
					S++;
					while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
					{
						memset(Cmd,0x00,10);
						S += Movstr(Cmd,S);
						S++;
						Sen = atoi((char*)Cmd);
						if(Sen>8) continue;	
						Sen--;
						Mask = 1;
						
						Mask = Mask << Sen;
						if(dsh)
						{
							HabPer.ROT &= (int)(~Mask);
						}
						else
						{
							HabPer.ROT |= (int) Mask;
						}
					}
					EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
				}
				else if(Check(Cmd,"TRB",sizeof("TRB")))
				{
					S++;
					while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
					{
						memset(Cmd,0x00,10);
						S += Movstr(Cmd,S);
						S++;
						Sen = atoi((char*)Cmd);
						if(Sen>3) continue;	
						Sen--;
						Mask = 1;
						
						Mask = Mask << Sen;
						if(dsh)
						{
							HabPer.TRB &= (int)(~Mask);
						}
						else
						{
							HabPer.TRB |= (int) Mask;
						}
					}
					HabPer.TRB &= 0x07;

					EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
				}
				else if(Check(Cmd,"TLV",sizeof("TLV")))
				{
					S++;
					while(*S!='<' && *S && *S!=0x0A && *S!= 0x0D)
					{
						memset(Cmd,0x00,10);
						S += Movstr(Cmd,S);
						S++;
						Sen = atoi((char*)Cmd);
						if(Sen>16) continue;	
						Sen--;
						Mask = 1;
						
						Mask = Mask << Sen;
						if(dsh)
						{
							HabPer.TOL &= (int)(~Mask);
						}
						else
						{
							HabPer.TOL |= (int) Mask;
						}
					}

					EepromWRBuf(M_STS_HAB_PER,(unsigned char *)&HabPer,sizeof(struct _DtsPer));
				}
				else
				{
					P = TXTError(P);
					goto lFinComando;
				}
				CargaConfSen();	
				CargaConfPer();									
			}
			P += Movstr(P,(unsigned char*)"START");
			FinCmd(P);
			CBuffersTx();		
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer64 ("SIEM",P,SenDtsHab.SemB1,SenDtsHab.SemB2);
			CBuffersTx();
			
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();			
			
			P = QTxBuf;
			StsPer64 ("FERT",P,SenDtsHab.FerB1,SenDtsHab.FerB2);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();			
			
			P = QTxBuf;
			StsPer16 ("MOD",P,HabPer.MOD);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer16 ("ROT",P,HabPer.ROT);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();			

			P = QTxBuf;
			StsPer16 ("TRB",P,HabPer.TRB);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
				
			P = QTxBuf;
			strcpy((char *)P,">QAJ,HAB,STOP<");
			P = P + strlen(">QAJ,HAB,STOP<");
		}
//-----------------------------------------------------------------------------
		//Configuro y leo el SP y el KD de todo el sistema de moduladoras
		else if(Check(Cmd,"SIS",sizeof("SIS")) )
		{
			P--;
			for(a=0;a<4;a++)
			{
			//	*P = ',';
			//	P++;
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				//Siembra Primaria
				if(Check(Cmd,"SP",sizeof("SP")) )
				{
					ModNum = 1;
				//	strcpy((char *)P,"SP");
				//	P = P + strlen("SP");
				}
				
				//Siembra Secundaria
				else if(Check(Cmd,"SS",sizeof("SS")) )
				{
					ModNum = 2;
				//	strcpy((char *)P,"SS");
				//	P = P + strlen("SS");
				}
					
				//Fertilizante Primario
				else if(Check(Cmd,"FP",sizeof("FP")) )
				{
					ModNum = 3;
				//	strcpy((char *)P,"FP");
				//	P = P + strlen("FP");
				}	
					
				//Fertilizante Secundario
				else if(Check(Cmd,"FS",sizeof("FS")) )
				{
					ModNum = 4;
				//	strcpy((char *)P,"FS");
				//	P = P + strlen("FS");
				}	
				else
				{
					P = TXTError(P);
					goto lFinComando;
				}	
					
				ModNum --;
				if(Com)
				{
					memset(Cmd,0x00,10);
					//Set Point
					S += Movstr(Cmd,S);
					S++;
					SPAux = atoi((char*)Cmd);
					//Constante KD
					memset(Cmd,0x00,10);
					S += Movstr(Cmd,S);
					S++;
					KDAux = atoi((char*)Cmd);
						
					for(i=0;i<16;i=i+4)
					{
						Sen = i + ModNum;
						if(	Moduladora[Sen].Sts.B.Det)
						{
							//P--;
						//	FinCmd(P);
						//	P=P+3;
							*P = ',';
							P++;
							//Numero de Sensor
						//	strcpy((char *)P,"MOD,");
						//	P = P + strlen("MOD,");
							AuxSen = Sen + 1;
							P  = (unsigned char*)uitos(AuxSen,P);
							*P = ',';
							P++;
							if(ModSistKpKd(Sen,SPAux,KDAux,true))
							{
								P  = (unsigned char*)uitos(Moduladora[Sen].SP,P);	
								*P = ',';
								P++;
								P  = (unsigned char*)uitos(Moduladora[Sen].KD,P);
						//		P++;
							}
							else
							{
							//	P = TXTError(P);
							//	P--;
								strcpy((char *)P,"ERR,ERR");
								P = P + strlen("ERR,ERR");	
							}	
						}
						else
						{
							//P--;
						//	FinCmd(P);
						//	P=P+3;
							*P = ',';
							P++;
				//			strcpy((char *)P,"MOD,");
				//			P = P + strlen("MOD,");
							AuxSen = Sen + 1;
							P  = (unsigned char*)uitos(AuxSen,P);
							*P = ',';
							P++;
							*P = ',';
							P++;
						//	strcpy((char *)P,"DESC");
						//	P = P + strlen("DESC");	
						}
					}
				}
			
				else
				{	
					for(i=0;i<16;i=i+4)
					{
						Sen = i + ModNum;
						if(	Moduladora[Sen].Sts.B.Det)
						{
						//	P--;
					//		FinCmd(P);
					//		P=P+3;
					//		//Numero de Sensor
					//		strcpy((char *)P,"MOD,");
					//		P = P + strlen("MOD,");
							AuxSen = Sen + 1;
							P  = (unsigned char*)uitos(AuxSen,P);
							*P = ',';
							P++;
							ModSistKpKd(Sen,SPAux,KDAux,false);
							P  = (unsigned char*)uitos(Moduladora[Sen].SP,P);	
							*P = ',';
							P++;
							P  = (unsigned char*)uitos(Moduladora[Sen].KD,P);	
						}
						else
						{
						//	P--;
					//		FinCmd(P);
					//		P=P+3;
					//		strcpy((char *)P,"MOD,");
					//		P = P + strlen("MOD,");
							AuxSen = Sen + 1;
							P  = (unsigned char*)uitos(AuxSen,P);
							*P = ',';
							P++;
							*P = ',';
							P++;
						//	strcpy((char *)P,"DESC");
						//	P = P + strlen("DESC");
							
						}
					}
				}
			//	FinCmd(P);
			//	P=P+3;
			}
		}
////-----------------------------------------------------------------------------
//		//Configuro y leo el SP y el KD de todo el sistema
//		else if(Check(Cmd,"SIST",sizeof("SIST")) )
//		{
//			memset(Cmd,0x00,10);
//			S += Movstr(Cmd,S);
//			S++;
//			//Siembra Primaria
//			if(Check(Cmd,"SP",sizeof("SP")) )
//			{
//				ModNum = 1;
//				strcpy((char *)P,"SP");
//				P = P + strlen("SP");
//			}
//			
//			//Siembra Secundaria
//			else if(Check(Cmd,"SS",sizeof("SS")) )
//			{
//				ModNum = 2;
//				strcpy((char *)P,"SS");
//				P = P + strlen("SS");
//			}
//				
//			//Fertilizante Primario
//			else if(Check(Cmd,"FP",sizeof("FP")) )
//			{
//				ModNum = 3;
//				strcpy((char *)P,"FP");
//				P = P + strlen("FP");
//			}	
//				
//			//Fertilizante Secundario
//			else if(Check(Cmd,"FS",sizeof("FS")) )
//			{
//				ModNum = 4;
//				strcpy((char *)P,"FS");
//				P = P + strlen("FS");
//			}	
//			else
//			{
//				P = TXTError(P);
//				goto lFinComando;
//			}	
//				
//			memset(Cmd,0x00,10);
//			//Set Point
//			S += Movstr(Cmd,S);
//			S++;
//			SPAux = atoi((char*)Cmd);
//			//Constante KD
//			memset(Cmd,0x00,10);
//			S += Movstr(Cmd,S);
//			S++;
//			KDAux = atoi((char*)Cmd);
//			ModNum --;
//			if(Com)
//			{	
//				for(i=0;i<16;i=i+4)
//				{
//					Sen = i + ModNum;
//					if(	Moduladora[Sen].Sts.B.Det)
//					{
//						//P--;
//						FinCmd(P);
//						P=P+3;
//						//Numero de Sensor
//						strcpy((char *)P,"MOD,");
//						P = P + strlen("MOD,");
//						AuxSen = Sen + 1;
//						P  = (unsigned char*)uitos(AuxSen,P);
//						*P = ',';
//						P++;
//						if(ModSistKpKd(Sen,SPAux,KDAux,true))
//						{
//							P  = (unsigned char*)uitos(Moduladora[Sen].SP,P);	
//							*P = ',';
//							P++;
//							P  = (unsigned char*)uitos(Moduladora[Sen].KD,P);
//					//		P++;
//						}
//						else
//						{
//							P = TXTError(P);
//							P--;
//						}	
//					}
//					else
//					{
//						//P--;
//						FinCmd(P);
//						P=P+3;
//						strcpy((char *)P,"MOD,");
//						P = P + strlen("MOD,");
//						AuxSen = Sen + 1;
//						P  = (unsigned char*)uitos(AuxSen,P);
//						*P = ',';
//						P++;
//						strcpy((char *)P,"DESCONECTADA");
//						P = P + strlen("DESCONECTADA");	
//					}
//				}
//			}
//			else
//			{	
//				for(i=0;i<16;i=i+4)
//				{
//					Sen = i + ModNum;
//					if(	Moduladora[Sen].Sts.B.Det)
//					{
//					//	P--;
//						FinCmd(P);
//						P=P+3;
//						//Numero de Sensor
//						strcpy((char *)P,"MOD,");
//						P = P + strlen("MOD,");
//						AuxSen = Sen + 1;
//						P  = (unsigned char*)uitos(AuxSen,P);
//						*P = ',';
//						P++;
//						ModSistKpKd(Sen,SPAux,KDAux,false);
//						P  = (unsigned char*)uitos(Moduladora[Sen].SP,P);	
//						*P = ',';
//						P++;
//						P  = (unsigned char*)uitos(Moduladora[Sen].KD,P);	
//					}
//					else
//					{
//					//	P--;
//						FinCmd(P);
//						P=P+3;
//						strcpy((char *)P,"MOD,");
//						P = P + strlen("MOD,");
//						AuxSen = Sen + 1;
//						P  = (unsigned char*)uitos(AuxSen,P);
//						*P = ',';
//						P++;
//						strcpy((char *)P,"DESCONECTADA");
//						P = P + strlen("DESCONECTADA");
//						
//					}
//				}
//			}
//			P++;
//		}
//-----------------------------------------------------------------------------			
		//Configuro el SP y el KD para cada moduladora individualmente
		else if(Check(Cmd,"MOD",sizeof("MOD")) )
		{	
			//Numero de Sensor
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			Sen = atoi((char*)Cmd);
				
			if(Sen == 0 || Sen > kModMax)
			{
				P = TXTError(P);
				goto lFinComando;
			}
			Sen--;
			
			if(Com)
			{	
				//Set Point
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Moduladora[Sen].SP = atoi((char*)Cmd);
				
				//Constante KD
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Moduladora[Sen].KD = atoi((char*)Cmd);
				
				ModAjtSPKD0(Sen);
			}
			
			ModLecSPKD0(Sen);
			
			//Numero de Sensor
			AuxSen = Sen + 1;
			P  = (unsigned char*)uitos(AuxSen,P);
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(Moduladora[Sen].SP,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(Moduladora[Sen].KD,P);	
			P++;  	
		}
		else
		{
			P = TXTError(P);
			goto lFinComando;
		}
	}
//------------------------------------------------------------------------------
//              PARAMETROS DE CONFIGURACION DE LA SEMBRADORA 
	else if(Check(Cmd,"CN",sizeof("CN")))
	{
		P += Movstr(P,S);
		*P = ',';
		P++;
		memset(Cmd,0x00,10);
		S += Movstr(Cmd,S);
		S++;
//-----------------------------------------------------------------------------		
		//DETIENE LA TRANSMICION DE DATOS AL TERMINAL
		if(Check(Cmd,"STOP",sizeof("STOP")))
		{
			Proceso.B.fStopDts 	= true;
			Proceso.B.fT2xCOM 	= true;
			Proceso.B.fT3xCOM	= true;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = false;
		}
//-----------------------------------------------------------------------------		
		//COMIENZA LA TRANSMICION DE DATOS AL TERMINAL
		else if(Check(Cmd,"RUN",sizeof("RUN")))
		{
			Proceso.B.fStopDts = false;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = false;
			Proceso.B.fSetId = false;
		}
//-----------------------------------------------------------------------------		
		//CONECTA EL MODULO WIFI CON EL PUERTO 232
		else if(Check(Cmd,"WIFION",sizeof("WIFION")))
		{
			Proceso.B.fStopDts = true;
			Proceso.B.fWifiConf = true;
			Proceso.B.fGPSLec = false;
		}
//-----------------------------------------------------------------------------		
		//DESCONECTA EL MODULO WIFI CON DEL PUERTO 232
		else if(Check(Cmd,"WIFIOFF",sizeof("WIFIOFF")))
		{
			Proceso.B.fStopDts = false;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = false;
		}
//-----------------------------------------------------------------------------
		//Saca los datos del puerto serie por los puertos de comunicación
		else if(Check(Cmd,"GPSON",sizeof("GPSON")))
		{
			Proceso.B.fStopDts = true;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = true;
		}
//-----------------------------------------------------------------------------		
		//DESCONECTA EL MODULO WIFI CON DEL PUERTO 232
		else if(Check(Cmd,"GPSOFF",sizeof("GPSOFF")))
		{
			Proceso.B.fStopDts = false;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = false;

		}
//-----------------------------------------------------------------------------
		//Transmite los datos de satelietes por comunicación
		else if(Check(Cmd,"GPSGANON",sizeof("GPSGANON")))
		{
			Proceso.B.fDtsSat = true;
		}
//-----------------------------------------------------------------------------		
		//Deja de transmitir los datos de los satelites
		else if(Check(Cmd,"GPSGANOFF",sizeof("GPSGANOFF")))
		{
			Proceso.B.fDtsSat = false;
		}
//-----------------------------------------------------------------------------
		//CARGA EL FACTOR DE CORRECCION HORARIA
		else if(Check(Cmd,"GTM",sizeof("GTM")))
		{
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				TimeZone = atoi((char*)Cmd);	
				EepromWRBuf(M_AJS_TIME_ZONE,&TimeZone,sizeof(TimeZone));
			}
			P  = (unsigned char*)sitos(TimeZone,P);	//Ganancia promedio de los satelies
			P++; 			
		}
//-----------------------------------------------------------------------------		
//           CONFIGURACION DE PARAMETROS DE SIEMBRA 
//>XCONF,ModSiembra,NumeroSurcos,SepSurcos,AnchoSiembra<
		else if(Check(Cmd,"CONF",sizeof("CONF")))
		{
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				DtsSiembra.ModSie = ModSiembra(Cmd);
				S++;
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				DtsSiembra.Surcos = atoi((char*)Cmd);
				S++;
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				DtsSiembra.SepSurco = atoi((char*)Cmd);
				S++;
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				DtsSiembra.Ancho = atoi((char*)Cmd);
				EepromWRBuf(M_DATOS_SEMBRADORA,(unsigned char *)&DtsSiembra,sizeof(struct _DtsSiembra));				
			}
			P  = ModSiembraTx(P);	
			*P = ',';
			P++; 
			P  = (unsigned char*)uitos(DtsSiembra.Surcos,P);	
			*P = ',';
			P++; 
			P  = (unsigned char*)uitos(DtsSiembra.SepSurco,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(DtsSiembra.Ancho,P);	
			P++;  			
		}
//-----------------------------------------------------------------------------
//REDETECCION DE LOS SENSORES 
		else if(Check(Cmd,"RDT",sizeof("RDT")))
		{
			P += Movstr(P,(unsigned char*)"START");
			FinCmd(P);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			if(Com)
			{
				SenRedetecion();
				
				EepromWRBuf(M_STS_CON_SEN,(unsigned char *)&SenDtsCon,sizeof(struct _SenDts));
				EepromWRBuf(M_STS_CON_PER,(unsigned char *)&ConPer,sizeof(struct _DtsPer));
				EepromWRBuf(M_STS_BUS_PER,(unsigned char *)&BusPer,sizeof(struct _DtsPer));	
			}
			P = QTxBuf;
			StsPer64 ("SIEM",P,SenDtsCon.SemB1,SenDtsCon.SemB2);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer64 ("FERT",P,SenDtsCon.FerB1,SenDtsCon.FerB2);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer16 ("MOD",P,ConPer.MOD);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer16 ("ROT",P,ConPer.ROT);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer16 ("TRB",P,ConPer.TRB);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			StsPer16 ("TOL",P,ConPer.TOL);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			
			P = QTxBuf;
			strcpy((char *)P,">RCN,RDT,STOP<");
			P = P + strlen(">RCN,RDT,STOP<");
		}
//-----------------------------------------------------------------------------
//CARGA LA CONFIGURACION A LA MODULADORA
		else if(Check(Cmd,"MOD",sizeof("MOD")) )
		{
			//Numero de Sensor
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			Sen = atoi((char*)Cmd);
			P  = (unsigned char*)uitos(Sen,P);
			*P = ',';
			P++; 
			if(Sen)
			{
				Sen--;
			}	
			if(!Moduladora[Sen].Sts.B.Det || (Sen > kModMax))
			{
			///	P = TXTError(P);
				P = (unsigned char*)EstadosCN("HAB",P);
				goto lFinComando;
			}
			if(Com)
			{						
				//Dientes Eje Traccion
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Moduladora[Sen].PVT = atoi((char*)Cmd);
				
				//Dientes Eje Distribucion
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Moduladora[Sen].PVD =(unsigned char) atoi((char*)Cmd);
				
				//Relacion de Transmicion
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Moduladora[Sen].RDT = atoi((char*)Cmd);
				
				ModConfParam(Sen);
			}
			Error =ModLectParam(Sen);
			if(!Error)
			{
				P = (unsigned char*)EstadosCN("COM",P);
				goto lFinComando;	
			}
			else if (Error ==2)
			{
				P = (unsigned char*)EstadosCN("ESC",P);
			}
			else
			{
			//Estado 
				P = (unsigned char*)EstadosCN("OK",P);
			}
			//Numero de Sensor
			P  = (unsigned char*)uitos(Moduladora[Sen].PVT,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(Moduladora[Sen].PVD,P);
			*P = ',';
			P++;
			P  = (unsigned char*)uitos(Moduladora[Sen].RDT,P);	
			P++;  	
		}
//-----------------------------------------------------------------------------		
		//CONFIGURACION MODULO ROTACION
		else if(Check(Cmd,"ROT",sizeof("ROT")))
		{
			//Numero de Sensor
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			Sen = atoi((char*)Cmd);
			P  = (unsigned char*)uitos(Sen,P);
			*P = ',';
			P++;
			Sen --;
			if(!Rotacion[Sen].Sts.B.Det || Sen > kModMax)
			{
			//	P = TXTError(P);
				P = (unsigned char*)EstadosCN("HAB",P);
				goto lFinComando;
			}
			if(Com)
			{			
				 
			//	Sen--;
				
				//Factor K
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Rotacion[Sen].FK = atol((char*)Cmd);
				RotEscPar00(Sen);
			}
			Error = RotLecPar00(Sen);
			if(!Error)
			{
				P = (unsigned char*)EstadosCN("COM",P);
				goto lFinComando;	
			}
			else if (Error ==2)
			{
				P = (unsigned char*)EstadosCN("ESC",P);
			}
			else
			{
			//Estado 
				P = (unsigned char*)EstadosCN("OK",P);
			}
			//Numero de Sensor
	//		AuxSen = Sen + 1;
	//		P  = (unsigned char*)uitos(AuxSen,P);
	//		*P = ',';
	//		P++;
			 
			P  = (unsigned char*)ultos(Rotacion[Sen].FK,P);	
			*P = ',';
			P++;	
		}
//-----------------------------------------------------------------------------
		//CONFIGURACION MODULO TRB (Turbina)
		else if(Check(Cmd,"TRB",sizeof("TRB")))
		{
			//Numero de Sensor
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			Sen = atoi((char*)Cmd);
			P  = (unsigned char*)uitos(Sen,P);
			*P = ',';
			P++;
			Sen--;
			if(!Turbina[Sen].Sts.B.Det || Sen > kModMax)
			{
			//	P = TXTError(P);
				P = (unsigned char*)EstadosCN("HAB",P);
				goto lFinComando;
			}
			if(Com)
			{	
			//	Sen--;
				//Factor K
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Turbina[Sen].FK = atol((char*)Cmd);
				
				//Nivel Alarma Minima
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Turbina[Sen].AlMin = atol((char*)Cmd);
				
				//Nivel Alarma Maxima
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Turbina[Sen].AlMax = atol((char*)Cmd);
				
				TRBEscPar00(Sen);
			}
			Error = TRBLecPar00(Sen);
			if(!Error)
			{
				P = (unsigned char*)EstadosCN("COM",P);
				goto lFinComando;	
			}
			else if (Error ==2)
			{
				P = (unsigned char*)EstadosCN("ESC",P);
			}
			else
			{
			//Estado 
				P = (unsigned char*)EstadosCN("OK",P);
			}
			//Numero de Sensor
		//	AuxSen = Sen + 1;
		//	P  = (unsigned char*)uitos(AuxSen,P);
		//	*P = ',';
		//	P++;
			P  = (unsigned char*)ultos(Turbina[Sen].FK,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(Turbina[Sen].AlMin,P);	
			*P = ',';
			P++;
			P  = (unsigned char*)ultos(Turbina[Sen].AlMax,P);	
			*P = ',';
			P++;	
		}
//-----------------------------------------------------------------------------
//CONFIGURACION DE VELOCIDAD DE TRASMICION DE LA UART-232 Y DEL MODULO WIFI
		else if(Check(Cmd,"UART",sizeof("UART")))
		{
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			EepromRDBuf(M_BAUDRATE,&LocUserW00,sizeof(union _UInt16));
			P += Movstr(P,(unsigned char*)"START");
			P = FinCmd(P);
			
			if(Com)
			{
				P += Movstr(P,(unsigned char*)">Actual");
				P = FinCmd(P);
				strcpy((char *)P,">RS232,Baudrate:");
				P = P + strlen(">RS232,Baudrate:");
				P = BauToStr(P,LocUserW00.B.V[0]);
				P = FinCmd(P);
				strcpy((char *)P,">WIFI,Baudrate:");
				P = P + strlen(">WIFI,Baudrate:");
				P = BauToStr(P,LocUserW00.B.V[1]);
				P = FinCmd(P);
				P += Movstr(P,(unsigned char*)">Nueva");
				P = FinCmd(P);
			
				if(Check(Cmd,"RS232",sizeof("RS232")))
				{
					///Cambiar valores de estos
					memset(Cmd,0x00,10);
					S += Movstr(Cmd,S);
					S++;
					Sen = atoi((char*)Cmd);
				//	Sen--;
					if(Sen <= 6 )
						LocUserW00.B.V[0] = Sen;
					else
					{
						P = TXTError(P);
						goto lFinComando;
					}
				}
				else if(Check(Cmd,"WIFI",sizeof("WIFI")))
				{
					memset(Cmd,0x00,10);
					S += Movstr(Cmd,S);
					S++;
					Sen = atoi((char*)Cmd);
					//CONVIEN ESTO O EL VALOR DEL BAUDRATE????
					if(Sen <= 6 )
						LocUserW00.B.V[1] = Sen; 
					else
					{
						P = TXTError(P);
						goto lFinComando;
					}	
				}
				else
				{
					P = TXTError(P);
					goto lFinComando;
				}
				EepromWRBuf(M_BAUDRATE,(unsigned char *)&LocUserW00,sizeof(union _UInt16));
				while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();
			}
			strcpy((char *)P,">RS232,Baudrate:");
			P = P + strlen(">RS232,Baudrate:");
			P = BauToStr(P,LocUserW00.B.V[0]);
			P = FinCmd(P);
			strcpy((char *)P,">WIFI,Baudrate:");
			P = P + strlen(">WIFI,Baudrate:");
			P = BauToStr(P,LocUserW00.B.V[1]);
			P = FinCmd(P);
			strcpy((char *)P,">RCM,UART,STOP");
			P = P + strlen(">RCM,UART,STOP");
			P = FinCmd(P);
			CBuffersTx();
			while(!U2STAbits.TRMT || !U3STAbits.TRMT) ExeTask();

			U2BRG = SCI_SetBaud(LocUserW00.B.V[0]);
			U3BRG = SCI_SetBaud(LocUserW00.B.V[1]);	
			P = QTxBuf;
			*P = 0;
		}
		//-----------------------------------------------------------------------------
//Carga del Numero de serie de los sensores		
		else if(Check(Cmd,"MGPS",sizeof("MGPS")))
		{
			if(Com)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				if(Check(Cmd,"2",sizeof("2")))
				{
					RPINR18bits.U1RXR = RP11;	//Puerto Auxiliar
					RPOR12bits.RP24R = U1TX_IO; //Puerto Auxiliar 
					MGPS.Puerto = 2; 
				}
				else
				{
					RPOR2bits.RP4R = U1TX_IO;
					RPINR18bits.U1RXR = RP2;
					MGPS.Puerto = 1;
				}
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				Sen = atoi((char*)Cmd);
				if(Sen <= 6 )
				{
					U1BRG = SCI_SetBaud(Sen);
					MGPS.Baudrate = Sen;	
				}
				else
				{
					U1BRG = SCI_SetBaud(1);
					MGPS.Baudrate = 1;
				}
				EepromWRBuf(M_GPS_AUX,(unsigned char *)&MGPS,sizeof(struct _MGPS));
			}
			strcpy((char *)P,"PUERTO:");
			P = P + strlen("PUERTO:");
			P = uitos(MGPS.Puerto,P);	//
			strcpy((char *)P,",Baudrate:");
			P = P + strlen(",Baudrate:");
			P = BauToStr(P,MGPS.Baudrate);
			P = FinCmd(P);
		}	
			
//-----------------------------------------------------------------------------
//Carga del Numero de ID de los sensores		
		else if(Check(Cmd,"DISPON",sizeof("DISPON")))
		{
		//	Proceso.B.fWifiConf = false;
		//	Proceso.B.fGPSLec = false;
			Proceso.B.fSetId = true;
			SetId.IdMax = 0;
			SetId.IdMin = 0;
			SetId.Id	= 0;
			SetId.NuevoId = 0;
			SetId.VoidId	= 0;
			SetId.StsBus.GrabaID = false;
			SetId.StsBus.Libre = false;
			SetId.StsBus.Ocupado = true;
			SetId.StsBus.SenEsc = false;
			SetId.StsBus.SenLec = false;
			SetId.StsBus.Sensor = false;			
		}
		else if(Check(Cmd,"DISPOFF",sizeof("DISPOFF")))
		{
			Proceso.B.fStopDts = false;
			Proceso.B.fWifiConf = false;
			Proceso.B.fGPSLec = false;
			Proceso.B.fSetId = false;
			Proceso.B.fInicio = false;
		}
		else if(Check(Cmd,"DISP",sizeof("DISP")))
		{
			if(!SetId.StsBus.Ocupado & !SetId.StsBus.Libre & SetId.StsBus.Sensor)
			{
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				if(Check(Cmd,"SEM",sizeof("SEM")))
				{
					if(SetId.VoidId != 0xF0)
						goto lFallaCargaNum;
					
					SetId.IdMax = 0x1F;
					SetId.IdMin = 0x00;
				}
				else if(Check(Cmd,"FER",sizeof("FER")))
				{
					if(SetId.VoidId != 0xF0)
						goto lFallaCargaNum;
					
					SetId.IdMax = 0x3F;
					SetId.IdMin = 0x20;
				}
				else if(Check(Cmd,"TLB",sizeof("TLB")))
				{
					if(SetId.VoidId != 0xF2)
						goto lFallaCargaNum;
				}
				else if(Check(Cmd,"ROT",sizeof("ROT")))
				{
					if(SetId.VoidId != 0xF1)
						goto lFallaCargaNum;	
				}	
				else if(Check(Cmd,"TRB",sizeof("TRB")))
				{
					if( SetId.IdMin != 0xD3)
						goto lFallaCargaNum;	
				}			
				memset(Cmd,0x00,10);
				S += Movstr(Cmd,S);
				S++;
				NumId = atoi((char*)Cmd);
				if(NumId)
				{
					NumId--;
					SetId.NuevoId = SetId.IdMin + NumId;
				}
				else
				{
					SetId.NuevoId = SetId.VoidId;
				}
				if((SetId.NuevoId <= SetId.IdMax) | (SetId.NuevoId == SetId.VoidId))
				{
					SetId.StsBus.SenEsc = true;
					SetId.StsBus.GrabaID = true;
				}
				else
				{
lFallaCargaNum:
					P = TXTError(P);
					goto lFinComando;
				}
			}
			else
			{
				P = TXTError(P);
				goto lFinComando;
			}	
		}
//-----------------------------------------------------------------------------
		else
		{
			P = TXTError(P);
			goto lFinComando;
		}
	}
//-----------------------------------------------------------------------------
//LECTURA / ESCRITURA EN LA MEMORIA EEPROM
//ESTE COMANDO NO SALE SIN TRANSMITIR,
	else if(Check(Cmd,"MEM",sizeof("MEM")))
	{
		memset(Cmd,0x00,10);
		if(Com)
		{
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			MemDatos = atoi((char*)Cmd);
			MemAddr = M_DTS_SEM_B00 + (MemDatos * 512);
			Wifi.EscDato = true;
			Wifi.IniDato = false;
			return;
			
		}
		else
		{
			memset(Cmd,0x00,10);
			S += Movstr(Cmd,S);
			S++;
			MemDatos = atoi((char*)Cmd);
			MemAddr = M_DTS_SEM_B00 + (MemDatos * 512);
			Wifi.LeeDato = true;
			Com_DtsTask_ROT010 = 0;
			Checksum = 0;
			return;
					
		}	
	}
//-----------------------------------------------------------------------------
//CARGA EL NUMERO DE SERIE
	else if(Check(Cmd,"SN",sizeof("SN")))
	{
		memset(Cmd,0x00,10);
		if(Com)
		{
			S += Movstr(Cmd,S);
			S++;
			SN1 = atol((char*)Cmd);
			S += Movstr(Cmd,S);
			S++;
			SN2= atol((char*)Cmd);
			if(SN1 != SN2)
			{
				P = TXTError(P);
				goto lFinComando;
			}
			memset(Nombre,0x00,20);
			strcpy(Nombre,"ECU5000-");
			strcat(Nombre,Cmd);
			EepromWRBuf(M_NOMBRE_SEMB,Nombre,20);
			EepromWRBuf(M_INFD_SN_0,(unsigned char *)&SN1,sizeof(SN1));
		}
		EepromRDBuf(M_INFD_SN_0,(unsigned char *)&SN1,sizeof(SN1));	
		P  = (unsigned char*) ultos10d(SN1,P);
		P++;	
	}
//-----------------------------------------------------------------------------
//CARGA EL NUMERO DE SERIE
	else if(Check(Cmd,"NOM",sizeof("NOM")))
	{
		
		if(Com)
		{
			memset(Nombre,0x00,20);
			for(i=0;i<20;i++)
			{
				if(*S == '<')
					break;
				Nombre[i]= *S;
				S++;
			}
			EepromWRBuf(M_NOMBRE_SEMB,Nombre,20);
		}
		memset(Nombre,0x00,20);
		Dly_10_MiliSec(10);
		EepromRDBuf(M_NOMBRE_SEMB,Nombre,20);
		for(i=0;i<20;i++)
		{
			if(!Nombre[i])
				break;
			*P = Nombre[i];
			P++;
		}
		P++;	
	}
//-----------------------------------------------------------------------------
//LECTURA DE LOS DATOS DE ID (IDENTIFICACION DE LA ECU)
	else if(Check(Cmd,"ID",sizeof("ID")))
	{
		strcpy((char *)P,kIdentificacion);
		P = P + strlen(kIdentificacion);
		EepromRDBuf(M_INFD_SN_0,(unsigned char *)&SN1,sizeof(SN1));	
		P  = (unsigned char*) ultos10d(SN1,P);
		P++;	
	}
//-----------------------------------------------------------------------------
//LECTURA DE LOS DATOS DE DERECHOS DE AUTOR
	else if(Check(Cmd,"CO",sizeof("CO")))
	{
		strcpy((char *)P,kCopyRight);
		P = P + strlen(kCopyRight);
	}
	else
	{
		P = TXTError(P);
	}
lFinComando:	
	P--;
	FinCmd(P);
	
//poner aca el llamado a la funcion para transmitir los datos
	CBuffersTx();
}

/******************************************************************************
*	Funcion:		Movstr()
*	Descricpion:	Copia datos desde el array de origen al de destino hasta    
*					encuentra el signo "<" o "," o el valor del array es cero
*	Ingreso Datos:	Puntero Array Destino
*					Puntero Arras Origen
*	Salida Datos:	La cantidad de caracters grabados
******************************************************************************/	 
unsigned char Movstr(unsigned char *Dest, unsigned char *Orig)
{	
	unsigned char i = 0;
	while(*Orig!=','&& *Orig!='<' && *Orig)
	{
		*Dest = *Orig;
		Dest++;
		Orig++;
		i++;
	}
	*Dest = 0x00;
	return i;
}

/******************************************************************************
*	Funcion:		Check()
*	Descricpion:	Compara si dos arrays son iguales
*	Ingreso Datos:	Puntero Array Destino
*					Puntero Arras Origen
*					Cantidad de caracteres a comparar
*	Salida Datos:	true si son iguales
*					false si son distintos
******************************************************************************/	 
char Check(unsigned char *Msg,char *Enc, int A)
{
	int i;
	A--;
	for(i=0;i<A;i++)
	{
		if(*Msg == *Enc)
		{
			Msg++;
			Enc++;
		}
		else
			return 0;
	}
	return 1;
}


/******************************************************************************
*	Funcion:		ModSiembraTx()
*	Descricpion:	Escribe en el array el modo de siembra en que esta 
*					configurado la sembradora
*	Ingreso Datos:	Puntero Array Destino
*
*	Salida Datos:	La cantidad de caracters grabados
******************************************************************************/	 
unsigned char* ModSiembraTx(unsigned char *S)
{
	if(DtsSiembra.ModSie == 1)
	{
		strcpy((char *)S,"SGCF");
		S=S + strlen("SGCF");
	}
	else if(DtsSiembra.ModSie == 2)
	{
		strcpy((char *)S,"SFCF");
		S=S + strlen("SFCF");
	}
	else if(DtsSiembra.ModSie == 3)
	{
		strcpy((char *)S,"SGSF");
		S=S + strlen("SGSF");
	}
	else if(DtsSiembra.ModSie == 4)
	{
		strcpy((char *)S,"SFCF");
		S=S + strlen("SFCF");
	}
	return (unsigned char*)S;
}


/******************************************************************************
*	Funcion:		ModSiembra()
*	Descricpion:	Lee el tipo de configuracion que tiene que tener la 
*					la sembradora, según el tipo guarda el parametro
*	Ingreso Datos:	Puntero Array Origen
*
*	Salida Datos:	La cantidad de caracters grabados
******************************************************************************/
unsigned int ModSiembra(unsigned char *Dest)
{
	if(Check(Dest,"SGCF",sizeof("SGCF")))
		return 1;
	else if(Check(Dest,"SFCF",sizeof("SFCF")))
		return 2;
	else if(Check(Dest,"SGSF",sizeof("SGSF")))
		return 3;
	else if(Check(Dest,"SFSF",sizeof("SFSF")))
		return 4;	
	else
		return 0;
}


/******************************************************************************
*	Funcion:		StsPer64()
*	Descricpion:	Carga en el buffer de el numero del sensor que esta activo
*					(en ASCCII)
*	Ingreso Datos:	Etiqueta de inicio
*					Puntero al buffer donde se cargan los parametros
*					Registros de sensores del 1 al 32  (Bus1)
*					Registros de sensores del 33 al 64 (Bus2)
*	Salida Datos:	Ninguno
******************************************************************************/	
void StsPer64 (char *lb, unsigned char *S,unsigned long Sen1,unsigned long Sen2)
{
	unsigned long Mask;
	int i;
	int cont;
	memset(QTxBuf,0x00,256); //strlen()
	*S = '>';
	S++;
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	cont = 1;
	for(i=0;i<32;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		
		if(Sen1 & Mask)
		{
			S  = (unsigned char*)uitos(cont,S);	//
			*S = ',';
			S++;
		}
		cont++;
	}	
	for(i=0;i<32;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		if(Sen2 & Mask)
		{
			S  = (unsigned char*)uitos(cont,S);	//
			*S = ',';
			S++;
		}
		cont++;
	}
	S--;
	*S = '<';
	S +=2;
	CRNL(S);
}

/******************************************************************************
*	Funcion:		StsPer16()
*	Descricpion:	Carga en el buffer de el numero del sensor que esta activo
*					(en ASCCII)
*	Ingreso Datos:	Etiqueta de inicio
*					Puntero al buffer donde se cargan los parametros
*					Registros de sensores del 1 al 16
*	Salida Datos:	Ninguno
******************************************************************************/			
void StsPer16 (char *lb, unsigned char *S,int Sen1)
{
	unsigned long Mask;
	int i;
	int cont;
	
	*S = '>';
	S++;
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	cont = 1;
	for(i=0;i<16;i++)
	{
		Mask = 1;
		Mask = Mask << i;
		if(Sen1 & Mask)
		{
			S  = (unsigned char*)uitos(cont,S);	//
			*S = ',';
			S++;
			
		}
		cont++;
	}	
	S--;
	*S = '<';
	S +=2;
	CRNL(S);
}


/******************************************************************************
*	Funcion:		TXTError()
*	Descricpion:	Envia un mensaje de error si hay un comando erroneo
*					
*	Ingreso Datos:	Puntero al buffer donde se cargan los parametros
*					
*	Salida Datos:	Direccion siguente del puntero del array
******************************************************************************/	
unsigned char * TXTError(unsigned char *S)
{
	strcpy((char*)S,"ERROR");
	S=S + strlen("ERROR");
	S++;
	return S;	
}

/******************************************************************************
*	Funcion:		EstadosCN()
*	Descricpion:	Envia un mensaje de error si hay un comando erroneo
*					
*	Ingreso Datos:	Puntero al buffer donde se cargan los parametros
*					
*	Salida Datos:	Direccion siguente del puntero del array
******************************************************************************/	
unsigned char * EstadosCN(char *lb,unsigned char *S)
{
	strcpy((char*)S,lb);
	S=S + strlen(lb);
	*S = ',';
	S++;
	return S;	
}
/******************************************************************************
*	Funcion:		FinCmd()
*	Descricpion:	Genera el fin del comando
*					
*	Ingreso Datos:	Puntero al buffer donde se cargan los parametros
*					
*	Salida Datos:	Direccion siguente del puntero del array
******************************************************************************/		
unsigned char * FinCmd(unsigned char *S)
{
	*S = '<';
	S ++;
	*S = 0x0A;
	S++;
	*S = 0x0D;
	S ++;
	*S = 0;
	return S;
}

/******************************************************************************
*	Funcion:		CBuffersTx()
*	Descricpion:	Espera a que se termine la transmicion de datos y carga los
*					datos en los buffer de transmicion de los puertos RS-232 y 
*					el puerto RS-232 que comunica con el modulo Wifi
*	Ingreso Datos:	No se ingresan datos
*					
*	Salida Datos:	No hay retorno
******************************************************************************/		
void CBuffersTx(void)
{
	while(!Proceso.B.fT2xCOM || !Proceso.B.fT3xCOM) 
	{
		
		ExeTask(); //Espero que termine la transmicion
		Nop();
		Nop();
	}
	
	memset(U2TxBuf,0,sizeof(U2TxBuf));
	memset(U3TxBuf,0,sizeof(U3TxBuf));
	
	memcpy(U2TxBuf,QTxBuf,256);
	memcpy(U3TxBuf,QTxBuf,256);
	U2TxInd=0;
	U3TxInd=0;
	U2TXREG = U2TxBuf[0];
	U3TXREG = U3TxBuf[0];
	memset(QTxBuf,0,sizeof(QTxBuf));
	
}
/******************************************************************************
*	Funcion:		BauToStr()
*	Descricpion:	Devuelve el valor en baudios segun como esta configurado el 
*					puerto que se quiere conocer la velocidad de transmicion.
*					
*	Ingreso Datos:	Puntero al buffer donde se cargan los parametros
*					Valor indice de baudios 
*	Salida Datos:	Direccion siguente del puntero del array
******************************************************************************/	
unsigned char * BauToStr(unsigned char * S, char Baud)
{
	switch(Baud)	
	{
		case 1:
			strcpy((char *)S,"4800");
			S=S + strlen("4800");
			break;
		case 2:
			strcpy((char *)S,"9600");
			S=S + strlen("9600");
			break;
		case 3:
			strcpy((char *)S,"19200");
			S=S + strlen("19200");
			break;
		case 4:	
			strcpy((char *)S,"38400");
			S=S + strlen("38400");					
			break;
		case 5:	
			strcpy((char *)S,"57600");
			S=S + strlen("57600");					
			break;
		case 6:	
			strcpy((char *)S,"115200");
			S=S + strlen("115200");					
			break;
		default:
			strcpy((char *)S,"9600");
			S=S + strlen("9600");
			break;
	}
	return S;
}	