// extern int Comando(unsigned char *S);
 extern void FindelaCita(unsigned char* S);
 extern char Check(unsigned char *Msg,char *Enc, int i);
 extern unsigned char Movstr(unsigned char *Dest, unsigned char *Orig);
 extern unsigned int ModSiembra(unsigned char *Dest);
 extern unsigned char* ModSiembraTx(unsigned char *S);
 extern unsigned char * TXTError(unsigned char *S);
 extern void StsPer64 (char *lb, unsigned char *S,unsigned long Sen1,unsigned long Sen2);
 extern void StsPer16 (char *lb, unsigned char *S,int Sen1);
 extern unsigned char * FinCmd(unsigned char *S);
 extern unsigned char * BauToStr(unsigned char * S, char Baud);
 extern void CBuffersTx(void);
 extern unsigned char * EstadosCN(char *lb,unsigned char *S);

