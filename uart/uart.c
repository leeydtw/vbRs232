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

// 選擇顯示元件fc:7SEG fb:ledbar f7:led f3:dotmatrix
void DisplaySelect(uchar val_1)
{
	uchar i;
	for (i=0;i<8;i++)
	{
		SRCLK = 0;			 // 串列移位時脈輸出低電位，可改變數據狀態
		if (val_1 & 0x80)	 // 高位元先移入
			SER = 1;
		else
			SER = 0;
		SRCLK = 1;			 // 串列移位時脈輸出高電位，數據移入
		val_1 <<= 1;		 // 輸入數據左移一位
	}
	RCLK = 0;				 // 移位暫存器鎖存至輸出正反器
	RCLK = 1;				 // 上升緣鎖存輸出
}

// 七段顯示器位選輸出
void DigitSelect(uchar val_2)
{
	uchar i;
	for (i=0;i<8;i++)
	{
		SRCLK7 = 0;			 // 串列移位時脈輸出低電位，可改變數據狀態
		if (val_2 & 0x80)	 // 高位元先移入
			SER7 = 1;
		else
			SER7 = 0;
		SRCLK7 = 1;			 // 串列移位時脈輸出高電位，數據移入
		val_2 <<= 1;		 // 輸入數據左移一位
	}
	RCLK7 = 0;				 // 移位暫存器鎖存至輸出正反器
	RCLK7 = 1;				 // 上升緣鎖存輸出
}

// 延遲1mS函數 (OSC=12MHz)
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

// 自主機收到一個串列數據後再傳送給主機
void main(void)
{
	uchar key_row, key_col, key;
	DisplaySelect(0xfc);	// 選擇7SEG元件顯示
	DigitSelect(dig[0]);	// 選擇7SEG LED2-C7位顯示
	P2 = seg[0];

	TMOD = 0x20;			// 設置計時器1模式2
	TH1 = 0xf3;				// fosc=12M,4800bps之初值
	TL1 = 0xf3;
	TR1 = 1;				// 啟動計時器1
	PCON |= 0x80;			// SMOD=1波特率加倍
	SM0 = 0;
	SM1 = 1; 				// 設置串列傳輸模式1
	REN = 1;				// 設置接收致能
	ES = 1;					// 打開串列中斷開關
	EA = 1;					// 打開中斷總開關

	while(1)
	{
		P0 = 0x0f;						   	// P0輸出0000 1111 (P37P36...P31P30)	
		key_row = P0;						// 自P0讀回按鍵值
		key_row = key_row & 0x0f;			// 保留低4位的值(列值)
		if (key_row != 0x0f)			   	// 若不等於0x0f表示有鍵被按下
		{
			delay_1mS(20);		   			// 消除按鍵抖動
			if (key_row != 0x0f)		   	// 若不等於0x0f表示確實有鍵被按下
			{
				key_row = P0 & 0x0f;		// 再從P0讀回按鍵值0000 1110(假設S1鍵被按下)
				key_row = key_row | 0xf0;	// 將高4位設為1    1111 1110
				P0 = key_row;				// 把1111 1110輸出至P0
				key_col = P0;				// 從P0讀回值      0111 1110
				key_col = key_col & 0xf0;	// 取高4位為行值   0111 0000		 
				key_row = key_row & 0x0f;	// 取高4位為列值   0000 1110
				key = key_row+key_col;		// 相加為鍵值      0111 1110
			}
		}
		else	
			flag = 0;

		switch(key)
		{
			case 0x7e : if (flag == 0) {SBUF = 0x30; keyval = 0;} 
						break;							// key=0x7e表示S0被按下
			case 0x7d : if (flag == 0) {SBUF = 0x31; keyval = 1;}
						break;							// key=0x7d表示S1被按下
			case 0x7b : if (flag == 0) {SBUF = 0x32; keyval = 2;}
						break;							// key=0x7b表示S2被按下
			case 0x77 : if (flag == 0) {SBUF = 0x33; keyval = 3;}
						break;							// key=0x77表示S3被按下
			case 0xbe : if (flag == 0) {SBUF = 0x34; keyval = 4;} 
						break;							// key=0xbe表示S4被按下
			case 0xbd : if (flag == 0) {SBUF = 0x35; keyval = 5;} 
						break;							// key=0xbd表示S5被按下
			case 0xbb : if (flag == 0) {SBUF = 0x36; keyval = 6;}
						break;							// key=0xbb表示S6被按下
			case 0xb7 : if (flag == 0) {SBUF = 0x37; keyval = 7;} 
						break;							// key=0xb7表示S7被按下
			case 0xde : if (flag == 0) {SBUF = 0x38; keyval = 8;} 
						break;							// key=0xde表示S8被按下
			case 0xdd : if (flag == 0) {SBUF = 0x39; keyval = 9;} 
						break;							// key=0xdd表示S9被按下
			case 0xdb : if (flag == 0) {SBUF = 0x41; keyval = 10;}
						break;							// key=0xdb表示S10被按下
			case 0xd7 : if (flag == 0) {SBUF = 0x42; keyval = 11;}
						break;							// key=0xd7表示S11被按下
			case 0xee : if (flag == 0) {SBUF = 0x43; keyval = 12;}
						break;							// key=0xee表示S12被按下
			case 0xed : if (flag == 0) {SBUF = 0x44; keyval = 13;}
						break;							// key=0xed表示S13被按下
			case 0xeb : if (flag == 0) {SBUF = 0x45; keyval = 14;} 
						break;							// key=0xeb表示S14被按下
			case 0xe7 : if (flag == 0) {SBUF = 0x46; keyval = 15;}
						break;							// key=0xe7表示S15被按下
		}
		P2 = seg[keyval];		
	}
}

void serial() interrupt 4
{	
	if (TI == 1)			// 等待數據傳送完畢
	{
		TI = 0;					// 手動將旗號TI清零
		flag = 1;				// 將旗號flag置1表示傳送一個串列數據
	}
	if (RI == 1)			// 等待數據接收完畢
	{
		RI = 0;					// 手動將旗號RI清零
		flag = 1;
		keyval = SBUF-48;
		P2 = seg[keyval];
	}
}
