
#include "Head.h"
/* --------����λ���� -----------*/
//unsigned char bdata status = 0xFF;  
///* ����,���ڳ����е�λ�ж� */
//sbit  fFK = status^0;//�ſ�
//sbit  fYQ = status^1;//��λ
//sbit  fZT = status^2;//����
//sbit  fFT = status^3;//����
//sbit  fSL = status^4;//��£
//sbit  fCJ = status^5;//�ɼ�
//sbit  fTX = status^6;//����̽ͷѡ��1Ϊ�ϣ�0Ϊ��
//sbit  fJD = status^7;//�Ƕ�						
unsigned char bdata statusL = 0xFF;  
unsigned char idata statusH = 0xFF;
/* ����,���ڳ����е�λ�ж� */
sbit  fYQ = statusL^0;//��λ
sbit  fFK = statusL^1;//�ſ�
sbit  fSL = statusL^2;//��£
sbit  fJD = statusL^3;//�Ƕ�		

sbit  fZT = statusL^4;//����
sbit  fFT = statusL^5;//����
sbit  fCJ = statusL^6;//�ɼ�
sbit  fTX = statusL^7;//����̽ͷѡ��1Ϊ�ϣ�0Ϊ��
//ȫ�ֱ�������
unsigned char	xdata	inputParam[20];							// �������ݴ洢����
unsigned char	xdata	array[20];					// �������ݴ洢����

unsigned int	idata	shijian;						// ���Ź��������(n*2.3S)
unsigned int	idata	shengsu;						// ÿ������ֵ
unsigned int	idata	advalue;						// ÿ��AD���
unsigned int  idata zbFlag = 0;
//unsigned char	idata	Jiaodu = 0;					// ÿһ���Ƕ���ת60�Ƚ�
eWorkMode 		idata	Task[5] = {ModeNull,ModeMTc,ModeCable,ModeTemp,ModePress};					//���������Ƚ��ȳ�
//unsigned char	idata	Tnow = 0;						//����������
unsigned char	idata	Tdo = 0;						//������������
eWorkMode		  idata uartCreateTask = ModeNull;
//unsigned char idata uartIqrFlag;	//�����жϱ�־

extern int (*pFunc[14])(void);
//Debug===================
//typedef struct 
//{
//	int a;
//	int b;
//	
//}sDDD;

//��ʼ�����ж�ִ�в���������ͨѶ����===========================================
void main()
{
 	eWorkMode ucFrame;
	unsigned char idata err;
	
	STOP(x)
 	SELSX = 1;
	EA = 0;									// �ر����ж�
	WatchDogClose();						// �رտ��Ź�
	JL = 0;									// ���㼤����
	CNV = 0;
	clear = 0;
	init();
	if(WDS == 0)							// �ϵ縴λ
		InitDevice();
	else									// ���Ź���λ
		;
	Init_T1();
	Init_T3();
	ES = 1;
	EA = 1;
//	if(!WDS)
//		Sendcomchar(0x33);
	if(WDS == 0)	
	{		// �ϵ縴λ
		Sendcomchar(0x33);Sendcomchar(0x33);Sendcomchar(0x33);Sendcomchar(0x33);
	}
	else		
	{		// ���Ź���λ
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

