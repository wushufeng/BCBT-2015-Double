#include "Head.h"
xdata struct WR_DATA SVersion;
xdata struct WR_DATA DTyep;
xdata struct WR_DATA DNum;
xdata struct WR_DATA DtVer;
void delay_us(unsigned int ff)
{
	for(;ff > 0;ff --)
	{
 		nop;nop;nop;nop;
		nop;nop;nop;nop;
		nop;nop;nop;nop;
		nop;nop;nop;nop;
		nop;nop;nop;nop;
	}
}
// #define	READ		0x01
// #define	WRITE		0x02
// #define	VERIFY		0x04
// #define	ERASE_PAGE	0x05
// #define	ERASE_ALL	0x06
// #define	READ_BYTE	0x81
// #define	WRITE_BYTE	0x82
// #define	EXULOAD		0x0F
// #define	ULOAD		0xF0
unsigned char ReadByte(unsigned int Addr)
{
	if(Addr < 0x1000)
	{
		EADR = Addr;
		ECON = READ_BYTE;	
		nop;nop;nop;
// 		dataa = EDATA1;
	}
	else
		return 0x4E;//地址超界返回特殊字节
	return EDATA1;
}
void ReadPage(unsigned int Addr,unsigned char *ptr)
{
	unsigned int tPage;

	tPage = Addr / 4;
	if(tPage < 0x0400)
	{
		EADR = tPage;
		ECON = READ;	
		nop;nop;nop;nop;nop;
		*ptr++  = EDATA1;
		*ptr++  = EDATA2;
		*ptr++  = EDATA3;
		*ptr	= EDATA4;
	}
// 	else
// 		return 0x4E;//地址超界返回特殊字节
}

// void ReadByte16(unsigned int Addr,unsigned char *ptr)
// {
// 	unsigned char xdata n;
// 	for(n = 0;n < 16;n ++)
// 		*ptr ++ = ReadByte(Addr + n);
// 		
// }
bit WriteByte16(unsigned int Addr,unsigned char *ptr)
{
	unsigned int tPage,n;
	tPage = Addr / 4;
	for(n = 0;n < 4;n ++)
	{
		EADR = tPage + n;
// 		ECON = READ_BYTE;	
// 		nop;nop;nop;	
		EDATA1 = *ptr ++;
		EDATA2 = *ptr ++;
		EDATA3 = *ptr ++;
		EDATA4 = *ptr ++;
		ECON = ERASE_PAGE;
		WatchDogFeet();
		delay_us(2000);
		ECON = WRITE;
		delay_us(380);
		ECON = VERIFY;
		nop;nop;nop;nop;nop;
		if(ECON)
		{
			nop;nop;nop;nop;nop;
			if(ECON)
				return 0;
		}
	}
	return 1;
}
bit WritePage(unsigned int Addr,unsigned char *ptr)
{
	unsigned int tPage;
	if(!(Addr % 4))
	{
		tPage = Addr / 4;
		EADR = tPage ;
	// 	ECON = READ_BYTE;	
	// 	nop;nop;nop;	
		EDATA1 = *ptr ++;
		EDATA2 = *ptr ++;
		EDATA3 = *ptr ++;
		EDATA4 = *ptr ++;
		ECON = ERASE_PAGE;
		WatchDogFeet();
		delay_us(2000);
		ECON = WRITE;
		delay_us(380);
		ECON = VERIFY;
		nop;nop;nop;nop;nop;
		if(ECON)
		{
			nop;nop;nop;nop;nop;
			if(ECON)
				return 0;
			else
				return 1;
		}
		else
			return 1;	
	}
	else
		return 0;
}
// bit WriteByte(unsigned int Addr,unsigned char bbyte)
// {
// 	unsigned int tPage,tAdd;
// 	tPage = Addr / 4;
// 	tAdd = Addr % 4;
// 	
// 	EADR = tPage;
// 	
// 	ECON = READ_BYTE;	
// 	nop;nop;nop;
// 	switch(tAdd)
// 	{
// 		case 0:
// 			EDATA1 = bbyte;
// 			break;
// 		case 1:
// 			EDATA2 = bbyte;
// 			break;
// 		case 2:
// 			EDATA3 = bbyte;
// 			break;
// 		case 3:
// 			EDATA4 = bbyte;
// 			break;
// 		default:
// 			return 0;
// 			break;
// 	}
// 	ECON = ERASE_PAGE;
// 	delay_us(2000);
// 	ECON = WRITE;
// 	delay_us(380);
// 	ECON = VERIFY;
// 	nop;nop;nop;nop;nop;
// 	if(ECON)
// 	{
// 		nop;nop;nop;nop;nop;
// 		if(ECON)
// 			return 0;
// 	}
// 	return 1;	
// }
void InitDevice()
{
/*
	unsigned char *ptr;
	unsigned char xdata n = 0,check = 0;
	//采集间隔
	ReadPage(c_caiy,array);
	check = array[0] + array[1];
	check = ~check;
	if(check == array[2])
// 	if((0xFF - array[0] - array[1]) == array[2])//满足校验
	{
		*(char*)&shijian = array[0];
		*((char*)&shijian + 1) = array[1];
		if((shijian < 2)||(shijian > 5))
		{
			shijian = 2;
			array[0] = 0x00;
			array[1] = 0x02;
			array[2] = 0xFC;
			WritePage(0x0050,array);
		}		
	}
 	else
	{
		shijian = 5;
		array[0] = 0x00;
		array[1] = 0x05;
		array[2] = 0xFA;
		WritePage(0x0050,array);
	}
//   	shijian = 2;
// array[0] = *(char*)&shijian;
// array[1] = *((char*)&shijian + 1);
// array[2] = 0xFF - array[0] - array[1];
// SendComStr(array,4);	
//	//软件版本
//	ptr = &SVersion.lenth;
//	for(n = 0; n < 16;n ++)
//	{
//		*ptr = ReadByte(c_soft + n);
//		if(n < 15)		
//			check += *ptr ++ ;
//	}
//	check = ~check;
// 	if(check != SVersion.check)
//	{
//		sprintf(SVersion.wrdata,"G4-V1.00");
//		SVersion.lenth = strlen(SVersion.wrdata);
//		ptr = &SVersion.lenth;
//		check = 0;
//		for(n = 0; n < (SVersion.lenth + 1);n ++)
//		{
//			array[n] = *ptr;
//			check += *ptr ++; 	
//		}
//		SVersion.check = ~check;
//		array[n] = SVersion.check;
//		WriteByte16(0x00,array);
//	}
//    	SendComStr(array,16);
//	//仪器类型
//	ptr = &DTyep.lenth;
//	for(n = 0; n < 16;n ++)
//	{
//		*ptr =  ReadByte(c_yiqi + n);;
//		if(n < 15)		
//			check += *ptr ++;
//	}
//	check = ~check;
//	if(check != DTyep.check)
//	{
// 		sprintf(DTyep.wrdata,"GTP-213S");
//		DTyep.lenth = strlen(DTyep.wrdata);
//		ptr = &DTyep.lenth;
//		check = 0;
//		for(n = 0; n < (DTyep.lenth + 1);n ++)
//		{
//			array[n] = *ptr;
//			check += *ptr ++; 	
//		}
//		DTyep.check = ~check;
//		array[n] = DTyep.check;
//		WriteByte16(0x10,array);
//	}
//   	SendComStr(array,16);
	//仪器编号
//		ptr = &DNum.lenth;
//		for(n = 0; n < 16;n ++)
//		{
//			*ptr =  ReadByte(c_numb + n);;
//			if(n < 15)		
//				check += *ptr ++;
//		}
//		check = ~check;
//		if(check != DNum.check)
//		{
//		sprintf(DNum.wrdata,"20140109");
//		DNum.lenth = strlen(DNum.wrdata);
//		ptr = &DNum.lenth;
//		check = 0;
//		for(n = 0; n < (DNum.lenth + 1);n ++)
//		{
//			array[n] = *ptr;
//			check += *ptr ++; 	
//		}
//		DNum.check = ~check;
//		array[n] = DNum.check;
//		WriteByte16(0x20,array);
//	}
// 	ptr = &DNum.lenth;
//   SendComStr(ptr,16);
// 	//数据结构版本
	ptr = &DtVer.lenth;
	for(n = 0; n < 16;n ++)
	{
		*ptr = ReadByte(c_stric + n);
		if(n < 15)		
			check += *ptr ++ ;
	}
	check = ~check;
 	if(check != DtVer.check)
	{
		sprintf(DtVer.wrdata,"Alpha");
		DtVer.lenth = strlen(DtVer.wrdata);
		ptr = &DtVer.lenth;
		check = 0;
		for(n = 0; n < (DtVer.lenth + 1);n ++)
		{
			array[n] = *ptr;
			check += *ptr ++; 	
		}
		DtVer.check = ~check;
		array[n] = DtVer.check;
		WriteByte16(0x0030,array);
	}
//   	SendComStr(array,16);
*/
;
}