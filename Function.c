
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
//��ʼ��SFR====================================================================
#ifdef ad834	
void init()
{
	PLLCON &=~0x07;							// 12.582912MHz
 	PLLCON |= 0x08;							// Set Fast Interrupt Resporse
	CFG834 |= 0x01;							// ʹ��XRAM����xdataָ���ڲ�xRAM������ָ���ⲿxRAM

	PWMCON=0x03;							//P1.0Singleģʽ��12.583M
	PWM1L=0x08;
	PWM1H=0x00;
	PWM0L=0x04;
	PWM0H=0x00;


 	PCON|=0x10;								// ��ֹALE���

//	TMOD=0x21;								// T1Ϊ�������ʣ�T0Ϊ16λ��ʱ��
	TCON=0x05;								// ���ж�Ϊ�ش���
	 /* I2C ����ʽ	*/
 	I2CCON=0xa8;
}
#endif

#ifdef ad832
void init()
{
	PLLCON&=0xF8;							//16.777216MHz
	CFG832|=0x01;							//CFG832^6=0ΪP2.6,P2.7;CFG832^6=1ΪP3.4,P3.3;CFG832^0=1ʹ��XRAM
//	CFG832|=0x41;							//CFG832^6=1ΪP3.2,P3.3;CFG832^6=1ΪP3.4,P3.3;CFG832^0=1ʹ��XRAM

//	EADRH=0x00;								//FLASH/EE���ֽڵ�ַΪ0

	PWMCON=0x83;							//����P3.3��P2.7,000ͣ,PWM������ʱ�ӷ�Ƶ��Ϊ1,PWMʱ��Դ16.78M
	PWM1L=0x0B;
	PWM0L=0x05;
	PWM1H=0x00;
	PWM0H=0x00;

//ʹ���ⲿ�ο���ѹ�������ݵ���
// 	ADCCON1=0x6C;							// ^8=0�ϵ�,�ⲿREF,16.78/16,4ADCclk,������T2��P3.5����PWM

// 	ADCCON1=0x1C;							// ^8=0�ϵ�,�ڲ�REF,16.78/4,4ADCclk,������T2��P3.5����PWM
// 	ADCCON1=0x0C;							// ^8=0�ϵ�,�ڲ�REF,16.78/8,4ADCclk,������T2��P3.5����PWM
	ADCCON1=0x2C;								// ^8=0�ϵ�,�ڲ�REF,16.78/16,4ADCclk,������T2��P3.5����PWM
	ADCCON2&=0x8F;							// ͣDMA,ͣ����ת��,ͣ����ת��
	ADCCON3&=0xFE;							// ������У׼
	ADCCON1 |= 0x80;							// ADC�ϵ�

//	PCON|=0x80;								// ˫�����ڲ�����
	PCON|=0x10;								// ��ֹALE���

//	TMOD=0x21;								// T1Ϊ�������ʣ�T0Ϊ16λ��ʱ��
	TCON=0x05;								// ���ж�Ϊ�ش���

//	P1=0xFC;								// 1��0��Ϊ������,��Ϊģ����
	P1=0x2D;								// 1,4,6,7��Ϊ������,��Ϊģ����
	 /* I2C ����ʽ	*/
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
//TIC����
int fnTICOn(void)
{
	init_tic();								// ��ʼ�����������ʱ��
	statusL &= ~0x40;
	TaskCreate(ModeNull);
	return ModeTicOn;
}
// TIC�ر�
int fnTICOff(void)
{
	dis_tic();
	statusL |= 0x40;
	TaskCreate(ModeNull);
	return ModeTicOff;
}
// �ɼ�����
int fnCollection(void)
{
//	WatchDogFeet();
	adkz();								// ��������,ѹ��,��ص�ѹ,�¶�(������KA=0)
//	TaskCreate(ModeComm);
	TaskCreate(ModeNull);
	return ModeCj;
}
// ���۶���
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
		statusH |= 0x04;	// �忪�۱�־
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
// �ձ۶���
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
		statusH |= 0x08;	// �忪�۱�־
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
// �������
int fnMotoP(void)
{
	statusL &= ~0x20; //��λ����״̬
	STOP(x)
	TURN_P(x)
	TaskCreate(ModeNull);
	return ModeZt;
}
// �������
int fnMotoN(void)
{
	statusL &= ~0x10; //��λ����״̬
	STOP(x)
	TURN_N(x)
	TaskCreate(ModeNull);
	return ModeFt;
}
// ���ֹͣ
int fnMotoST(void)
{
	STOP(x)
	statusL |= 0x20; 	// ������״̬
	statusL |= 0x10; 	// �巴��״̬
	statusH |= 0x04;	// �忪�۱�־
	statusH |= 0x08;	// ���ձ۱�־
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
	tempAD = ad(c_adMC);					// �������
	tempAD = (int)((float)tempAD / 4096.0 * 250 + 0.05);

//	tt = tt > tempAD ? tt : tempAD;
	if(tempAD > 110)							// �������������110mAֹͣ����
	{
		if(temp++ > 100)
		{
			statusH &= ~0x01;						//��Ϊ���������־ 
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
//	tempAD = ad(c_adWD) - 0x0A; 						// AD590�¶�,�¶�ƫ��10��ADֵ����
	tempAD = ad(c_adWD); 					// AD590�¶�
//	tempAD = 123;
//	tempAD |= 0xF000;
	inputParam[cTempH] = tempAD >> 8;
	inputParam[cTempL] = tempAD;					// �洢�¶ȵ�ѹֵ(12λ)
	return ModeTemp;
}
int fnColPress(void)
{
	unsigned int idata tempAD = 0;
// //	WatchDogFeet();
	tempAD = ad(c_adYL);					// ѹ��
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
	tempAD = ad(c_adDL);					// ��ͷ��ѹ
	tempAD = (int)((float)tempAD / 4096.0 * 2.5 / 33 * 2000 + 1.3);
//	inputParam[cCableH] = tempAD >> 8;
	inputParam[cCableH] = tempAD;
	return ModeCable;
}