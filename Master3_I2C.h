extern void InitI2C3(void);
extern int StartRdI2C3(unsigned int ADDR);
extern void BufferRdI2C3(unsigned char *pBuf,unsigned char Size);

extern int StartWrI2C3(unsigned int ADDR);
extern void BufferWrI2C3(unsigned char *pBuf,unsigned char Size);


extern void Master3_StartI2C(void);

extern void Master3_WriteI2C(unsigned char byte);
extern void Master3_IdleI2C(void);

