
#include "Head.h"
/* --------定义位变量 -----------*/
//unsigned char bdata status = 0xFF;  
///* 输入,用于程序中的位判断 */
//sbit  fFK = status^0;//放开
//sbit  fYQ = status^1;//到位
//sbit  fZT = status^2;//正调
//sbit  fFT = status^3;//反调
//sbit  fSL = status^4;//收拢
//sbit  fCJ = status^5;//采集
//sbit  fTX = status^6;//上下探头选择，1为上，0为下
//sbit  fJD = status^7;//角度						
unsigned char bdata statusL = 0xFF;  
unsigned char idata statusH = 0xFF;
/* 输入,用于程序中的位判断 */
sbit  fYQ = statusL^0;//到位
sbit  fFK = statusL^1;//放开
sbit  fSL = statusL^2;//收拢
sbit  fJD = statusL^3;//角度		

sbit  fZT = statusL^4;//正调
sbit  fFT = statusL^5;//反调
sbit  fCJ = statusL^6;//采集
sbit  fTX = statusL^7;//上下探头选择，1为上，0为下
//全局变量定义
unsigned char	xdata	inputParam[20];							// 测量数据存储数组
unsigned char	xdata	array[20];					// 测量数据存储数组

unsigned int	idata	shijian;						// 看门狗溢出次数(n*2.3S)
unsigned int	idata	shengsu;						// 每次声速值
unsigned int	idata	advalue;						// 每次AD结果
unsigned int  idata zbFlag = 0;
//unsigned char	idata	Jiaodu = 0;					// 每一个角度旋转60度角
eWorkMode 		idata	Task[5] = {ModeNull,ModeMTc,ModeCable,ModeTemp,ModePress};					//任务序列先进先出
//unsigned char	idata	Tnow = 0;						//新来的任务
unsigned char	idata	Tdo = 0;						//正在做的任务
eWorkMode		  idata uartCreateTask = ModeNull;
//unsigned char idata uartIqrFlag;	//串口中断标志

extern int (*pFunc[14])(void);
//Debug===================
//typedef struct 
//{
//	int a;
//	int b;
//	
//}sDDD;

//初始化后判断执行测量函数或通讯函数===========================================
void main()
{
 	eWorkMode ucFrame;
	unsigned char idata err;
	
	STOP(x)
 	SELSX = 1;
	EA = 0;									// 关闭总中断
	WatchDogClose();						// 关闭看门狗
	JL = 0;									// 清零激励脚
	CNV = 0;
	clear = 0;
	init();
	if(WDS == 0)							// 上电复位
		InitDevice();
	else									// 看门狗复位
		;
	Init_T1();
	Init_T3();
	ES = 1;
	EA = 1;
//	if(!WDS)
//		Sendcomchar(0x33);
	if(WDS == 0)	
	{		// 上电复位
		Sendcomchar(0x33);Sendcomchar(0x33);Sendcomchar(0x33);Sendcomchar(0x33);
	}
	else		
	{		// 看门狗复位
		Sendcomchar(0x44);Sendcomchar(0x44);Sendcomchar(0x44);Sendcomchar(0x44);
	}
	while(1)
	{
		ReadZhibi(x)	
		WatchDogFeet();
//		if(uartCreateTask == ModeCj)
//		{
//			ucFrame = ModeCj;
//			uartCreateTask = ModeNull;
//		}
//		else if(Task[0] == ModeNull)
		if(Task[0] == ModeNull)
			ucFrame = Task[Tdo];
		else
			ucFrame = Task[0];
		err = (*pFunc[ucFrame])();
		if(++ Tdo > 4)
			Tdo = 1;
//		if((!SL)&&(zbFlag == ModeSl))
//		{
//			STOP(x);
//			zbFlag = ModeNull;
//			TaskCreate(ModeST);
//		}
//		else if((!FK)&&(zbFlag == ModeKb))
//		{
//			STOP(x);
//			zbFlag = ModeNull;
//			TaskCreate(ModeST);
//		}
//		else if((SL)&&(zbFlag == ModeSl))
//		{
//			(*pFunc[ModeSl])();
//		}
//		else if((FK)&&(zbFlag == ModeKb))
//		{
//			(*pFunc[ModeKb])();
//		}
//		if(!err)
//			Sendcomchar(ucFrame);
	}
}

