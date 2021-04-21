#include <reg52.h>

#include<stdlib.h>
#define uchar unsigned char
#define DIV 0x0d
#define EQU 0x0e
#define A 0x0a
/*********************************步进电机正反转控制设计**************************************************麦龙泉************/
/***********************功能：启动、停止、正转、反转、减速、加速（正转1，反转0，停止2，加速A，减速S）**********************/
#define CLR 0x0f
sbit key1=P3^0;
sbit key2=P3^1;
sbit led1=P1^6;
sbit led2=P1^7;
xdata unsigned char OUTBIT _at_	0x9002;	
xdata unsigned char OUTSEG	_at_	0x9004;   
xdata unsigned char IN	_at_	0x9001;	
unsigned long int  Result1,Result2,Result3;
unsigned char	LEDBuf[6];		
sbit beep=P2^0;
sbit clock1=P3^2;
unsigned char sum=0,temp;
unsigned int close=10;
//A-AB-B-BC-C-CD-D-DA
unsigned char code F[]= {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09}; 
unsigned char code Z[]={0x09,0x08,0x0C,0x04,0x06,0x02,0x03,0x01};	
unsigned char	LEDMAP[] = {	
	0x3f,	0x06,	0x5b, 0x4f,	0x66,	0x6d, 0x7d,	0x07,
	0x7f,	0x6f,	0x77, 0x7c,	0x39,	0x5e, 0x79,	0x71,
	0x76,   0x38,   0x37, 0x3e, 0x73,   0x5c, 0x40
};
void delay1ms(unsigned int t){//电机延时；
	char i=120;
	while(t--){
		while(i--);
	}
}
void Delay(unsigned char CNT)  
{

	unsigned char i;
	
	while (CNT-- !=0)
	
	for (i=100; i !=0; i--);
			
}
					
void ket(unsigned int x)	 
{	
    unsigned int sum=0,i;
    srand(temp);
	for(i=0;i<4;i++){

	sum=sum*10+rand()%10;
	} 
}

void DisplayLED()			 

{
	
	unsigned char i;
	
	unsigned char Pos;
	
	unsigned char LED;
	
	Pos = 0x20;
	
	for (i = 0; i < 6; i++) {

			OUTBIT = 0;	// trun off all LED
			LED	= LEDBuf[i];	
			OUTSEG = LED;	
			OUTBIT = Pos;	// trun on one LED
			Delay(5);		
			Pos	>>= 1;		
	}			
}			
unsigned char	KeyTable[] = {					 
		0x16,	0x15,	0x14, 0xff,
		0x13,	0x12,	0x11, 0x10,
		0x0d,	0x0c,	0x0b, 0x0a,
		0x0e,	0x03,	0x06, 0x09,
		0x0f,	0x02,	0x05, 0x08,
		0x00,	0x01,	0x04, 0x07
};			

unsigned char TestKey()		   
{
	
	OUTBIT = 0;  
	
	return (~IN & 0x0f); 

}

unsigned char GetKey()				  

{
	unsigned char Pos;	
	unsigned char i;	
	unsigned char k;	
	i = 6;	
	Pos = 0x20;	
	do {	
		OUTBIT = ~ Pos;	
		Pos >>= 1;	
		k = ~IN & 0x0f;	
    } while ((--i != 0) && (k == 0));	
	if (k != 0) {	
		i *= 4;	
		if (k & 2)	
		i += 1;	
		else if (k & 4)	
		i += 2;	
		else if (k & 8)	
		i += 3;	
		OUTBIT = 0;	
		do Delay(10); while (TestKey());
		return(KeyTable[i]);	
     } else 
	 return(0xff);	

}
void DisplayResult(unsigned long int Result1,unsigned long int  Result2,unsigned long int Result3)	  //??????????

{

	unsigned char i;



			
			LEDBuf[0] =LEDMAP[Result1] ;
			LEDBuf[1] = LEDMAP[Result2];
			LEDBuf[2] = LEDMAP[Result3];
			LEDBuf[3]	=0;
			LEDBuf[4]	= 0;
			LEDBuf[5]	= 0;	
	  
}
unsigned int Accelerated(unsigned int gear)//加速,一、二、三档；
{
	unsigned int speed;
	beep=1;
	switch(gear){
		case 2:speed=40;break;
		case 3:speed=20;break;
		case 4:speed=10;break;
		default:break;
	}
		return speed;
}
unsigned int Slowdown(unsigned int gear)//减速，一二三档；
‘{
	unsigned int speed;
	beep=1;
	switch(gear){
		case 2:speed=40;break;
		case 3:speed=20;break;
		case 4:speed=10;break;
		default:break;
}
		return speed;
}
void stop_1()
{
	led1=1;
	led2=0;
	while(key2){};
}
void foreword(	unsigned char Key,unsigned int gear,unsigned int speed)//正转；
{
	unsigned int i;
	for(i =0;i<8*5;i++){
				Key = GetKey();
				DisplayResult(Result1,Result3,Result2);
				DisplayLED();
				if(key2==0)
				{
					DisplayResult(0,0,0);
					DisplayLED();
					stop_1();
					led1=0;
					led2=1;
					i=-1;
					DisplayResult(1,1,0);
				}
				if(Key==2)
				{
					Result3=A;
					gear++;
					Result2=gear;
					speed=Accelerated(gear);
					delay1ms(speed);
					beep=1;
					delay1ms(speed);
					beep=~beep;
					DisplayResult(Result1,Result3,Result2);
				}
				if(Key==3)
				{
					Result3=5;
					gear--;
					Result2=gear;
					speed=Accelerated(gear);
					delay1ms(speed);
					beep=1;
					delay1ms(speed);
					beep=~beep;
					DisplayResult(Result1,Result3,Result2);
				}
				P1 = Z[i];
				delay1ms(speed);	
				}
}
void Reverse(	unsigned char Key,unsigned int gear,unsigned int speed)//反转；
{
	unsigned int i;
				for(i =0;i<8*3;i++){
				Key = GetKey();
				DisplayResult(Result1,Result3,Result2);
				DisplayLED();
				if(key2==0)
				{
					DisplayResult(0,0,0);
					DisplayLED();
					stop_1();
					i=-1;
					DisplayResult(0,1,0);
				}
				if(Key==2)
				{
					Result3=A;
					gear++;
					Result2=gear;
					speed=Accelerated(gear);
					delay1ms(speed);
					beep=1;
					delay1ms(speed);
					beep=~beep;
					DisplayResult(Result1,Result3,Result2);
				}
				if(Key==3)
				{
					Result3=5;
					gear--;
					Result2=gear;
					speed=Accelerated(gear);
					delay1ms(speed);
					beep=1;
					delay1ms(speed);
					beep=~beep;
					DisplayResult(Result1,Result3,Result2);
				}
				P1 = F[i];
				delay1ms(speed);	
				}
}
void main()
{
	unsigned char Key=0;		
  unsigned int i = 0;
	unsigned int speed=60;
	beep=0;		
	Result1= 0;
	Result2=0;
	Result3=1;	
	while(1){
			led1=0;
			led2=0;
		if(key1==0)
		{
			led1=0;
			led2=1;
	while (1) {
		if(key1==0)
		{
			led1=0;
			led2=1;
		while (!TestKey()) DisplayLED();
		Key = GetKey();
		if ( Key==0 ) {
			Result1=1;
			DisplayResult(Result1,Result3,Result2);
			DisplayLED();
			foreword(Key,Result3,speed);
			Result1= 1;
			Result2=0;
			Result3=1;
		}
		else if (Key == 1) {	
			Result1=0;
			DisplayResult(Result1,Result3,Result2);
			DisplayLED();
			Reverse(Key,Result3,speed);
			Result1= 0;
			Result2=0;
			Result3=1;
		} 
		}	
		else
		{
			break;
		}
	}
	}
	else if(key2==0){
			led1=1;
			led2=0;
			DisplayResult(0,0,0);
			DisplayLED();
	}
}
}

