
#include "Head.h"
//**************************************************************************************************************************************************************************
// ι��������1000ms��ʱ
//**************************************************************************************************************************************************************************
void WatchDogFeet(void)
{
//	if(EA)
//	{
//		EA = 0;
		WDCON |= 0x01;						// ���Ź��Ĵ���д����
		WDCON  = 0x72;						// ���ÿ��Ź���ʱ1000ms����������Ź���ʱ�����ر�д����
//		EA = 1;	
//	}
//	else
//	{
//		WDCON |= 0x01;						// ���Ź��Ĵ���д����
//		WDCON  = 0x72;						// ���ÿ��Ź���ʱ1000ms����������Ź���ʱ�����ر�д����
//	}
	
}
//**************************************************************************************************************************************************************************
// �ع�
//**************************************************************************************************************************************************************************
void WatchDogClose(void)
{
	WDCON |= 0x01;						// ���Ź��Ĵ���д����
	WDCON  = 0x70;						// ���ÿ��Ź���ʱ1000ms�����رտ��Ź�
}
//=====================================
// ʱ������ʱ��TIC��ʼ������
void init_tic(void)
{
 	shijian = 1;
	if(shijian<60)							// 1������
	{
		TIMECON = 0x12;						// 0-255Сʱ,��,����,�����ж�,
		INTVAL = shijian;						// ���ʱ��5,10,20,30s;1,2,5,10,20,30m
	}
	else if(shijian<3600)					// 1Сʱ����
	{
		TIMECON=0x22;						// 0-255Сʱ,��,����,�����ж�,
		INTVAL=shijian/60;					// ���ʱ��5,10,20,30s;1,2,5,10,20,30m
	}
	else									// 1��ʽ��65535/3600=18.2Сʱ
	{
		TIMECON=0x32;						// 0-255Сʱ,��,����,�����ж�,
		INTVAL=shijian/3600;				// ���ʱ��5,10,20,30s;1,2,5,10,20,30m
	}

	TIMECON=TIMECON|0x03;					// ʹ��ʱ��,����TIC
	IEIP2=IEIP2|0x04;						// ����TIC�ж� ETI��1 Written by User to Enable ��1�� or Disable ��0�� Time Interval Counter Interrupt.
}   
//=====================================
// ʱ������ʱ��TIC����
void dis_tic(void)
{
	TIMECON = TIMECON&0xFC;			// ֹͣTIC
	IEIP2 = IEIP2&0xFB;				// ��ֹTIC�ж�
}
//��ʱ��1��ʼ��=====================================
//����ͨѶ�ְ�
void Init_T1(void)							//�ϰ�ʱ��Ϊ3.5���ֳ�
{
	TMOD |= 0x10;								//��ʱ��1������16λģʽ��
}
//��ʱ��1��λ=====================================
void Timer1_ReSet(void)	  						//0(19200),1(38400),2(57600),3(115200)
{

	TH1 = 0xC9;//( char ) (( 65536 - (int)( SYSCLK / 12 * 10)) / 256 ) ;
	TL1 = 0x62;//( char ) (( 65536 - (int)( SYSCLK / 12 * 10)) % 256 ) ;
	ET1   = 1 ;		// Enable TIMER0 Interrupt.
	TR1   = 1;		// ��ʱ��0���п���λ
}
// ��ʱ��T3��ʼ������(��T3Ϊ���ڲ�����9600bps������)
void Init_T3(void)
{
//	SCON=0xC0;								// ����11b�ɱ�,
	SCON = 0x40;								// ����10b�ɱ�,2007

	T3CON = 0x85;
#ifdef ad834
	T3FD = 0x12;
#endif
#ifdef ad832
	T3FD = 0x2D;	
#endif
	RI = 0;
	TI = 0;
	REN = 1;
//	ES=1;
}
//��ʱ��2��ʼ��=====================================
//����ͨѶ�ְ�
//void Init_T2(void)							//�ϰ�ʱ��Ϊ3.5���ֳ�
//{
//	TMOD |= 0x10;								//��ʱ��0������16λģʽ��
//}
//��ʱ��0��λ=====================================
//void Timer2_ReSet(void)	  						//0(19200),1(38400),2(57600),3(115200)
//{
//	T2DATA = ( 65536 - (int)( SYSCLK / 12 * 4));
//	ET2   = 1 ;		// Enable TIMER0 Interrupt.
//	TR2   = 1;		// ��ʱ��0���п���λ
//}