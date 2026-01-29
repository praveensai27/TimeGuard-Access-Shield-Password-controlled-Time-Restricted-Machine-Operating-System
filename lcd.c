#include "types.h"
#include "arduino_defines.h"
#include "arduino.h"
#include "lcd_defines.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
		//select Write operations
		digitalWrite(LCD_RW,0);
		//write any byte onto data pins(d0-d7)
		Write2pins(LCD_DATA,8,byte);
		//provide high to low pulse for latching
		digitalWrite(LCD_EN,1);
		delay_us(1);
		digitalWrite(LCD_EN,0);
		delay_ms(2);
}
void CmdLCD(u8 CmdByte)
{
	//select the register
	digitalWrite(LCD_RS,0);
	//Write to cmd register via data pins
	WriteLCD(CmdByte);
}
void Init_LCD(void)
{
		//cfg lcd data pins and &control pins as gpio out
	portMode(LCD_DATA,11,OUTPUT);
	delay_ms(15);//initialization sequence start
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(4100);
	CmdLCD(MODE_8BIT_1LINE);
	delay_us(100);
	CmdLCD(MODE_8BIT_1LINE);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
	//initialization sequence end
	
}
void CharLCD(u8 asciival)
{
	//select data register
	digitalWrite(LCD_RS,1);
	//write to ddram /display via data register
	WriteLCD(asciival);
}
void StrLCD(s8 *p)
{
	while(*p)
		CharLCD(*p++);
}
void u32LCD(u32 n)
{
	u8 digit[10];
	s32 i=0;
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
			digit[i++]=(n%10)+48;
			n/=10;
		}
		for(--i;i>=0;i--)
		{
			CharLCD(digit[i]);
		}
	}
}
void s32LCD(s32 n)
{
	if(n<0)
	{
		CharLCD('-');
		n=-n;
	}
	u32LCD(n);
	
}
void f32LCD(f32 fn,u32 nDP)
{
	u32 n,i;
	if(fn<0.0)
	{
		CharLCD('-');
		fn=-fn;
	}
	n=fn;
	u32LCD('.');
	for(i=0;i<nDP;i++)
	{
		fn=(fn-n)*10;
		n=fn;
		CharLCD(n+48);
	}
}
void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	//point to cgram start
	CmdLCD(GOTO_CGRAM_START);
	//select data register
	digitalWrite(LCD_RS,1);
	//Write to cgram,req bytes for custom char
	for(i=0;i<nBytes;i++)
	{
		WriteLCD(p[i]);
	}
	//point back to ddram
	CmdLCD(GOTO_LINE1_POS0);
	
}































































