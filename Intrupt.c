
//�ۼӺ�У����idata sum-----------
#include "Head.h"

//=====================================
void EXT0(void)
{
	unsigned char x;//�˴�X�����ͺ��й�ϵxdataҪ��idata��ʱʱ�䳤�ܶ�

	TR2 = 0;
	shengsu = T2DATA;							//����
	TR2 = 1;

	for(x = PWDELW;x > 0;x --);	//��ʱ��AD
//	ad(c_adXW);								//�ڲ�AD�ɼ�
	CNV = 1;								//�ⲿAD�ɼ�
	for(x = PWPLUS;x > 0;x --);
//	for(x = 16;x > 0;x --);

	JX = 0;
	clear = 0;
	EX0 = 0;								//���ⲿ�ж�
}		
void EXTI0_IQRHandler() interrupt 0 using 1
{
	EXT0();
}
  
//modbus�ְ�=========================
//void Timer0_IQRHandler() interrupt 1
//{
////	TR0=0;
////	nop;
////	nop;
////	TF0=0;
//	TF0 = 0;									//��������־λ
//	TR0 = 0;									//��ʱ��0���п���λ
//	ET0 = 0;									//��ֹT0�ж�
////	adflag = 1;
////	RTU_read_comm();
////	ucDatLong = 0;

////	NextTask(1);
////	Task[Tnow] = ModeCj;
////	NextTask(1);
////	Task[Tnow] = ModeComm;
//}

////�ز�����INT1ʱ=======================
//void EXTI1_IQRHandler() interrupt 2
//{
////	unsigned char x;
//	nop;
//	nop;

//}

//=====================================
//���ջز���ʱ=========================
void Timer1_IQRHandler() interrupt 3
{
//	TR1=0;
////	bitqqcxfs_kd=1;
//#ifdef ad834
//	PWMCON = 0x03;			//ͣPWM
//#endif
//#ifdef ad832
//	PWMCON = 0x83;			//ͣPWM
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
	}   //��������жϱ�־λ����TI��0��
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
	TIMECON = TIMECON & 0xFB;					//�����жϱ�־TII
//	TaskCreate(ModeCj);
} 
//=====================================
////��ʱ��2�жϷ�����
////��ʱ1.5ms
//void Timer2_IQRHandler() interrupt 5
//{
//	TR2=0;
//	TF2=0;
//	EXF2=0;
//	RTU_read_comm();
//	ucDatLong = 0;
//}

//=====================================

