
#include "Head.h"

//void ad_Moto(void);
//unsigned int Moto_A = 0;
//extern void delay(void);
//char idata adflag;

//===================================================================
#ifdef ad834
unsigned int ad(unsigned char channel)
{
	// 压力通道
	if(channel == 0)							
	{
		ADCMODE  = 0x22;						// 主ADC的单次采集模式。
//		ADC0CON	 = 0x44;				// 0x43	主ADC设置，选择外部基准，设置为AIN1和AIN2通道，双极性320mV信号
 		ADC0CON	 = 0x43;				// 0x43	主ADC设置，选择外部基准，设置为AIN1和AIN2通道，双极性160mV信号
		while(!RDY0) WatchDogFeet();
		advalue  = ADC0H;						// 只取高16位，低8位丢掉
		advalue<<=8;
		advalue += ADC0M;
		if(advalue > 0x8000)
			advalue -= 0x8000;
		else
			advalue = 0;
		advalue >>= 3;								//所有AD数据只取12位
	}
	// 温度通道，
	else if(channel == 1)					//温度计算公式：AD*640/4096-273.15	
	{
		ADCMODE  = 0x22;						// 主ADC的单次采集模式。
// 		ADC0CON	 = 0x5F;						// 主ADC设置，选择外部基准，设置为AIN3和AIN4通道，单极性0~2.5V信号
 		ADC0CON	 = 0x5D;						// 主ADC设置，选择外部基准，设置为AIN3和AIN4通道，单极性0~640mV信号
		while(!RDY0) WatchDogFeet();
		advalue  = ADC0H;						// 只取高16位，低8位丢掉
		advalue<<=8;
		advalue |= ADC0M;
		advalue >>= 4;								//所有AD数据只取12位
	}
	// 缆头电压
	else if(channel == 2)						
	{
		ADCMODE  = 0x12;						// 从ADC的单次采集模式。
		ADC1CON	 = 0x78;						// 从ADC设置，选择外部基准，设置为AIN5和AGND通道，单极性
		while(!RDY1) WatchDogFeet();
		advalue  = ADC1H;
		advalue<<=8;
		advalue |= ADC1L;
		advalue >>= 4;								//所有AD数据只取12位
	}
	// 片上温度通道，结果：0x8000代表0℃，高位每增加1℃，数值增加1，低位每位代表1/256℃。
	else if(channel == 3)						
	{
		ADCMODE  = 0x12;						// 开启从ADC的单次采集模式。
		ADC1CON	 = 0x20;						// 从ADC设置，选择内部基准，设置为TempSensor和AGND通道，双极性
		while(!RDY1) WatchDogFeet();
		advalue  = ADC1H;//高位代表温度整数部分
		advalue<<=8;
		advalue |= ADC1L;//地位代表温度小数部分，其形式为ADC1L/256的值为小数部分
// 		if(ADC1L > 127)
// 			advalue += 1;
// 		advalue = (advalue + 273) * 1.64; 	
		advalue >>= 4;
	}
	return(advalue);
}
#endif
#ifdef ad832
unsigned int adDc(unsigned char channel)//单次AD采集
{
 	ADCCON2 = channel;								// 启动单AD
	while(ADCCON3 & 0x80)
	{
		WatchDogFeet();											// 判BUSY等待AD转换完成
	}
	advalue = (ADDATA & 0x0FFF);
	return(advalue);
}
//===================================================================
// AD平均值计算(1/16)
unsigned int ad(unsigned char adchannel)
{
	unsigned char adj,yiwei;
	unsigned int  adsum = 0;
	unsigned int  adshu = 0;
	float tempdata = 0;
// 	ADCCON1 |= 0x80;							// ADC上电
	for (adj = 0,adsum = 0;adj < 16;adj++)				// 取16次平均值
	{
		for (yiwei = 0;yiwei < 10;yiwei++)				// 延时
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
// 	ADCCON1 &= ~0x80;							// ADC断电
	return(adshu);
}
#endif
// //读1B===============================================================
void ReadAD()
{
 	idata unsigned char j,bitt;
	MDE = 0;MCO = 0;//xsda=1; 设置SDATA pin为输入
// 	nop;nop;nop;nop;nop;nop;
// 	nop;nop;nop;nop;nop;nop;
	if(CNV)
	{
		CNV = 0;
		for (j = 0;j < 16;j ++)							//AD7980完成一次测试后数据必须全部读出
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
//	temp = ad(c_adDL);					// 缆头电压
//	temp = (int)((float)temp / 4096.0 * 2.5 / 33 * 20000 + 7);
//	inputParam[cCableH] = temp >> 8;
//	inputParam[cCableL] = temp;

//	temp = ad(c_adMC);					// 电机电流
//	temp = (int)((float)temp / 4096.0 * 2500);
//	inputParam[cMotoAcH] = temp >> 8;
//	inputParam[cMotoAcL] = temp;	

//}
// AD转换主程序======================================================
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
//	上探头组相位测量-----------------------------------------------
	JL=0;
	JX=0;
	clear=0;
//	SEL_P_D(x)//交替选择上下探头
	SELSX = tiaoshi % 2;
	if(SELSX)							// 控制较为0时候选择上一组探头，并将上下探头标志位置1
		statusL |= 0x40;
	else
		statusL &= ~0x40;
//	SEL_UP(x)
//	statusL |= 0x40;
	advalue = 3;								// 保证无接收时显小相位
	shengsu = 0xFFFF;							// 显示约为1.5ms
//	REN = 0;
//	ES = 0;

	for (totss = 0,totdn = 0,maxdn = 0,mindn = 60000,maxss = 0,minss = 60000,ever = 0;ever < maxloop;ever ++)
	{
//		REN = 0;
//		ES = 0;
//		uartIqrFlag = 0x0F;
		EX0 = 0;								//禁止接收波中断
#ifdef ad834
		JL = 1;//aduc_834
		PWMCON = 0x13;						//启动1.3981MHz输出PWM在P1.0单
#else
		JL = 0;//aduc_832
		PWMCON=0x93;						//启动1.3981MHz输出PWM在P2.7单		
#endif

		TF2 = 0;								//清定时器2中断标志
		T2DATA = TIMEVA;						//声速T2赋初值或等中断
		TR2 = 1;								//启动定时器2
		for(del = PWTIME;del > 0;del --)
		{
			;
		}
#ifdef ad834
		PWMCON = 0x03;					//停止PWM输出X000XXXX
#endif
#ifdef ad832
		PWMCON = PWMCON & 0x8F;					//停止PWM输出X000XXXX
#endif
		JL = 0;
// 		JD = 1;
		for(del = PWDEL;del > 0;del --)				//等待衰减
		{
			;
		}

		JX = 1;
		clear = 1;
		
		IE0 = 0;								//清外部中断标志
		EX0 = 1;								//允许接收波中断

//		RI = 0;
//		TI = 0;
//		REN = 1;
//		ES = 1;
		while(!TF2);						//等待接收波中断并返回和T2溢出
//		if(uartIqrFlag == 0xF0)
//		{
//			tiaoshi2 ++;
//			inputParam[cSonicAH] = tiaoshi2 >> 8;				// 存储声速
//			inputParam[cSonicBL] = tiaoshi2;				// 存储声速
//			return 1;
//		}		
		ReadAD();
		TR2 = 0;
		EX0 = 0;								//关接收波中断
		JX = 0;
		clear = 0;
		totss += (shengsu - TIMEVA);//bak	//求声速累加和
		if(maxss < shengsu-TIMEVA) maxss = shengsu - TIMEVA;
		if(minss > shengsu-TIMEVA) minss = shengsu - TIMEVA;

		totdn += advalue;						//求DN相位
		maxdn = maxdn > advalue ? maxdn : advalue;	// 求相位最大值
		mindn = mindn < advalue ? mindn : advalue;	// 求相位最小值
	}
	ucDatLong = 0;
//	RI = 0;
//	TI = 0;
//	REN = 1;
//	ES = 1;	
	WatchDogFeet();
//每次测试完毕不关模拟电源
//	KA=0;									// 关模拟电源
// 	ADCCON1 &= 0x7F;							// ADC断电

	totdn = totdn - maxdn - mindn;	// 去掉最大值和最小值，以及次最大和次最小

	zanshi = totdn / (maxloop - 2);
	
//	if(tiaoshi % 2)
	if((statusL & 0x40) == 0x40)	// 上探头
	{
//		zanshi = (int)((float)zanshi / 6553.6 * 3600);
		inputParam[cFlowAH] = zanshi>>8;			// 存储流量相位差
		inputParam[cFlowAL] = zanshi;
	}
	else	// 下探头
	{
		inputParam[cFlowBH] = zanshi>>8;			// 存储流量相位差
		inputParam[cFlowBL] = zanshi;
		
	}
// 	zanshi=totss/maxloop;
	zanshi = (totss - maxss - minss) / (maxloop - 2) - 10;
	if(zanshi > 255)						// 声速最大值不能超过255
		zanshi = 255;
//	if(tiaoshi % 2)
	if((statusL & 0x40) == 0x40)	// 上探头
	{
//		inputParam[cSonicAH] = zanshi>>8;			// 存储声速
		inputParam[cSonicAH] = zanshi;				// 存储声速
	}
	else	// 下探头
	{
//		inputParam[cSonicBH] = zanshi>>8;			// 存储声速
		inputParam[cSonicBL] = zanshi;				// 存储声速
	}
	tiaoshi ++;
/*
//	下探头组相位测量开始-----------------------------------------------
	JL=0;
	JX=0;
	clear=0;
	SEL_DW(x)//选择下探头
	status &= ~0x40;
	advalue=3;								// 保证无接收时显小相位
	shengsu=0xFFFF;							// 显示约为1.5ms

	for (totss=0,totdn=0,maxdn=0,mindn=4000,maxss=0,minss=65535,
		ever=0;ever<maxloop;ever++)
	{
		REN = 0;
		ES = 0;
		EX0 = 0;								//禁止接收波中断
		
#ifdef ad834
		JL = 1;//aduc_834
#else
		JL = 0;//aduc_832
#endif		
		PWMCON = 0x13;						//启动1.3981MHz输出PWM在P1.0单

		TF2=0;								//清定时器2中断标志
		T2DATA = TIMEVA;						//声速T2赋初值或等中断
		TR2=1;								//启动定时器2
		for(del=PWTIME;del>0;del--)//短仪器140mm间距
		{
			;
		}
#ifdef ad834
		PWMCON = 0x03;					//停止PWM输出X000XXXX
#endif
#ifdef ad832
		PWMCON=PWMCON&0x8F;					//停止PWM输出X000XXXX
#endif
		JL = 0;
// 		JD = 1;
		for(del=PWDEL;del>0;del--)				//等待衰减
		{
			;
		}

		JX=1;
		clear=1;

		IE0 = 0;								//清外部中断标志
		EX0 = 1;								//允许接收波中断
		RI = 0;
		TI = 0;
		REN = 1;
		ES = 1;
		while(!TF2);						//等待接收波中断并返回和T2溢出
		ReadAD();
		TR2 = 0;
		EX0 = 0;								//关接收波中断
		JX = 0;
		clear = 0;
		
		totss += (shengsu - TIMEVA);//bak	//求声速累加和
		if(maxss < shengsu-TIMEVA) maxss=shengsu-TIMEVA;
		if(minss > shengsu-TIMEVA) minss=shengsu-TIMEVA;

		totdn+=advalue;						//求DN相位
		maxdn = maxdn > advalue ? maxdn : advalue;	// 求相位最大值
		mindn = mindn < advalue ? mindn : advalue;	// 求相位最小值

	}
	WatchDogFeet();
	totdn = totdn - maxdn - mindn;	// 去掉最大值和最小值，以及次最大和次最小
	zanshi = totdn / (maxloop - 2);
zanshi = tiaoshi2 ++;	

	inputParam[cFlowBH] = (zanshi >> 8);			// 存储流量相位差
	inputParam[cFlowBL] = (zanshi & 0xff);
// 	zanshi=totss/maxloop;
	zanshi = (totss - maxss - minss) / (maxloop - 2) - 10;
//	if(zanshi > 255)						// 声速最大值不能超过255
//		zanshi = 255;	
	inputParam[cSonicBH] = zanshi >> 8;			// 存储声速相位差 
	inputParam[cSonicBL] = zanshi;				// 存储声速相位差
	SEL_UP(x)//选择上探头
//	下探头组相位测量完毕-----------------------------------------------
*/
//	zanshi = ad(c_adWD) - 0x0A; 						// AD590温度,温度偏高10个AD值左右
//	zanshi = ad(c_adWD); 					// AD590温度
//	inputParam[cTempH] = zanshi >> 8;
//	inputParam[cTempL] = zanshi;					// 存储温度电压值(12位)

//	zanshi = ad(c_adYL);					// 压力
//	inputParam[cPressH] = zanshi >> 8;
//	inputParam[cPressL] = zanshi;			

//	zanshi = ad(c_adDL);					// 缆头电压
//	zanshi = (int)((float)zanshi / 4096.0 * 2.5 / 33 * 20000 + 7);
//	inputParam[cCableH] = zanshi >> 8;
//	inputParam[cCableL] = zanshi;

//	zanshi = ad(c_adMC);					// 电机电流
//	zanshi = (int)((float)zanshi / 4096.0 * 2500);
//	inputParam[cMotoAcH] = zanshi >> 8;
//	inputParam[cMotoAcL] = zanshi;	
// //tiaoshi
// 	inputParam[c_xiangweih]=0x23;			// 存储流量相位差
// 	inputParam[c_xiangweil]=0x90;
// 	inputParam[c_shengsuh]=0x00;			// 存储声速相位差 
// 	inputParam[c_shengsul]=0x8d;				// 存储声速相位差
// //tiaoshi

// //tiaoshi
// 	inputParam[c_xwh]=0x1e;			// 存储流量相位差
// 	inputParam[c_xwl]=0x90;
// 	inputParam[c_ssh]=0x00;			// 存储声速相位差 
// 	inputParam[c_ssl]=0x8d;				// 存储声速相位差
// //tiaoshi
// 	inputParam[0]=sizeof(inputParam);
// //tiaoshi	
	return 1;
}
