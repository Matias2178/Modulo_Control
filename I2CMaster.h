extern void Master3_InitI2C(void);
extern void Master3_StartI2C(void);
extern void Master3_RestartI2C(void);
extern void Master3_StopI2C(void);
extern void Master3_WriteI2C(unsigned char byte);
extern void Master3_IdleI2C(void);
extern unsigned int Master3_getI2C(void);
extern void Master3_NotAckI2C(void);
extern void Master3_AckI2C(void);

extern char StartRdM3_I2C(unsigned int ADDR);