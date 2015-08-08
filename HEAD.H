// #define ad834
#define ad832
#ifdef ad834
#include <ADUC834.H>
#endif
#ifdef ad832
#include <ADUC832.H>
#endif
#include <intrins.h>
#include <stdio.h>					//Prototypes（原型） for standard I/O functions.				
#include <string.h>	
// #define	uchar	unsigned char
// #define	uint	unsigned int
// #define ulong	unsigned long

#define	nop		_nop_()

//=====================================
// sfr16	ADC0DATA	=	0xDA;
// sfr16	ADC1DATA	=	0xDC;
sfr16	ADDATA	=	0xD9;
sfr16	T2DATA	=	0xCC;
sfr16	T1DATA	=	0xCC;
sfr16	EADR	=	0xC6;

sbit P0_0=P0^0;		sbit P1_0=P1^0;		sbit P2_0=P2^0;		sbit P3_0=P3^0;
sbit P0_1=P0^1;		sbit P1_1=P1^1;		sbit P2_1=P2^1;		sbit P3_1=P3^1;
sbit P0_2=P0^2;		sbit P1_2=P1^2;		sbit P2_2=P2^2;		sbit P3_2=P3^2;
sbit P0_3=P0^3;		sbit P1_3=P1^3;		sbit P2_3=P2^3;		sbit P3_3=P3^3;
sbit P0_4=P0^4;		sbit P1_4=P1^4;		sbit P2_4=P2^4;		sbit P3_4=P3^4;
sbit P0_5=P0^5;		sbit P1_5=P1^5;		sbit P2_5=P2^5;		sbit P3_5=P3^5;
sbit P0_6=P0^6;		sbit P1_6=P1^6;		sbit P2_6=P2^6;		sbit P3_6=P3^6;
sbit P0_7=P0^7;		sbit P1_7=P1^7;		sbit P2_7=P2^7;		sbit P3_7=P3^7;

//-------------------------------------
//--------片内存储器功能定义-----------
//-------------------------------------
#define	READ		0x01
#define	WRITE		0x02
#define	VERIFY		0x04
#define	ERASE_PAGE	0x05
#define	ERASE_ALL	0x06
#define	READ_BYTE	0x81
#define	WRITE_BYTE	0x82
#define	EXULOAD		0x0F
#define	ULOAD		0xF0
//-------------------------------------
//-----------数据存储结构体------------
//-------------------------------------
struct WR_DATA
{
	unsigned char lenth;
	unsigned char wrdata[14];
	unsigned char check;
};
extern xdata struct WR_DATA SVersion;
extern xdata struct WR_DATA DTyep;
extern xdata struct WR_DATA DNum;
extern xdata struct WR_DATA DtVer;
//-------------------------------------
//--------霍尔及仪器状态定义-----------
//-------------------------------------
extern unsigned char bdata statusL;  
extern unsigned char idata statusH;  
/* 输入,用于程序中的位判断 */
extern bit fFK;				//放开
extern bit fYQ;				//到位
extern bit fZT;				//正调
extern bit fFT;				//反调
extern bit fSL;				//收拢
extern bit fCJ;				//采集
extern bit fTX;				//上下探头选择，1为上，0为下
extern bit fJD;				//角度	
//-------------------------------------
//------------ADuc834定义--------------
//-------------------------------------
#ifdef ad834
#define SYSCLK 12583

#define JL		P1_0       					// 超声波激励

#define CNV		P2_0         				// 启动7980
#define MC2 	P2_1         				// 电机控制口2
#define MC1 	P2_2         				// 电机控制口1
#define SL		P2_3         				// 调节臂收拢否
#define FK		P2_4         				// 调节臂放开否
#define YQ		P2_5	         			// 仪器到位否
#define JD		P2_6						// 堵塞器角度
#define KA		P2_7						// 模拟电源控制 

#define JX		P3_3         				// 控制7474鉴相
#define clear	P3_4						// 接收波分频控制
#define SELSX	P3_5						// 上下探头选择控制

#define ReadZhibi(x)	{P2 |= 0x78;fFK = FK; fSL = SL;fYQ = YQ;fJD = JD;}	//读支臂当前状态

//AD834 AD通道选择
#define c_adYL	0						// 压力测量
#define c_adWD	1						// 片内温度监测
#define c_adDL	2						// 缆头电压
#define c_adSN	3					// 片内温度监测

//ADuc834探头140mm间距
#define PWTIME   	26   						// 发送脉冲延时
#define PWDEL			10    					// 发送与接收之间延迟时间；延时等待接收波
#define PWDELW   	16  						// 开始滤波与采样之间延迟 //35
#define PWPLUS   	4    						// 两次发送脉冲序列延迟时间

#define TIMEVA		0XF9DA					// ADuc834//2mS=F7CE,1.5ms=F9DA,1.2ms=FB16,1ms=FBE6

#endif
//-------------------------------------
//------------ADuc832定义--------------
//-------------------------------------
#ifdef ad832
#define SYSCLK 16777

#define KM		P2_5								// 电机总开关
//#define MC2 	P0_7         				// 电机控制口2
//#define MC1 	P0_6         				// 电机控制口1
#define MC2 	P3_5          			// 电机控制口2
#define MC1 	P3_6          			// 电机控制口1
#define SL		P2_4         				// 调节臂收拢否
#define FK		P2_3         				// 调节臂放开否
#define YQ		P2_2	         			// 仪器到位否
#define JD		P2_1								// 堵塞器角度
#define SELSX	P2_6								// 上下探头选择控制
#define JL		P2_7       					// 超声波激励

#define clear	P3_3								// 接收波分频控制
#define KA		P3_5								// 模拟电源控制 
#define JX		P3_7         				// 控制7474鉴相
#define CNV		P2_0         				// 启动7980

#define ReadZhibi(x)	{P2 |= 0x1E;fFK = FK; fSL = SL;fYQ = YQ;fJD = JD;}	//读支臂当前状态

// AD832 AD通道选择
#define c_adDL	0x10						// 缆头电压
#define c_adMC	0x11						// 电机电流
#define c_adYL	0x12						// 压力测量
#define c_adWD	0x13						// 温度转换电压
#define c_adSN	0x18						// 片内温度监测
#define c_adAG	0x1B						// AGD测量
#define c_adRF	0x1C						// 参考电压测量

//ADuc832探头140mm间距
//#define PWTIME   	35   					// 发送脉冲延时
//#define PWDEL			16    				// 发送与接收之间延迟时间；延时等待接收波
//#define PWDELW   	22  					// 开始滤波与采样之间延迟 //35
//#define PWPLUS   	6    					// 两次发送脉冲序列延迟时间
//ADuc832探头400mm间距
// #define PWTIME   	100   					// 发送脉冲延时
// #define PWDEL			44    					// 发送与接收之间延迟时间；延时等待接收波
// #define PWDELW   	60  						// 开始滤波与采样之间延迟 //35
// #define PWPLUS   	50    					// 两次发送脉冲序列延迟时间
//ADuc832探头单测220mm间距
//#define PWTIME   	40   					// 发送脉冲延时
//#define PWDEL			35    				// 发送与接收之间延迟时间；延时等待接收波
//#define PWDELW   	20  					// 开始滤波与采样之间延迟 //35
//#define PWPLUS   	25    				// 两次发送脉冲序列延迟时间
#define PWTIME   	55   					// 发送脉冲延时
#define PWDEL			24    				// 发送与接收之间延迟时间；延时等待接收波
#define PWDELW   	33  					// 开始滤波与采样之间延迟 //35
#define PWPLUS   	20//28    				// 两次发送脉冲序列延迟时间

#define TIMEVA		0xFA8A 				// ADuc832//2mS=F514,1.5ms=f7cf,1.2ms=F972,1ms=FA8A

#endif
//-------------------------------------
//----------仪器控制命令定义-----------
//-------------------------------------
#define TURN_P(x)	{KM = 0;MC1 = 1; MC2 = 0; }	//正转
#define TURN_N(x)	{KM = 0;MC1 = 0; MC2 = 1; }	//反转
#define STOP(x)		{KM = 1;MC1 = 0; MC2 = 0; }	//停止
#define SEL_UP(x)	{SELSX = 1;}	//SELECT UP
#define SEL_DW(x)	{SELSX = 0;}	//SELECT DOWM
#define SEL_P_D(x)	{SELSX = ~SELSX;}	//SELECT UP DOWM 交替

//-------------------------------------
//--------数据存储空间定义-------------
//-------------------------------------
#define		c_soft		0x0000						// 软件版本，共16字节
#define 	c_yiqi		0x0010						// 仪器类型，共16字节
#define 	c_numb		0x0020						// 仪器序列号，共16字节
#define 	c_stric		0x0030						// 数据结构，共16字节
#define 	c_jingh		0x0040						// 井号，共16字节
#define 	c_caiy		0x0050						// 采集间隔，共16字节
#define 	c_ssxw		0x0060						// 标定声速/零点相位，共16字节
#define 	c_zjzl		0x0070						// 空，共16字节
#define 	c_bank0		0x0080						// 空，共16字节
#define 	c_bank1		0x0090						// 空，共16字节
#define 	c_bank2		0x00A0						// 空，共16字节
#define 	c_bank3		0x00B0						// 空，共16字节
#define 	c_bdrq		0x00C0						// 标定日期，共16字节

#define 	c_ylbd		0x00D0						// 压力标定数据，共560字节

#define 	c_llsd		0x0300						// 流量标定数据上吊，共64字节
#define 	c_llsz		0x0340						// 流量标定数据上吊，共64字节
#define 	c_llxd		0x0380						// 流量标定数据上吊，共64字节
#define 	c_llxz		0x03c0						// 流量标定数据上吊，共64字节
//-------------------------------------
//--------数据帧通讯格式定义-----------
//-------------------------------------
//#define	c_status	1						// 仪器当前状态
//#define c_xiangweih	2						// 相位差1存储帧地址高位
//#define c_xiangweil	3						// 相位差1存储帧地址低位
//#define c_shengsuh	4						// 声速1存储帧地址高位
//#define c_shengsul	5						// 声速1存储帧地址低位
//#define c_yali		6						// 压力高位存储帧地址
//#define c_ylwd		7						// 压力低位,温度高位存储帧地址
//#define c_wendu		8						// 温度低位存储帧地址
//#define c_xwh		9						// 相位差2存储帧地址高位
//#define c_xwl		10						// 相位差2存储帧地址低位
//#define c_ssh		11						// 声速2存储帧地址高位
//#define c_ssl		12						// 声速2存储帧地址高位
//#define c_ltdyh		13						//缆头电压高位
//#define c_ltdyl		14						//缆头电压高位

//#define	cStatusH		1						// 仪器当前状态高位
//#define	cStatusL		2						// 仪器当前状态地位
//#define cMotoAcH 		3						// 电机电流高位
//#define cMotoAcL		4						// 电机电流地位
//#define cPressH			5						// 压力高位
//#define cPressL			6						// 压力地位
//#define cTempH			7						// 温度高位
//#define cTempL			8						// 温度低位
//#define cFlowAH			9						// 流量1高位
//#define cFlowAL			10					// 流量1低位
//#define cSonicAH		11					// 声速1高位
//#define cSonicAL		12					// 声速1低位
//#define cFlowBH			13					// 流量2高位
//#define cFlowBL			14					// 流量2低位
//#define cSonicBH		15					// 声速2高位
//#define cSonicBL		16					// 声速2高位
typedef enum
{
	cStatusH = 0,										// 仪器当前状态高位
	cStatusL,												// 仪器当前状态低位
	cCableH,												// 缆头电压高位
//	cCableL,												// 缆头电压低位
//	cMotoAcH,												// 电机电流高位
	cMotoAcL,												// 电机电流低位
	cPressH,												// 压力高位
	cPressL,												// 压力低位
	cTempH,													// 温度高位
	cTempL,													// 温度低位
	cFlowAH,												// 流量1高位
	cFlowAL,												// 流量1低位
	cFlowBH,												// 流量2高位
	cFlowBL,												// 流量2低位
	cSonicAH,												// 声速1高位
//	cSonicAL,												// 声速1低位
//	cSonicBH,												// 声速2高位
	cSonicBL,												// 声速2低位			
}eInputReg;
//-------------------------------------
//------------采集相关定义-------------
//-------------------------------------
#define dlong	16        					// 数据长度16=15(data)+1(Sum)
#define maxloop  	262//260  				// 每次采样数据总数
#define No       	2
#define Yes      	1
//-------------------------------------
//------------工作模式定义-------------
//-------------------------------------
//#define ModeSCj		0xF0
//#define ModeCj		0xF1
//#define ModeTx		0xF2
//#define ModeYq		0xF3
//#define ModeKb		0xF4
//#define ModeSl		0xF5
//#define ModeFt		0xF6
//#define ModeZt		0xF7
//#define ModeST		0xF8
typedef enum
{
	ModeNull = 0,
	ModeTicOn,
	ModeTicOff,
	ModeComm,
	ModeCj,
	ModeKb,
	ModeSl,
	ModeZt,
	ModeFt,
	ModeST,
	ModeMTc,
	ModeCable,
	ModeTemp,
	ModePress
}eWorkMode;

//-------------------------------------
//------------全局变量说明-------------
//-------------------------------------
extern	unsigned char	xdata	inputParam[20];					// 测量数据存储数组
extern	unsigned char	xdata	array[20];				// 测量数据存储数组

extern	unsigned int	idata	shijian;					// 看门狗溢出次数(n*2.3S)
extern	unsigned int	idata	shengsu;					// 每次声速值
extern	unsigned int	idata	advalue;					// 每次AD结果
extern 	unsigned int 	idata zbFlag;

extern unsigned char	xdata	DatPocket[30];		//串口接收数据空间
extern unsigned char	data	ucDatLong;
//extern unsigned char	idata	Jiaodu;						// 每一个角度旋转60度角
//extern unsigned char	idata	Tnow;							//新来的任务
extern unsigned char	idata	Tdo;							//正在做的任务
extern eWorkMode			idata	Task[5];					//任务序列先进先出
extern eWorkMode		  idata uartCreateTask;				//串口中断标志
//extern unsigned char idata uartIqrFlag;				//串口创建任务标志
//extern unsigned char	idata	Clting;				//采集进行中标识
//extern unsigned char	idata	Kbing;				//开臂进行中标识extern 
//extern unsigned char	idata	Sbing;				//收臂进行中标识extern 
//extern unsigned int				Moto_A;
//-------------------------------------
//-------------函数定义-------==-------
//-------------------------------------
extern void Init_T1(void);
extern void Timer1_ReSet(void);
extern unsigned int	adDc(unsigned char);
extern unsigned int	ad(unsigned char);
extern void ReadAD();
extern void RTU_read_comm(void);
extern void Sendcomchar(unsigned char buff);
extern void EnterUART(void);
//extern void ad_Moto(void);
extern void SendComStr(unsigned char *string,unsigned int num);
extern void delay_us(unsigned int ff);
extern unsigned char ReadByte(unsigned int Addr);
extern bit WriteByte16(unsigned int Addr,unsigned char *ptr);
extern bit WritePage(unsigned int Addr,unsigned char *ptr);
extern void TaskCreate(eWorkMode ucTask);//0:tdo;1:new
extern void init_tic(void);
void dis_tic(void);
extern	unsigned char	adkz(void);
// MAIN.C
void WatchDogClose(void);
void WatchDogFeet(void);
extern	void	init(void);
extern	void	Init_T3();
extern void InitDevice();
// Function.c
extern int fnNULL(void);
extern int fnTICOn(void);
extern int fnTICOff(void);
extern int fnCollection(void);
extern int fnKB(void);
extern int fnSL(void);
extern int fnMotoP(void);
extern int fnMotoN(void);	
extern int fnMotoST(void);
extern int fnComm(void);
extern int fnMotoTC(void);
extern int fnColTemp(void);
extern int fnColPress(void);
extern int fnColCable(void);
	