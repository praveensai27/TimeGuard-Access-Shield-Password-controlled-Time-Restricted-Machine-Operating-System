#include "system.h"
#include "types.h"
#include "kpm.h"
#include "lcd.h"
#include "delay.h"
#include "lcd_defines.h"
#include <lpc21xx.h>
#include "rtc.h"
#define sw 1//p0.1
#define led 0
u32 key;
int main()
{
	IODIR0 |=1<<led;
	Init_system();
	while(1)
	{
		CmdLCD(0x01);
			do
			{
				display_RTC();
				
				}while((ColScan()));
			key=KeyScan();
			
			if(key=='e')
			{
			  if(password())
			{
				CmdLCD(0x01);
				 if(valid_entry_or_not())
				 {
					 	 StrLCD("welcome");
				   					 						 
							IOCLR0 =1<<led;
							delay_ms(100);
							IOSET0 =1<<led;
						 delay_ms(100);
					  
								delay_s(1);
				 }
				 else
				 {
					  StrLCD("invalid time");
					 					 delay_s(1);
				 }
			}
		}
		
			
	 
}
}

