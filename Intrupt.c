
//累加和校验用idata sum-----------
#include "Head.h"

//=====================================
void EXT0(void)
{
	unsigned char x;//此处X的类型很有关系xdata要比idata延时时间长很多

	TR2 = 0;
	shengsu = T2DATA;							//声速
	TR2 = 1;

	for(x = PWDELW;x > 0;x --);	//延时后AD
//	ad(c_adXW);								//内部AD采集
	CNV = 1;								//外部AD采集
	for(x = PWPLUS;x > 0;x --);
//	for(x = 16;x > 0;x --);

	JX = 0;
	clear = 0;
	EX0 = 0;								//关外部中断
}		
void EXTI0_IQRHandler() interrupt 0 using 1
{
	EXT0();
}
  
//modbus分包=========================
//void Timer0_IQRHandler() interrupt 1
//{
////	TR0=0;
////	nop;
////	nop;
////	TF0=0;
//	TF0 = 0;									//软件清除标志位
//	TR0 = 0;									//定时器0运行控制位
//	ET0 = 0;									//禁止T0中断
////	adflag = 1;
////	RTU_read_comm();
////	ucDatLong = 0;

////	NextTask(1);
////	Task[Tnow] = ModeCj;
////	NextTask(1);
////	Task[Tnow] = ModeComm;
//}

////回波接至INT1时=======================
//void EXTI1_IQRHandler() interrupt 2
//{
////	unsigned char x;
//	nop;
//	nop;

//}

//=====================================
//接收回波超时=========================
void Timer1_IQRHandler() interrupt 3
{
//	TR1=0;
////	bitqqcxfs_kd=1;
//#ifdef ad834
//	PWMCON = 0x03;			//停PWM
//#endif
//#ifdef ad832
//	PWMCON = 0x83;			//停PWM
//#endif
//	nop;
//	nop;

//	IE0=0;
//	TF1=0;
	TF1=0;
	TR1=0;
	ET1=0;
	
	RTU_read_comm();
	ucDatLong = 0;	
	
//	TaskCreate(ModeComm);
}

//=====================================
void EnterUART(void)
{
	if(TI)
	{
		TI = 0;
		return;
	}   //清楚发送中断标志位：将TI置0；
	RI = 0;
// 	adflag = 1;
//	if(ucDatLong > 21)		ucDatLong = 0;
	DatPocket[ucDatLong] = SBUF;
	ucDatLong ++;
//	Timer0_ReSet();	
	Timer1_ReSet();	
//	uartIqrFlag = 0xF0;
}void UART_IQRHandler() interrupt 4 //using 1
{
	EnterUART();
}
void TIC_IQRHandler() interrupt 10
{
	TIMECON = TIMECON & 0xFB;					//清零中断标志TII
//	TaskCreate(ModeCj);
} 
//=====================================
////定时器2中断服务函数
////定时1.5ms
//void Timer2_IQRHandler() interrupt 5
//{
//	TR2=0;
//	TF2=0;
//	EXF2=0;
//	RTU_read_comm();
//	ucDatLong = 0;
//}

//=====================================

