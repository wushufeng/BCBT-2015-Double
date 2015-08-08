
#include "Head.h"

//void ad_Moto(void);
//unsigned int Moto_A = 0;
//extern void delay(void);
//char idata adflag;

//===================================================================
#ifdef ad834
unsigned int ad(unsigned char channel)
{
	// ѹ��ͨ��
	if(channel == 0)							
	{
		ADCMODE  = 0x22;						// ��ADC�ĵ��βɼ�ģʽ��
//		ADC0CON	 = 0x44;				// 0x43	��ADC���ã�ѡ���ⲿ��׼������ΪAIN1��AIN2ͨ����˫����320mV�ź�
 		ADC0CON	 = 0x43;				// 0x43	��ADC���ã�ѡ���ⲿ��׼������ΪAIN1��AIN2ͨ����˫����160mV�ź�
		while(!RDY0) WatchDogFeet();
		advalue  = ADC0H;						// ֻȡ��16λ����8λ����
		advalue<<=8;
		advalue += ADC0M;
		if(advalue > 0x8000)
			advalue -= 0x8000;
		else
			advalue = 0;
		advalue >>= 3;								//����AD����ֻȡ12λ
	}
	// �¶�ͨ����
	else if(channel == 1)					//�¶ȼ��㹫ʽ��AD*640/4096-273.15	
	{
		ADCMODE  = 0x22;						// ��ADC�ĵ��βɼ�ģʽ��
// 		ADC0CON	 = 0x5F;						// ��ADC���ã�ѡ���ⲿ��׼������ΪAIN3��AIN4ͨ����������0~2.5V�ź�
 		ADC0CON	 = 0x5D;						// ��ADC���ã�ѡ���ⲿ��׼������ΪAIN3��AIN4ͨ����������0~640mV�ź�
		while(!RDY0) WatchDogFeet();
		advalue  = ADC0H;						// ֻȡ��16λ����8λ����
		advalue<<=8;
		advalue |= ADC0M;
		advalue >>= 4;								//����AD����ֻȡ12λ
	}
	// ��ͷ��ѹ
	else if(channel == 2)						
	{
		ADCMODE  = 0x12;						// ��ADC�ĵ��βɼ�ģʽ��
		ADC1CON	 = 0x78;						// ��ADC���ã�ѡ���ⲿ��׼������ΪAIN5��AGNDͨ����������
		while(!RDY1) WatchDogFeet();
		advalue  = ADC1H;
		advalue<<=8;
		advalue |= ADC1L;
		advalue >>= 4;								//����AD����ֻȡ12λ
	}
	// Ƭ���¶�ͨ���������0x8000����0�棬��λÿ����1�棬��ֵ����1����λÿλ����1/256�档
	else if(channel == 3)						
	{
		ADCMODE  = 0x12;						// ������ADC�ĵ��βɼ�ģʽ��
		ADC1CON	 = 0x20;						// ��ADC���ã�ѡ���ڲ���׼������ΪTempSensor��AGNDͨ����˫����
		while(!RDY1) WatchDogFeet();
		advalue  = ADC1H;//��λ�����¶���������
		advalue<<=8;
		advalue |= ADC1L;//��λ�����¶�С�����֣�����ʽΪADC1L/256��ֵΪС������
// 		if(ADC1L > 127)
// 			advalue += 1;
// 		advalue = (advalue + 273) * 1.64; 	
		advalue >>= 4;
	}
	return(advalue);
}
#endif
#ifdef ad832
unsigned int adDc(unsigned char channel)//����AD�ɼ�
{
 	ADCCON2 = channel;								// ������AD
	while(ADCCON3 & 0x80)
	{
		WatchDogFeet();											// ��BUSY�ȴ�ADת�����
	}
	advalue = (ADDATA & 0x0FFF);
	return(advalue);
}
//===================================================================
// ADƽ��ֵ����(1/16)
unsigned int ad(unsigned char adchannel)
{
	unsigned char adj,yiwei;
	unsigned int  adsum = 0;
	unsigned int  adshu = 0;
	float tempdata = 0;
// 	ADCCON1 |= 0x80;							// ADC�ϵ�
	for (adj = 0,adsum = 0;adj < 16;adj++)				// ȡ16��ƽ��ֵ
	{
		for (yiwei = 0;yiwei < 10;yiwei++)				// ��ʱ
			nop;

		adsum += adDc(adchannel);
	}
//	if(adchannel == c_adWD)
//	{
//		tempdata = adsum;
//		tempdata = tempdata * 2.5 / 0.64; 
//		adsum = (int)tempdata;
//	}
	adshu = adsum >> 4;
// 	ADCCON1 &= ~0x80;							// ADC�ϵ�
	return(adshu);
}
#endif
// //��1B===============================================================
void ReadAD()
{
 	idata unsigned char j,bitt;
	MDE = 0;MCO = 0;//xsda=1; ����SDATA pinΪ����
// 	nop;nop;nop;nop;nop;nop;
// 	nop;nop;nop;nop;nop;nop;
	if(CNV)
	{
		CNV = 0;
		for (j = 0;j < 16;j ++)							//AD7980���һ�β��Ժ����ݱ���ȫ������
		{
			advalue = advalue << 1;
			MCO = 1;//xscl=1;
			bitt = MDI;//bitt=xsda;
			if (bitt) advalue = advalue | 0x0001;
			MCO = 0;//xscl=0;
		}
//		advalue = advalue >> 1;
	}
	CNV = 0;	
}

//void ad_Moto(void)
//{
//	unsigned int temp;
//	WatchDogFeet();
//	temp = ad(c_adDL);					// ��ͷ��ѹ
//	temp = (int)((float)temp / 4096.0 * 2.5 / 33 * 20000 + 7);
//	inputParam[cCableH] = temp >> 8;
//	inputParam[cCableL] = temp;

//	temp = ad(c_adMC);					// �������
//	temp = (int)((float)temp / 4096.0 * 2500);
//	inputParam[cMotoAcH] = temp >> 8;
//	inputParam[cMotoAcL] = temp;	

//}
// ADת��������======================================================
unsigned char adkz()
{
	unsigned char	del;
//  	unsigned char 	amd;
	unsigned int	ever;
	unsigned int	maxss,minss;
	unsigned int	zanshi,maxdn,mindn;
	unsigned long	totss;
	unsigned long	totdn;
	static unsigned long tiaoshi = 1;
//	static unsigned int tiaoshi2 = 1;
//	��̽ͷ����λ����-----------------------------------------------
	JL=0;
	JX=0;
	clear=0;
//	SEL_P_D(x)//����ѡ������̽ͷ
	SELSX = tiaoshi % 2;
	if(SELSX)							// ���ƽ�Ϊ0ʱ��ѡ����һ��̽ͷ����������̽ͷ��־λ��1
		statusL |= 0x40;
	else
		statusL &= ~0x40;
//	SEL_UP(x)
//	statusL |= 0x40;
	advalue = 3;								// ��֤�޽���ʱ��С��λ
	shengsu = 0xFFFF;							// ��ʾԼΪ1.5ms
//	REN = 0;
//	ES = 0;

	for (totss = 0,totdn = 0,maxdn = 0,mindn = 60000,maxss = 0,minss = 60000,ever = 0;ever < maxloop;ever ++)
	{
//		REN = 0;
//		ES = 0;
//		uartIqrFlag = 0x0F;
		EX0 = 0;								//��ֹ���ղ��ж�
#ifdef ad834
		JL = 1;//aduc_834
		PWMCON = 0x13;						//����1.3981MHz���PWM��P1.0��
#else
		JL = 0;//aduc_832
		PWMCON=0x93;						//����1.3981MHz���PWM��P2.7��		
#endif

		TF2 = 0;								//�嶨ʱ��2�жϱ�־
		T2DATA = TIMEVA;						//����T2����ֵ����ж�
		TR2 = 1;								//������ʱ��2
		for(del = PWTIME;del > 0;del --)
		{
			;
		}
#ifdef ad834
		PWMCON = 0x03;					//ֹͣPWM���X000XXXX
#endif
#ifdef ad832
		PWMCON = PWMCON & 0x8F;					//ֹͣPWM���X000XXXX
#endif
		JL = 0;
// 		JD = 1;
		for(del = PWDEL;del > 0;del --)				//�ȴ�˥��
		{
			;
		}

		JX = 1;
		clear = 1;
		
		IE0 = 0;								//���ⲿ�жϱ�־
		EX0 = 1;								//������ղ��ж�

//		RI = 0;
//		TI = 0;
//		REN = 1;
//		ES = 1;
		while(!TF2);						//�ȴ����ղ��жϲ����غ�T2���
//		if(uartIqrFlag == 0xF0)
//		{
//			tiaoshi2 ++;
//			inputParam[cSonicAH] = tiaoshi2 >> 8;				// �洢����
//			inputParam[cSonicBL] = tiaoshi2;				// �洢����
//			return 1;
//		}		
		ReadAD();
		TR2 = 0;
		EX0 = 0;								//�ؽ��ղ��ж�
		JX = 0;
		clear = 0;
		totss += (shengsu - TIMEVA);//bak	//�������ۼӺ�
		if(maxss < shengsu-TIMEVA) maxss = shengsu - TIMEVA;
		if(minss > shengsu-TIMEVA) minss = shengsu - TIMEVA;

		totdn += advalue;						//��DN��λ
		maxdn = maxdn > advalue ? maxdn : advalue;	// ����λ���ֵ
		mindn = mindn < advalue ? mindn : advalue;	// ����λ��Сֵ
	}
	ucDatLong = 0;
//	RI = 0;
//	TI = 0;
//	REN = 1;
//	ES = 1;	
	WatchDogFeet();
//ÿ�β�����ϲ���ģ���Դ
//	KA=0;									// ��ģ���Դ
// 	ADCCON1 &= 0x7F;							// ADC�ϵ�

	totdn = totdn - maxdn - mindn;	// ȥ�����ֵ����Сֵ���Լ������ʹ���С

	zanshi = totdn / (maxloop - 2);
	
//	if(tiaoshi % 2)
	if((statusL & 0x40) == 0x40)	// ��̽ͷ
	{
//		zanshi = (int)((float)zanshi / 6553.6 * 3600);
		inputParam[cFlowAH] = zanshi>>8;			// �洢������λ��
		inputParam[cFlowAL] = zanshi;
	}
	else	// ��̽ͷ
	{
		inputParam[cFlowBH] = zanshi>>8;			// �洢������λ��
		inputParam[cFlowBL] = zanshi;
		
	}
// 	zanshi=totss/maxloop;
	zanshi = (totss - maxss - minss) / (maxloop - 2) - 10;
	if(zanshi > 255)						// �������ֵ���ܳ���255
		zanshi = 255;
//	if(tiaoshi % 2)
	if((statusL & 0x40) == 0x40)	// ��̽ͷ
	{
//		inputParam[cSonicAH] = zanshi>>8;			// �洢����
		inputParam[cSonicAH] = zanshi;				// �洢����
	}
	else	// ��̽ͷ
	{
//		inputParam[cSonicBH] = zanshi>>8;			// �洢����
		inputParam[cSonicBL] = zanshi;				// �洢����
	}
	tiaoshi ++;
/*
//	��̽ͷ����λ������ʼ-----------------------------------------------
	JL=0;
	JX=0;
	clear=0;
	SEL_DW(x)//ѡ����̽ͷ
	status &= ~0x40;
	advalue=3;								// ��֤�޽���ʱ��С��λ
	shengsu=0xFFFF;							// ��ʾԼΪ1.5ms

	for (totss=0,totdn=0,maxdn=0,mindn=4000,maxss=0,minss=65535,
		ever=0;ever<maxloop;ever++)
	{
		REN = 0;
		ES = 0;
		EX0 = 0;								//��ֹ���ղ��ж�
		
#ifdef ad834
		JL = 1;//aduc_834
#else
		JL = 0;//aduc_832
#endif		
		PWMCON = 0x13;						//����1.3981MHz���PWM��P1.0��

		TF2=0;								//�嶨ʱ��2�жϱ�־
		T2DATA = TIMEVA;						//����T2����ֵ����ж�
		TR2=1;								//������ʱ��2
		for(del=PWTIME;del>0;del--)//������140mm���
		{
			;
		}
#ifdef ad834
		PWMCON = 0x03;					//ֹͣPWM���X000XXXX
#endif
#ifdef ad832
		PWMCON=PWMCON&0x8F;					//ֹͣPWM���X000XXXX
#endif
		JL = 0;
// 		JD = 1;
		for(del=PWDEL;del>0;del--)				//�ȴ�˥��
		{
			;
		}

		JX=1;
		clear=1;

		IE0 = 0;								//���ⲿ�жϱ�־
		EX0 = 1;								//������ղ��ж�
		RI = 0;
		TI = 0;
		REN = 1;
		ES = 1;
		while(!TF2);						//�ȴ����ղ��жϲ����غ�T2���
		ReadAD();
		TR2 = 0;
		EX0 = 0;								//�ؽ��ղ��ж�
		JX = 0;
		clear = 0;
		
		totss += (shengsu - TIMEVA);//bak	//�������ۼӺ�
		if(maxss < shengsu-TIMEVA) maxss=shengsu-TIMEVA;
		if(minss > shengsu-TIMEVA) minss=shengsu-TIMEVA;

		totdn+=advalue;						//��DN��λ
		maxdn = maxdn > advalue ? maxdn : advalue;	// ����λ���ֵ
		mindn = mindn < advalue ? mindn : advalue;	// ����λ��Сֵ

	}
	WatchDogFeet();
	totdn = totdn - maxdn - mindn;	// ȥ�����ֵ����Сֵ���Լ������ʹ���С
	zanshi = totdn / (maxloop - 2);
zanshi = tiaoshi2 ++;	

	inputParam[cFlowBH] = (zanshi >> 8);			// �洢������λ��
	inputParam[cFlowBL] = (zanshi & 0xff);
// 	zanshi=totss/maxloop;
	zanshi = (totss - maxss - minss) / (maxloop - 2) - 10;
//	if(zanshi > 255)						// �������ֵ���ܳ���255
//		zanshi = 255;	
	inputParam[cSonicBH] = zanshi >> 8;			// �洢������λ�� 
	inputParam[cSonicBL] = zanshi;				// �洢������λ��
	SEL_UP(x)//ѡ����̽ͷ
//	��̽ͷ����λ�������-----------------------------------------------
*/
//	zanshi = ad(c_adWD) - 0x0A; 						// AD590�¶�,�¶�ƫ��10��ADֵ����
//	zanshi = ad(c_adWD); 					// AD590�¶�
//	inputParam[cTempH] = zanshi >> 8;
//	inputParam[cTempL] = zanshi;					// �洢�¶ȵ�ѹֵ(12λ)

//	zanshi = ad(c_adYL);					// ѹ��
//	inputParam[cPressH] = zanshi >> 8;
//	inputParam[cPressL] = zanshi;			

//	zanshi = ad(c_adDL);					// ��ͷ��ѹ
//	zanshi = (int)((float)zanshi / 4096.0 * 2.5 / 33 * 20000 + 7);
//	inputParam[cCableH] = zanshi >> 8;
//	inputParam[cCableL] = zanshi;

//	zanshi = ad(c_adMC);					// �������
//	zanshi = (int)((float)zanshi / 4096.0 * 2500);
//	inputParam[cMotoAcH] = zanshi >> 8;
//	inputParam[cMotoAcL] = zanshi;	
// //tiaoshi
// 	inputParam[c_xiangweih]=0x23;			// �洢������λ��
// 	inputParam[c_xiangweil]=0x90;
// 	inputParam[c_shengsuh]=0x00;			// �洢������λ�� 
// 	inputParam[c_shengsul]=0x8d;				// �洢������λ��
// //tiaoshi

// //tiaoshi
// 	inputParam[c_xwh]=0x1e;			// �洢������λ��
// 	inputParam[c_xwl]=0x90;
// 	inputParam[c_ssh]=0x00;			// �洢������λ�� 
// 	inputParam[c_ssl]=0x8d;				// �洢������λ��
// //tiaoshi
// 	inputParam[0]=sizeof(inputParam);
// //tiaoshi	
	return 1;
}
