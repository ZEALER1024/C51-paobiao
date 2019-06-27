#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char
sbit S1 = P1^3;
sbit S2 = P1^2;
sbit S3 = P1^1;
sbit S4 = P1^0;
sbit a = P0^0;
sbit b = P0^1;
sbit c = P0^2;
sbit d = P0^3;
sbit e = P0^4;
sbit f = P0^5;
sbit g = P0^6;
sbit p = P0^7;
sbit key1 = P3^4;
sbit key2 = P3^5;
sbit ledm = P2^0;
uchar num,kms,sec,min;
uchar code N[10] = {0xc0, 0xf9 ,0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90}; //0, 1, 2, 3, ...8, 9
uchar code Z[10] = {0x40, 0x79 ,0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10}; //0., 1., 2., ... 8., 9.

void delayms(uint xms)
{
	uint i,j;
	for(i = xms; i > 0; i--)
		for(j = 110; j > 0; j--);
}

void init()
{
	TMOD = 0x01;
  TL0 = 0x00;		
	TH0 = 0x4C;		
	S3 = 1;
	S1 = S2 = S4 = 0;
	P0 = N[0];
	delayms(5);

	S3 = 0;
	S1 = S2 = S4 = 1;
	P0 = Z[0];
	delayms(5);

	EA = 1;
	TR0 = 0;
	ET0 = 1;	
	kms = sec = min = 0;
}

void display()
{
	uchar shi, ge;
	if(kms >= 0)
	{
		S4 = 1;
		S1 = S2 = S3 = 0;
		P0 = N[kms];
	}
	delayms(5);
	if(sec >= 0)
	{
		shi = sec/10;
		ge = sec%10;

		S3 = 1;
		S1 = S2 = S4 = 0;
		P0 = Z[ge];
		delayms(5);

		S2 = 1;
		S1 = S3 = S4 = 0;
		P0 = N[shi];
	}
	delayms(5);
	if(min >= 0)
	{
		S1 = 1;
		S2 = S3 = S4 = 0;
		P0 = Z[min];
	}
	delayms(5);
}

void keyscan()
{
	if(key1 == 0)
	{
		delayms(10);
		if(key1 == 0)
		{
			while(!key1);
			TR0 = ~TR0;
		}
	}
	if(key2 == 0)
	{
		delayms(10);
		if(key2 == 0)
		{
			min = sec = kms = 0;
			while(!key1);
		}
	}
}
main()
{
	init();
	while(1)
	{
		keyscan();
		display();
  }
}
void T0_time() interrupt 1
{
  TL0 = 0x00;		
	TH0 = 0x4C;		
		num++;
	if(num == 2)
	{
		num = 0;
		kms++;
  	ledm=1;
		if(kms == 10)
		{
			kms = 0;
			ledm=kms;
			sec++;
			if(sec == 60)
			{
				sec = 0;
				min++;
				if(min == 10)
				{
					TR0 = 0;
					min = 9;
					sec = 59;
					kms = 9;
				}
			}
		}
	}
} 