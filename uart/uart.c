// uart_1.c
#include"at89x52.h"
#include <intrins.h>

typedef  unsigned char  uchar;
typedef  unsigned int  uint;

#define RCLK 	P1_0
#define SRCLK	P1_1
#define SER 	P1_2
#define RCLK7 	P3_5
#define SRCLK7	P3_6
#define SER7 	P3_7

uchar  seg[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
uchar  dig[]={0xBF, 0xDF, 0xEF, 0xFB, 0xFD, 0xFE};	// c7,c6,c5,c3,c2,c1

// �����ܤ���fc:7SEG fb:ledbar f7:led f3:dotmatrix
void DisplaySelect(uchar val_1)
{
	uchar i;
	for (i=0;i<8;i++)
	{
		SRCLK = 0;			 // ��C����ɯ߿�X�C�q��A�i���ܼƾڪ��A
		if (val_1 & 0x80)	 // ���줸�����J
			SER = 1;
		else
			SER = 0;
		SRCLK = 1;			 // ��C����ɯ߿�X���q��A�ƾڲ��J
		val_1 <<= 1;		 // ��J�ƾڥ����@��
	}
	RCLK = 0;				 // ����Ȧs����s�ܿ�X���Ͼ�
	RCLK = 1;				 // �W�ɽt��s��X
}

// �C�q��ܾ�����X
void DigitSelect(uchar val_2)
{
	uchar i;
	for (i=0;i<8;i++)
	{
		SRCLK7 = 0;			 // ��C����ɯ߿�X�C�q��A�i���ܼƾڪ��A
		if (val_2 & 0x80)	 // ���줸�����J
			SER7 = 1;
		else
			SER7 = 0;
		SRCLK7 = 1;			 // ��C����ɯ߿�X���q��A�ƾڲ��J
		val_2 <<= 1;		 // ��J�ƾڥ����@��
	}
	RCLK7 = 0;				 // ����Ȧs����s�ܿ�X���Ͼ�
	RCLK7 = 1;				 // �W�ɽt��s��X
}

// ����1mS��� (OSC=12MHz)
void delay_1mS(uchar t)
{
	uchar i,j;
	do {
	   i=4;
	   do {
	   	   j=82;
		   do {
		       _nop_();
		   } while(--j);	
	   } while (--i);
	} while(--t);
}

uchar	flag = 0;
char  keyval;

// �ۥD������@�Ӧ�C�ƾګ�A�ǰe���D��
void main(void)
{
	uchar key_row, key_col, key;
	DisplaySelect(0xfc);	// ���7SEG�������
	DigitSelect(dig[0]);	// ���7SEG LED2-C7�����
	P2 = seg[0];

	TMOD = 0x20;			// �]�m�p�ɾ�1�Ҧ�2
	TH1 = 0xf3;				// fosc=12M,4800bps�����
	TL1 = 0xf3;
	TR1 = 1;				// �Ұʭp�ɾ�1
	PCON |= 0x80;			// SMOD=1�i�S�v�[��
	SM0 = 0;
	SM1 = 1; 				// �]�m��C�ǿ�Ҧ�1
	REN = 1;				// �]�m�����P��
	ES = 1;					// ���}��C���_�}��
	EA = 1;					// ���}���_�`�}��

	while(1)
	{
		P0 = 0x0f;						   	// P0��X0000 1111 (P37P36...P31P30)	
		key_row = P0;						// ��P0Ū�^�����
		key_row = key_row & 0x0f;			// �O�d�C4�쪺��(�C��)
		if (key_row != 0x0f)			   	// �Y������0x0f��ܦ���Q���U
		{
			delay_1mS(20);		   			// ��������ݰ�
			if (key_row != 0x0f)		   	// �Y������0x0f��ܽT�꦳��Q���U
			{
				key_row = P0 & 0x0f;		// �A�qP0Ū�^�����0000 1110(���]S1��Q���U)
				key_row = key_row | 0xf0;	// �N��4��]��1    1111 1110
				P0 = key_row;				// ��1111 1110��X��P0
				key_col = P0;				// �qP0Ū�^��      0111 1110
				key_col = key_col & 0xf0;	// ����4�쬰���   0111 0000		 
				key_row = key_row & 0x0f;	// ����4�쬰�C��   0000 1110
				key = key_row+key_col;		// �ۥ[�����      0111 1110
			}
		}
		else	
			flag = 0;

		switch(key)
		{
			case 0x7e : if (flag == 0) {SBUF = 0x30; keyval = 0;} 
						break;							// key=0x7e���S0�Q���U
			case 0x7d : if (flag == 0) {SBUF = 0x31; keyval = 1;}
						break;							// key=0x7d���S1�Q���U
			case 0x7b : if (flag == 0) {SBUF = 0x32; keyval = 2;}
						break;							// key=0x7b���S2�Q���U
			case 0x77 : if (flag == 0) {SBUF = 0x33; keyval = 3;}
						break;							// key=0x77���S3�Q���U
			case 0xbe : if (flag == 0) {SBUF = 0x34; keyval = 4;} 
						break;							// key=0xbe���S4�Q���U
			case 0xbd : if (flag == 0) {SBUF = 0x35; keyval = 5;} 
						break;							// key=0xbd���S5�Q���U
			case 0xbb : if (flag == 0) {SBUF = 0x36; keyval = 6;}
						break;							// key=0xbb���S6�Q���U
			case 0xb7 : if (flag == 0) {SBUF = 0x37; keyval = 7;} 
						break;							// key=0xb7���S7�Q���U
			case 0xde : if (flag == 0) {SBUF = 0x38; keyval = 8;} 
						break;							// key=0xde���S8�Q���U
			case 0xdd : if (flag == 0) {SBUF = 0x39; keyval = 9;} 
						break;							// key=0xdd���S9�Q���U
			case 0xdb : if (flag == 0) {SBUF = 0x41; keyval = 10;}
						break;							// key=0xdb���S10�Q���U
			case 0xd7 : if (flag == 0) {SBUF = 0x42; keyval = 11;}
						break;							// key=0xd7���S11�Q���U
			case 0xee : if (flag == 0) {SBUF = 0x43; keyval = 12;}
						break;							// key=0xee���S12�Q���U
			case 0xed : if (flag == 0) {SBUF = 0x44; keyval = 13;}
						break;							// key=0xed���S13�Q���U
			case 0xeb : if (flag == 0) {SBUF = 0x45; keyval = 14;} 
						break;							// key=0xeb���S14�Q���U
			case 0xe7 : if (flag == 0) {SBUF = 0x46; keyval = 15;}
						break;							// key=0xe7���S15�Q���U
		}
		P2 = seg[keyval];		
	}
}

void serial() interrupt 4
{	
	if (TI == 1)			// ���ݼƾڶǰe����
	{
		TI = 0;					// ��ʱN�X��TI�M�s
		flag = 1;				// �N�X��flag�m1��ܶǰe�@�Ӧ�C�ƾ�
	}
	if (RI == 1)			// ���ݼƾڱ�������
	{
		RI = 0;					// ��ʱN�X��RI�M�s
		flag = 1;
		keyval = SBUF-48;
		P2 = seg[keyval];
	}
}
