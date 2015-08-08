// #define ad834
#define ad832
#ifdef ad834
#include <ADUC834.H>
#endif
#ifdef ad832
#include <ADUC832.H>
#endif
#include <intrins.h>
#include <stdio.h>					//Prototypes��ԭ�ͣ� for standard I/O functions.				
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
//--------Ƭ�ڴ洢�����ܶ���-----------
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
//-----------���ݴ洢�ṹ��------------
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
//--------����������״̬����-----------
//-------------------------------------
extern unsigned char bdata statusL;  
extern unsigned char idata statusH;  
/* ����,���ڳ����е�λ�ж� */
extern bit fFK;				//�ſ�
extern bit fYQ;				//��λ
extern bit fZT;				//����
extern bit fFT;				//����
extern bit fSL;				//��£
extern bit fCJ;				//�ɼ�
extern bit fTX;				//����̽ͷѡ��1Ϊ�ϣ�0Ϊ��
extern bit fJD;				//�Ƕ�	
//-------------------------------------
//------------ADuc834����--------------
//-------------------------------------
#ifdef ad834
#define SYSCLK 12583

#define JL		P1_0       					// ����������

#define CNV		P2_0         				// ����7980
#define MC2 	P2_1         				// ������ƿ�2
#define MC1 	P2_2         				// ������ƿ�1
#define SL		P2_3         				// ���ڱ���£��
#define FK		P2_4         				// ���ڱ۷ſ���
#define YQ		P2_5	         			// ������λ��
#define JD		P2_6						// �������Ƕ�
#define KA		P2_7						// ģ���Դ���� 

#define JX		P3_3         				// ����7474����
#define clear	P3_4						// ���ղ���Ƶ����
#define SELSX	P3_5						// ����̽ͷѡ�����

#define ReadZhibi(x)	{P2 |= 0x78;fFK = FK; fSL = SL;fYQ = YQ;fJD = JD;}	//��֧�۵�ǰ״̬

//AD834 ADͨ��ѡ��
#define c_adYL	0						// ѹ������
#define c_adWD	1						// Ƭ���¶ȼ��
#define c_adDL	2						// ��ͷ��ѹ
#define c_adSN	3					// Ƭ���¶ȼ��

//ADuc834̽ͷ140mm���
#define PWTIME   	26   						// ����������ʱ
#define PWDEL			10    					// ���������֮���ӳ�ʱ�䣻��ʱ�ȴ����ղ�
#define PWDELW   	16  						// ��ʼ�˲������֮���ӳ� //35
#define PWPLUS   	4    						// ���η������������ӳ�ʱ��

#define TIMEVA		0XF9DA					// ADuc834//2mS=F7CE,1.5ms=F9DA,1.2ms=FB16,1ms=FBE6

#endif
//-------------------------------------
//------------ADuc832����--------------
//-------------------------------------
#ifdef ad832
#define SYSCLK 16777

#define KM		P2_5								// ����ܿ���
//#define MC2 	P0_7         				// ������ƿ�2
//#define MC1 	P0_6         				// ������ƿ�1
#define MC2 	P3_5          			// ������ƿ�2
#define MC1 	P3_6          			// ������ƿ�1
#define SL		P2_4         				// ���ڱ���£��
#define FK		P2_3         				// ���ڱ۷ſ���
#define YQ		P2_2	         			// ������λ��
#define JD		P2_1								// �������Ƕ�
#define SELSX	P2_6								// ����̽ͷѡ�����
#define JL		P2_7       					// ����������

#define clear	P3_3								// ���ղ���Ƶ����
#define KA		P3_5								// ģ���Դ���� 
#define JX		P3_7         				// ����7474����
#define CNV		P2_0         				// ����7980

#define ReadZhibi(x)	{P2 |= 0x1E;fFK = FK; fSL = SL;fYQ = YQ;fJD = JD;}	//��֧�۵�ǰ״̬

// AD832 ADͨ��ѡ��
#define c_adDL	0x10						// ��ͷ��ѹ
#define c_adMC	0x11						// �������
#define c_adYL	0x12						// ѹ������
#define c_adWD	0x13						// �¶�ת����ѹ
#define c_adSN	0x18						// Ƭ���¶ȼ��
#define c_adAG	0x1B						// AGD����
#define c_adRF	0x1C						// �ο���ѹ����

//ADuc832̽ͷ140mm���
//#define PWTIME   	35   					// ����������ʱ
//#define PWDEL			16    				// ���������֮���ӳ�ʱ�䣻��ʱ�ȴ����ղ�
//#define PWDELW   	22  					// ��ʼ�˲������֮���ӳ� //35
//#define PWPLUS   	6    					// ���η������������ӳ�ʱ��
//ADuc832̽ͷ400mm���
// #define PWTIME   	100   					// ����������ʱ
// #define PWDEL			44    					// ���������֮���ӳ�ʱ�䣻��ʱ�ȴ����ղ�
// #define PWDELW   	60  						// ��ʼ�˲������֮���ӳ� //35
// #define PWPLUS   	50    					// ���η������������ӳ�ʱ��
//ADuc832̽ͷ����220mm���
//#define PWTIME   	40   					// ����������ʱ
//#define PWDEL			35    				// ���������֮���ӳ�ʱ�䣻��ʱ�ȴ����ղ�
//#define PWDELW   	20  					// ��ʼ�˲������֮���ӳ� //35
//#define PWPLUS   	25    				// ���η������������ӳ�ʱ��
#define PWTIME   	55   					// ����������ʱ
#define PWDEL			24    				// ���������֮���ӳ�ʱ�䣻��ʱ�ȴ����ղ�
#define PWDELW   	33  					// ��ʼ�˲������֮���ӳ� //35
#define PWPLUS   	20//28    				// ���η������������ӳ�ʱ��

#define TIMEVA		0xFA8A 				// ADuc832//2mS=F514,1.5ms=f7cf,1.2ms=F972,1ms=FA8A

#endif
//-------------------------------------
//----------�������������-----------
//-------------------------------------
#define TURN_P(x)	{KM = 0;MC1 = 1; MC2 = 0; }	//��ת
#define TURN_N(x)	{KM = 0;MC1 = 0; MC2 = 1; }	//��ת
#define STOP(x)		{KM = 1;MC1 = 0; MC2 = 0; }	//ֹͣ
#define SEL_UP(x)	{SELSX = 1;}	//SELECT UP
#define SEL_DW(x)	{SELSX = 0;}	//SELECT DOWM
#define SEL_P_D(x)	{SELSX = ~SELSX;}	//SELECT UP DOWM ����

//-------------------------------------
//--------���ݴ洢�ռ䶨��-------------
//-------------------------------------
#define		c_soft		0x0000						// �����汾����16�ֽ�
#define 	c_yiqi		0x0010						// �������ͣ���16�ֽ�
#define 	c_numb		0x0020						// �������кţ���16�ֽ�
#define 	c_stric		0x0030						// ���ݽṹ����16�ֽ�
#define 	c_jingh		0x0040						// ���ţ���16�ֽ�
#define 	c_caiy		0x0050						// �ɼ��������16�ֽ�
#define 	c_ssxw		0x0060						// �궨����/�����λ����16�ֽ�
#define 	c_zjzl		0x0070						// �գ���16�ֽ�
#define 	c_bank0		0x0080						// �գ���16�ֽ�
#define 	c_bank1		0x0090						// �գ���16�ֽ�
#define 	c_bank2		0x00A0						// �գ���16�ֽ�
#define 	c_bank3		0x00B0						// �գ���16�ֽ�
#define 	c_bdrq		0x00C0						// �궨���ڣ���16�ֽ�

#define 	c_ylbd		0x00D0						// ѹ���궨���ݣ���560�ֽ�

#define 	c_llsd		0x0300						// �����궨�����ϵ�����64�ֽ�
#define 	c_llsz		0x0340						// �����궨�����ϵ�����64�ֽ�
#define 	c_llxd		0x0380						// �����궨�����ϵ�����64�ֽ�
#define 	c_llxz		0x03c0						// �����궨�����ϵ�����64�ֽ�
//-------------------------------------
//--------����֡ͨѶ��ʽ����-----------
//-------------------------------------
//#define	c_status	1						// ������ǰ״̬
//#define c_xiangweih	2						// ��λ��1�洢֡��ַ��λ
//#define c_xiangweil	3						// ��λ��1�洢֡��ַ��λ
//#define c_shengsuh	4						// ����1�洢֡��ַ��λ
//#define c_shengsul	5						// ����1�洢֡��ַ��λ
//#define c_yali		6						// ѹ����λ�洢֡��ַ
//#define c_ylwd		7						// ѹ����λ,�¶ȸ�λ�洢֡��ַ
//#define c_wendu		8						// �¶ȵ�λ�洢֡��ַ
//#define c_xwh		9						// ��λ��2�洢֡��ַ��λ
//#define c_xwl		10						// ��λ��2�洢֡��ַ��λ
//#define c_ssh		11						// ����2�洢֡��ַ��λ
//#define c_ssl		12						// ����2�洢֡��ַ��λ
//#define c_ltdyh		13						//��ͷ��ѹ��λ
//#define c_ltdyl		14						//��ͷ��ѹ��λ

//#define	cStatusH		1						// ������ǰ״̬��λ
//#define	cStatusL		2						// ������ǰ״̬��λ
//#define cMotoAcH 		3						// ���������λ
//#define cMotoAcL		4						// ���������λ
//#define cPressH			5						// ѹ����λ
//#define cPressL			6						// ѹ����λ
//#define cTempH			7						// �¶ȸ�λ
//#define cTempL			8						// �¶ȵ�λ
//#define cFlowAH			9						// ����1��λ
//#define cFlowAL			10					// ����1��λ
//#define cSonicAH		11					// ����1��λ
//#define cSonicAL		12					// ����1��λ
//#define cFlowBH			13					// ����2��λ
//#define cFlowBL			14					// ����2��λ
//#define cSonicBH		15					// ����2��λ
//#define cSonicBL		16					// ����2��λ
typedef enum
{
	cStatusH = 0,										// ������ǰ״̬��λ
	cStatusL,												// ������ǰ״̬��λ
	cCableH,												// ��ͷ��ѹ��λ
//	cCableL,												// ��ͷ��ѹ��λ
//	cMotoAcH,												// ���������λ
	cMotoAcL,												// ���������λ
	cPressH,												// ѹ����λ
	cPressL,												// ѹ����λ
	cTempH,													// �¶ȸ�λ
	cTempL,													// �¶ȵ�λ
	cFlowAH,												// ����1��λ
	cFlowAL,												// ����1��λ
	cFlowBH,												// ����2��λ
	cFlowBL,												// ����2��λ
	cSonicAH,												// ����1��λ
//	cSonicAL,												// ����1��λ
//	cSonicBH,												// ����2��λ
	cSonicBL,												// ����2��λ			
}eInputReg;
//-------------------------------------
//------------�ɼ���ض���-------------
//-------------------------------------
#define dlong	16        					// ���ݳ���16=15(data)+1(Sum)
#define maxloop  	262//260  				// ÿ�β�����������
#define No       	2
#define Yes      	1
//-------------------------------------
//------------����ģʽ����-------------
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
//------------ȫ�ֱ���˵��-------------
//-------------------------------------
extern	unsigned char	xdata	inputParam[20];					// �������ݴ洢����
extern	unsigned char	xdata	array[20];				// �������ݴ洢����

extern	unsigned int	idata	shijian;					// ���Ź��������(n*2.3S)
extern	unsigned int	idata	shengsu;					// ÿ������ֵ
extern	unsigned int	idata	advalue;					// ÿ��AD���
extern 	unsigned int 	idata zbFlag;

extern unsigned char	xdata	DatPocket[30];		//���ڽ������ݿռ�
extern unsigned char	data	ucDatLong;
//extern unsigned char	idata	Jiaodu;						// ÿһ���Ƕ���ת60�Ƚ�
//extern unsigned char	idata	Tnow;							//����������
extern unsigned char	idata	Tdo;							//������������
extern eWorkMode			idata	Task[5];					//���������Ƚ��ȳ�
extern eWorkMode		  idata uartCreateTask;				//�����жϱ�־
//extern unsigned char idata uartIqrFlag;				//���ڴ��������־
//extern unsigned char	idata	Clting;				//�ɼ������б�ʶ
//extern unsigned char	idata	Kbing;				//���۽����б�ʶextern 
//extern unsigned char	idata	Sbing;				//�ձ۽����б�ʶextern 
//extern unsigned int				Moto_A;
//-------------------------------------
//-------------��������-------==-------
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
	