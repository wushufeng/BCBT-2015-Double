
#include "Head.h"

//extern	unsigned char	xread_data(void);
//extern	void	wlong(void);
//extern	void	rlong(void);
int (*pFunc[14])(void) = {
	fnNULL,
	fnTICOn,
	fnTICOff,
	fnComm,
	fnCollection,
	fnKB,
	fnSL,
	fnMotoP,
	fnMotoN,	
	fnMotoST,
	fnMotoTC,	
	fnColCable,
	fnColTemp,
	fnColPress
//fnMotoTC,
};
//typedef enum
//{
//	ModeNull = 0,
//	ModeTicOn,
//	ModeTicOff,
//	ModeComm,
//	ModeCj,
//	ModeKb,
//	ModeSl,
//	ModeZt,
//	ModeFt,
//	ModeST,
//	ModeMTc,
//	ModeCable,
//	ModeTemp,
//	ModePress
//}eWorkMode;
//初始化SFR====================================================================
#ifdef ad834	
void init()
{
	PLLCON &=~0x07;							// 12.582912MHz
 	PLLCON |= 0x08;							// Set Fast Interrupt Resporse
	CFG834 |= 0x01;							// 使能XRAM，即xdata指到内部xRAM，否则指向外部xRAM

	PWMCON=0x03;							//P1.0Single模式，12.583M
	PWM1L=0x08;
	PWM1H=0x00;
	PWM0L=0x04;
	PWM0H=0x00;


 	PCON|=0x10;								// 禁止ALE输出

//	TMOD=0x21;								// T1为串波特率，T0为16位定时器
	TCON=0x05;								// 外中断为沿触发
	 /* I2C 主方式	*/
 	I2CCON=0xa8;
}
#endif

#ifdef ad832
void init()
{
	PLLCON&=0xF8;							//16.777216MHz
	CFG832|=0x01;							//CFG832^6=0为P2.6,P2.7;CFG832^6=1为P3.4,P3.3;CFG832^0=1使能XRAM
//	CFG832|=0x41;							//CFG832^6=1为P3.2,P3.3;CFG832^6=1为P3.4,P3.3;CFG832^0=1使能XRAM

//	EADRH=0x00;								//FLASH/EE高字节地址为0

	PWMCON=0x83;							//单在P3.3或P2.7,000停,PWM计数器时钟分频率为1,PWM时钟源16.78M
	PWM1L=0x0B;
	PWM0L=0x05;
	PWM1H=0x00;
	PWM0H=0x00;

//使用外部参考电压测试数据递增
// 	ADCCON1=0x6C;							// ^8=0断电,外部REF,16.78/16,4ADCclk,不允许T2或P3.5启动PWM

// 	ADCCON1=0x1C;							// ^8=0断电,内部REF,16.78/4,4ADCclk,不允许T2或P3.5启动PWM
// 	ADCCON1=0x0C;							// ^8=0断电,内部REF,16.78/8,4ADCclk,不允许T2或P3.5启动PWM
	ADCCON1=0x2C;								// ^8=0断电,内部REF,16.78/16,4ADCclk,不允许T2或P3.5启动PWM
	ADCCON2&=0x8F;							// 停DMA,停连续转换,停单次转换
	ADCCON3&=0xFE;							// 不启动校准
	ADCCON1 |= 0x80;							// ADC上电

//	PCON|=0x80;								// 双倍串口波特率
	PCON|=0x10;								// 禁止ALE输出

//	TMOD=0x21;								// T1为串波特率，T0为16位定时器
	TCON=0x05;								// 外中断为沿触发

//	P1=0xFC;								// 1、0脚为数字入,余为模拟入
	P1=0x2D;								// 1,4,6,7脚为数字入,余为模拟入
	 /* I2C 主方式	*/
 	I2CCON=0xa8;
}
#endif
void TaskCreate(eWorkMode ucTask)//0:tdo;1:new
{
	Task[0] = ucTask;
}
int fnNULL(void)
{
	TaskCreate(ModeNull);
	return ModeNull;
}
//TIC开启
int fnTICOn(void)
{
	init_tic();								// 初始化采样间隔定时器
	statusL &= ~0x40;
	TaskCreate(ModeNull);
	return ModeTicOn;
}
// TIC关闭
int fnTICOff(void)
{
	dis_tic();
	statusL |= 0x40;
	TaskCreate(ModeNull);
	return ModeTicOff;
}
// 采集流量
int fnCollection(void)
{
//	WatchDogFeet();
	adkz();								// 测量流量,压力,电池电压,温度(其内已KA=0)
//	TaskCreate(ModeComm);
	TaskCreate(ModeNull);
	return ModeCj;
}
// 开臂动作
int fnKB(void)
{
	unsigned char xdata n = 0;
	if(FK)
	{
//		afk:
		statusH &= ~0x04; 
		TURN_P(x)
			n = 0;
//		while(FK&&(Task[0] != ModeST))
		while(Task[0] != ModeST)
		{
			if(!FK)
				if(n++ > 6)
					break;
			(*pFunc[ModeMTc])();
			WatchDogFeet();
			(*pFunc[ModeCable])();
			ReadZhibi(x)
		}
		STOP(x)	
//		while(n ++ < 250)
//			WatchDogFeet();
//		if(FK&&(Task[0] != ModeST))
//		{
//			n = 0;
//			goto afk;
//		}
		statusH |= 0x04;	// 清开臂标志
	}
	ReadZhibi(x)
	if(Task[0] == ModeKb)
		TaskCreate(ModeNull);
	return ModeKb;
//	if(FK)
//	{
//		statusH &= ~0x04;
//		STOP(x)
//		TURN_N(x)
//		zbFlag = ModeKb;
//	}
//	TaskCreate(ModeNull);
//	return ModeKb;
}
// 收臂动作
int fnSL(void)
{
	unsigned int xdata n = 0;
	if(SL)
	{
//		asl:
		statusH &= ~0x08;
		TURN_N(x)
		while(Task[0] != ModeST)
		{
			if(!SL)
				if(n++ > 6)
					break;			
			(*pFunc[ModeMTc])();
			WatchDogFeet();
			(*pFunc[ModeCable])();
			ReadZhibi(x)
		}
		STOP(x)
//		while(n ++ < 250)
//			WatchDogFeet();
//		if(SL && (Task[0] != ModeST))
//		{
//			n = 0;
//			goto asl;
//		}
		statusH |= 0x08;	// 清开臂标志
	}
	ReadZhibi(x)
	if(Task[0] == ModeSl)
		TaskCreate(ModeNull);
	return ModeSl;
//	if(SL)
//	{
//		statusH &=  ~0x08;
//		STOP(x)
//		TURN_N(x)
//		zbFlag = ModeSl;
//	}
//	TaskCreate(ModeNull);
//	return ModeSl;
}
// 电机正调
int fnMotoP(void)
{
	statusL &= ~0x20; //置位正调状态
	STOP(x)
	TURN_P(x)
	TaskCreate(ModeNull);
	return ModeZt;
}
// 电机反调
int fnMotoN(void)
{
	statusL &= ~0x10; //置位反调状态
	STOP(x)
	TURN_N(x)
	TaskCreate(ModeNull);
	return ModeFt;
}
// 电机停止
int fnMotoST(void)
{
	STOP(x)
	statusL |= 0x20; 	// 清正调状态
	statusL |= 0x10; 	// 清反调状态
	statusH |= 0x04;	// 清开臂标志
	statusH |= 0x08;	// 清收臂标志
	zbFlag = ModeNull;
	STOP(x)
	if(Task[0] == ModeST)
		TaskCreate(ModeNull);
	return ModeST;
}
int fnComm(void)
{
//	RTU_read_comm();
//	ucDatLong = 0;
//	if(Task[0] == ModeComm)
//		TaskCreate(ModeNull);
	return ModeComm;
}
int fnMotoTC(void)
{
	unsigned int idata tempAD = 0;
//	static unsigned char idata tt = 0;
	static unsigned char idata temp = 0;
//	WatchDogFeet();
	tempAD = ad(c_adMC);					// 电机电流
	tempAD = (int)((float)tempAD / 4096.0 * 250 + 0.05);

//	tt = tt > tempAD ? tt : tempAD;
	if(tempAD > 110)							// 若电机电流大于110mA停止动作
	{
		if(temp++ > 100)
		{
			statusH &= ~0x01;						//置为电机过流标志 
//			STOP(x)
//			TaskCreate(ModeST);
			temp = 0;
		}
	}
	inputParam[cMotoAcL] = tempAD;
//	inputParam[cMotoAcL] = tt;
	
	return ModeMTc;
}
int fnColTemp(void)
{
	unsigned int idata tempAD = 0;
//	static unsigned int idata temp = 1;temp ++;
//	WatchDogFeet();
//	tempAD = ad(c_adWD) - 0x0A; 						// AD590温度,温度偏高10个AD值左右
	tempAD = ad(c_adWD); 					// AD590温度
//	tempAD = 123;
//	tempAD |= 0xF000;
	inputParam[cTempH] = tempAD >> 8;
	inputParam[cTempL] = tempAD;					// 存储温度电压值(12位)
	return ModeTemp;
}
int fnColPress(void)
{
	unsigned int idata tempAD = 0;
// //	WatchDogFeet();
	tempAD = ad(c_adYL);					// 压力
//	tempAD |= 0xF000;
	inputParam[cPressH] = tempAD >> 8;
	inputParam[cPressL] = tempAD;				
	return ModePress;
}
int fnColCable(void)
{
	unsigned int idata tempAD = 0;
//	static unsigned int idata temp = 1;temp ++;
//	WatchDogFeet();
	tempAD = ad(c_adDL);					// 缆头电压
	tempAD = (int)((float)tempAD / 4096.0 * 2.5 / 33 * 2000 + 1.3);
//	inputParam[cCableH] = tempAD >> 8;
	inputParam[cCableH] = tempAD;
	return ModeCable;
}