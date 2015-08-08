#include "Head.h"

unsigned char xdata	DatPocket[30];		  	//���ڽ������ݿռ�
unsigned int xdata nrev_check;//CRCУ���
unsigned char data ucDatLong;
//=================================================================== 
void Sendcomchar(unsigned char buff);
void SendWrongFalg(unsigned char ucFlag);
void ReadandSend(unsigned int add);
void ReceiveandSave(unsigned int add);

extern unsigned int CRC16(unsigned char xdata * data puchMsg, unsigned int usDataLen);
extern void Pocket(unsigned char * Data,unsigned char D_Long);
//extern	unsigned char	xread_data(void);
extern	void	xwrite_data();
extern	void	init_t3();
extern void init_tic(void);
//=================================================================== 

void RTU_read_comm(void)
{
	unsigned int data read_lenth,loop_verp;
// 	signed char *str_ptr1,
	unsigned char xdata * data str_ptr1;
	unsigned char xdata * data str_ptr2;
	unsigned int  data temp=0x00;
	unsigned int  data address=0x00;
//	unsigned char xdata aa[5] = {0x11,0x22,0x33,0x44,0x55};
	nrev_check = CRC16(DatPocket,ucDatLong - 2);
	*(char*)&temp = DatPocket[ucDatLong - 2]; 				                // ����У����			              
	*((char*)&temp + 1)= DatPocket[ucDatLong - 1];
	if(temp==nrev_check)					                        // У������ȷ
  {		
		
		*(char*)&address = DatPocket[2];						// ������ַ
		*((char*)&address + 1) = DatPocket[3];				
		*(char*)&read_lenth = DatPocket[4];						// ����������
		*((char*)&read_lenth + 1) = DatPocket[5];	
		if((DatPocket[0] == 0x88)||(DatPocket[0] == 0x00))                           // ����豸��ַ��
    {
			switch(DatPocket[1])
			{

				case 0x08://DatPocket//���ؽ��յ������ݣ����ڲ�������
					if((DatPocket[2] == 0x00)&&(DatPocket[3] == 0x00))
					{
						nrev_check = CRC16(DatPocket,6);
						DatPocket[6] = *(char*)&nrev_check;
						DatPocket[7] = *((char*)&nrev_check + 1);
						//��ʼ��������
 						SendComStr(DatPocket,8);
					}
				break;
				case 0x01:
 					if(address == 0xFFF0)//������״̬(01 FF F0 00 01)
					{
						if(read_lenth > 8)
						{
							SendWrongFalg(0x03);//��ȡ��������,���ش������0x02	
						}
						else
						{													
  						ReadZhibi(x)
							DatPocket[2] = 0x01;
							DatPocket[3] = statusL;
							nrev_check = CRC16(DatPocket,4);
							DatPocket[4] = *(char*)&nrev_check;
							DatPocket[5] = *((char*)&nrev_check + 1);
							SendComStr(DatPocket,6);
						}
					}
					else
						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02
				break;	
				case 0x03://��E2proom�洢������
					read_lenth *= 2;										
					if(address < 0x1000)//(03 00 10 00 08)
					{
						if(read_lenth == dlong)
						{							
							ReadandSend(address);
						}
						else
							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03	
					}
					else
						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02
				break;
				case 0x04://������Ĵ���
					if((read_lenth >= 0x0001)&&(read_lenth <= 0x0009))
					{
							if((address < 0x0009) && (address + read_lenth) <= 0x0009) 
							{
								DatPocket[2] = read_lenth * 2;
//								ReadZhibi(x)
								inputParam[cStatusH] = statusH;
								inputParam[cStatusL] = statusL;
								str_ptr1 = &DatPocket[3];
								str_ptr2 = &inputParam[address * 2];
								for(loop_verp = 0; loop_verp < read_lenth * 2; loop_verp++) 
									*str_ptr1++ = *str_ptr2++;								
								Pocket(DatPocket,(3 + read_lenth * 2));
								SendComStr(DatPocket,(5 + read_lenth * 2));				

								if((address > 0x0003)||((address + read_lenth) > 0x0004))
									uartCreateTask = ModeCj;
									TaskCreate(ModeCj);
							}
							else
								SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02
					}
					else
						SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03	
//					if(address == 0xF0F0)//��ʵʱ��������(04 F0 F0 00 08)
//					{
//						if(read_lenth <= 16)
//						{
//							DatPocket[2] = read_lenth * 2;
//							ReadZhibi(x)
//							inputParam[1] = status;
//							str_ptr1 = &DatPocket[3];
//							str_ptr2 = &inputParam[1];
//							for(loop_verp = 0; loop_verp < 14; loop_verp++) 
//								*str_ptr1++ = *str_ptr2++;
//							nrev_check = CRC16(DatPocket,17);
//							DatPocket[17] = *(char*)&nrev_check;
//							DatPocket[18] = *((char*)&nrev_check + 1);
//							SendComStr(DatPocket,19);
//						}
//						else
//							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03	
//					}
//					else if(address == 0xF0FF)//����һ�ε������(04 F0 FF 00 01)
//					{
//						if(read_lenth == 0x01)
//						{							
//							DatPocket[2] = 0x02;

//							DatPocket[3] = *(char*)&Moto_A;
//							DatPocket[4] = *((char*)&Moto_A + 1);
//			
//							nrev_check = CRC16(DatPocket,5);
//							DatPocket[5] = *(char*)&nrev_check;
//							DatPocket[6] = *((char*)&nrev_check + 1);
//							SendComStr(DatPocket,7);
//						}
//						else
//							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03
//					}
//					else
//						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02
				break;
				case 0x05://д��Ȧ
					
					if(read_lenth == 0xFF00)
					{
						if(address == 0x0000)
						{
							TaskCreate(ModeKb);	
							SendComStr(DatPocket,8);							
						}
						else if(address == 0x0001)
						{
							TaskCreate(ModeSl);	
							SendComStr(DatPocket,8);
						}
						else if(address == 0x0002)
						{
							TaskCreate(ModeZt);	
							SendComStr(DatPocket,8);
						}
						else if(address == 0x0003)
						{
							TaskCreate(ModeFt);	
							SendComStr(DatPocket,8);
						}
						else
							SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02	
//						switch(address)
//						{
//							case 0x0000:
//								TaskCreate(ModeKb);	
//								SendComStr(DatPocket,8);
////								uartCreateTask = ModeKb;
//								break;
//							case 0x0001:
//								TaskCreate(ModeSl);	
//								SendComStr(DatPocket,8);
////								uartCreateTask = ModeSl;
//								break;
//							case 0x0002:
//								TaskCreate(ModeZt);	
//								SendComStr(DatPocket,8);
////								uartCreateTask = ModeZt;
//								break;
//							case 0x0003:
//								TaskCreate(ModeFt);	
//								SendComStr(DatPocket,8);
////								uartCreateTask = ModeFt;
//								break;
//							default:
//								SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02	
//								break;
//						}
					}
					else if(read_lenth == 0x0000)
					{
						switch(address)
						{
							case 0x0000:
							case 0x0001:
							case 0x0002:
							case 0x0003:
								SendComStr(DatPocket,8);
								TaskCreate(ModeST);	
//								uartCreateTask = ModeST;
								break;
							default:
								SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02	
								break;
						}
					}						
					else
							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03	
//					if(address == 0xFFF0)//�򿪻�رշ���	
//					{
//						if(read_lenth == 0x00F0) //ѡ����̽ͷ	(05 FF F0 00 F0)
//						{
//							SEL_UP(x)
//							SendComStr(DatPocket,8);
//							statusL |= 0x40;
//						}
//						else if(read_lenth == 0x000F) //ѡ����̽ͷ	(05 FF F0 00 0F)
//						{
//							SEL_DW(x)
//							SendComStr(DatPocket,8);
//							statusL &= ~0x40;
//						}						
//						else if(read_lenth == 0xFF00) //���뿪�۶���	(05 FF F0 FF 00)
//						{
//							TaskCreate(ModeKb);	
//							SendComStr(DatPocket,8);
////							TURN_N(x)	
//						}
//						else if(read_lenth == 0x0000) //�����ձ۶���(05 FF F0 00 00)
//						{
//							TaskCreate(ModeSl);
//							SendComStr(DatPocket,8);
////							TURN_P(x)
//						}
//						else if((read_lenth >= 0xFF01)&&(read_lenth < 0xFFFF)) //����60��*Jiaodu
//						{
//							TaskCreate(ModeZt);
//// 							status &= ~0x04; //��λ����״̬
//							SendComStr(DatPocket,8);
//							Jiaodu = DatPocket[5];//&0x0F;
//// 							TURN_P(x)
//						}
//						else if((read_lenth >= 0xFE01)&&(read_lenth < 0xFEFF)) //����60��*Jiaodu
//						{
//							TaskCreate(ModeFt);
//// 							status &= ~0x08; //��λ����״̬
//							SendComStr(DatPocket,8);
// 							Jiaodu = DatPocket[5];
//// 							TURN_N(x) 
//						}
//						else if(read_lenth == 0xFFFF) //ֹ֧ͣ�۶���  (05 FF F0 FF FF)
//						{
//							TaskCreate(ModeST);
//							for(loop_verp = 0;loop_verp < 10; loop_verp ++)
//							{
//								if((Task[loop_verp] >= ModeKb)&&(Task[loop_verp] <= ModeZt))
//									Task[loop_verp] = ModeST;
//							}
//							Jiaodu = 0;
//// 							Scomd = ModeST;
//// 							status |= 0x04; //������״̬
//// 							status |= 0x08; //�巴��״̬
//// 							Jiaodu = 0;
// 							SendComStr(DatPocket,8);
//// 							STOP(x)
//						}
//						else
//							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03	
//					}
//					else
//						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02					
				break;

				case 0x06://д�����Ĵ���
					if(address == 0xFF00)	   //����ɼ�(06 FF 00 FF 00)
					{
						if(read_lenth == 0xFF00)
						{
							TaskCreate(ModeTicOn);
//							init_tic();								// ��ʼ�����������ʱ��
//							IEIP2=IEIP2|0x04;						// ����TIC�ж� ETI��1 Written by User to Enable ��1�� or Disable ��0�� Time Interval Counter Interrupt.

						}
						else
						{
							TaskCreate(ModeTicOff);

							for(loop_verp = 0;loop_verp < 10; loop_verp ++)
							{
								if(Task[loop_verp] == ModeCj)
									Task[loop_verp] = ModeTicOff;
							}							
// 							Scomd = 0x00;
// 							ScomdCj = 0x00;
// 							status |= 0x20;
// 							TIMECON=TIMECON&0xFC;					// ֹͣTIC2007.9
// 							IEIP2=IEIP2&0xFB;						// ��ֹTIC�ж�2007.9
						}
						SendComStr(DatPocket,8);
					}					
// 					else if(address == 0xFFFF)	   //дE2proomƬѡ(06 FF FF 00 00)
// 					{
// // 						if(read_lenth == 0xFF00)	
// // 							cs = 1;
// // 						else 
// // 							cs = 0;
// 						SendComStr(DatPocket,8);
// 					}					
					else
						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02					
				break;
				case 0x10://дE2proom
					read_lenth *= 2;
					if(address < 0x1000)
					{
						if(read_lenth == dlong)
						{							
// 							if(address == 0x0090)
//  							{
// 	
// 								*(char*)&shijian = DatPocket[5];
// 								*((char*)&shijian + 1) = DatPocket[6];								
// // 								shijian = DatPocket[6];	
// 								if(shijian < 2)
// 									shijian = 2;
// 								else 
// 									shijian = 5;
// 							}
							ReceiveandSave(address);
						}
						else
							SendWrongFalg(0x03);//�Ĵ�����������,���ش������0x03						
					}
					else
					{
						SendWrongFalg(0x02);//��֧�ִ˵�ַ,���ش������0x02
						break;
					}
					nrev_check = CRC16(DatPocket,6);
					DatPocket[6] = *(char*)&nrev_check;
					DatPocket[7] = *((char*)&nrev_check + 1);
					SendComStr(DatPocket,8);
				break;
				default:
					SendWrongFalg(0x01);//��֧�ִ˹�����,���ش������0x01
				break;			
			}
		}
	}
//	SendComStr(DatPocket,ucDatLong);
	ucDatLong = 0;	
}
// ͨѶʱ���������ӳ��򣨵����ã�====================================
void Sendcomchar(unsigned char buff)
{
	ES = 0;
	REN = 0;
	TI=0;											// 2007.8
	SBUF=buff;
	while(!TI)
	{
		WDCON |= 0x01;						// ���Ź��Ĵ���д����
		WDCON  = 0x72;						// ���ÿ��Ź���ʱ2000ms����������Ź���ʱ�����ر�д����
	}
//	for (buff=0;TI==0;)
//	{
//		if (buff>=250) return;
//		buff++;
//	}
	REN = 1;
	ES = 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendComStr(unsigned char *string,unsigned int num)
{
	unsigned int xdata i=0x00;
	ES = 0;
	REN = 0;	
//	for(i=0;i<10;i++);
	for(i=0;i<num;i++)
	{
		Sendcomchar(*string++);	 
	}
	ES = 1;
	REN = 1;
//	for(i=0;i<10;i++);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����: SendWrongFalg
// ˵��:  
// ����: ���ش������02��֧�ִ˵�ַ03��֧�ִ˳���
// ����:
// ����:  firstaddress ��ʼ��ַ  bytes       �������ֽ�����
// ����: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SendWrongFalg(unsigned char ucFlag)
{
	unsigned int xdata rev_check;
	DatPocket[1] += 0x80;
	DatPocket[2] = ucFlag;
	rev_check = CRC16(DatPocket,3);
	DatPocket[3] = *(char*)&rev_check;
	DatPocket[4] =	*((char*)&rev_check+1);					
	SendComStr(DatPocket,5);																		//��ʼ��������
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����: ReadandSend
// ˵��:  
// ����: ��Flash��������Com�ڷ�������
// ����:
// ����:  
// ����: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadandSend(unsigned int add)
{
	unsigned char  xdata n;
	signed char *sp;
//	dptr=add;
	sp = &DatPocket[3];
	for (n = 0;n<dlong;n++)	// �Ӵ洢������ģ��,dlong=16
	{
// 		*sp ++ = xread_data();					// ����1B
		*sp ++ = ReadByte(add + n);					// ����1B
//		sum=sum + *sp++;
	}
	DatPocket[2] = dlong;
//DatPocket[6] = shijian;	
//	DatPocket[dlong + 3] = sum;
//	DatPocket[dlong + 4] = 0;
	nrev_check = CRC16(DatPocket,(dlong + 3));
	DatPocket[dlong + 3] = *(char*)&nrev_check;
	DatPocket[dlong + 4] = *((char*)&nrev_check + 1);
	SendComStr(DatPocket,(dlong + 5));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����: ReceiveandSave
// ˵��:  
// ����: ��Com�ڶ�������д��Flash����
// ����:
// ����:  
// ����: 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ReceiveandSave(unsigned int add)
{
// 	unsigned char xdata n;
// 	signed char *sp;
// 	dptr=add;
// 	sp = &DatPocket[7];
// 	for (n = 0;n<dlong;n++,dptr++)	// �Ӵ洢������ģ��,dlong=16
// 	{
// 		dataa = *sp ++;                            
// 		xwrite_data();				// ָ����ַд1B
// 	}
 	WriteByte16(add,&DatPocket[7]);
}
