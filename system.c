#include <lpc214x.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"
#include "types.h"
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#define EINT0_VIC_CHNO 14
#define EINT0_PIN_0_1 PIN_FUNC4
#define led 0
 unsigned char forword_arrow[24]={0x00,0x04,0x02,0x1f,0x02,0x04,0,0,
	                                     0x04,0x0e,0x15,0x04,0x04,0x04,0x04,0x04,
                                       0x04,0x04,0x04,0x04,0x15,0x0e,0x04,0x04};
s32 hour,min,sec,month,year,date;
s32 day;
u32 default_password=1234;
u32 user_password=0,login=9,logout=12,time;
//char pass1[5]="12345";
//char pass2[5];
void open_edit_menu(void) __irq;
void Init_system()
{
	//initialization of lcd,keypad,RTC
	Init_LCD();
	Init_kpm();
	Init_RTC();
	//set time ,date and day
	SetRTCTimeInfo(12,14,00);
	SetRTCDateInfo(30,10,2025);
	SetRTCDay(4);
	 cfgportpinfunc(0, 1, EINT0_PIN_0_1);
  VICIntEnable = 1 << EINT0_VIC_CHNO ;
  VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
  VICVectAddr0 = (u32)open_edit_menu;
  EXTMODE = 1 << 0;
}
	

void display_RTC()
{
	GetRTCTimeInfo(&hour, &min,&sec);
	DisplayRTCTime(hour,min,sec);
	GetRTCDateInfo(&date,&month,&year);
	DisplayRTCDate(date,month,year);
	GetRTCDay(&day);
	DisplayRTCDay(day);
}
int password()
{
	s32 i;
	s8 key;
//	s8 str[4];
	IODIR0 |=1<<led;
	CmdLCD(0x01);
  user_password = 0;
  StrLCD("enter password");
  CmdLCD(0xc0);
	//delay_ms(100);
  for (i = 0; i <= 3; i++) {
    key = KeyScan();
		if(key == 'c')
		{
			if(i!=0)
			i--;
			CmdLCD(0XC0+i);
			CharLCD(' ');
		  CmdLCD(0XC0+i);
			user_password /=10;
			if(i!=0)
			i--;
		}
    else if (('0' <= key) && (key <= '9')) {
			CharLCD('*');
       user_password = (user_password * 10) + (key - 48);
    }	
		else 
		 i--;
    
  }

  if(default_password != user_password) {
    CmdLCD(0x01);
    StrLCD("INVALID PASSWORD");
    CmdLCD(0xc0);
    StrLCD("try agsin");
		
  delay_ms(500);
    // if(KeyScan() == 'e')
   return 0;
	}
	
	if(default_password == user_password)
	{
		CmdLCD(0x01);
		StrLCD("valid password");
		delay_ms(500);
		return 1;
	}
	return 0;
}
void change_time()
{
	
	int i,hour=0,min=0,sec=0;
	char key;
		BuildCGRAM(forword_arrow,24);
	CmdLCD(0x01);
   CmdLCD(GOTO_LINE1_POS0+2);
	StrLCD("HH");
	 CmdLCD(GOTO_LINE1_POS0+7);
	StrLCD("MM");
	 CmdLCD(GOTO_LINE1_POS0+13);
	StrLCD("SS");
	hour_again:CmdLCD(0xc0);
	CharLCD(0);
	hour=0;
	for(i=1;i<=2;i++)
	{
		CmdLCD(0xc0+1+i);
	   key=KeyScan();
		CharLCD(key);
		hour=(hour*10)+(key-48);
	}
	delay_ms(100);
	if(!((0<=hour)&&(hour<=23)))
	{
	   CmdLCD(0xc3);
		 CharLCD(' ');
		 CmdLCD(0xc2);
		 CharLCD(' ');
		 goto hour_again;
	}
	min_again:CmdLCD(0xc6);
	CharLCD(0);
	min=0;
	for(i=1;i<=2;i++)
	{
		CmdLCD(0xc0+6+i);
	   key=KeyScan();
		CharLCD(key);
		min=(min*10)+(key-48);
	}
	delay_ms(100);
	if(!((0<=min)&&(min<=59)))
	{
	   CmdLCD(0xc7);
		 CharLCD(' ');
		 CmdLCD(0xc8);
		 CharLCD(' ');
		 goto min_again;
	}
	
	sec_again:CmdLCD(0xc0+11);
	CharLCD(0);
	sec=0;
	for(i=1;i<=2;i++)
	{
		CmdLCD(0xc0+12+i);
	   key=KeyScan();
		CharLCD(key);
		sec=(sec*10)+(key-48);
	}
	delay_ms(100);
	if(!((0<=sec)&&(sec<=59)))
	{
	   CmdLCD(0xc0+13);
		 CharLCD(' ');
		 CmdLCD(0xc0+14);
		 CharLCD(' ');
		 goto sec_again;
	}
	delay_ms(500);
     SetRTCTimeInfo(hour,min,sec);
	CmdLCD(0x01);
	StrLCD("TIME CHANGED");
	delay_ms(100);
	CmdLCD(0x01);
}
void change_password()
{
	 u32 i;
	 char key;
    u32 user_password2;
start1:
  CmdLCD(0x01);
  StrLCD("0ld password");
  user_password = 0;
  CmdLCD(0xc0);
  for (i = 0; i <= 3; i++) {
    key = KeyScan();
		if(key == 'c')
		{
			if(i!=0)
			i--;
			CmdLCD(0XC0+i);
			CharLCD(' ');
		  CmdLCD(0XC0+i);
			user_password /=10;
			if(i!=0)
			i--;
		}
   else if (('0' <= key) && (key <= '9')) {
      CharLCD('*');
      user_password = (user_password * 10) + (key - 48);
    } else {
      if (i != 0)
        i--;
    }
  }

  if (default_password == user_password) {
		start:    CmdLCD(0x01);
    StrLCD("new password");
    user_password = 0;
    CmdLCD(0xc0);
    for (i = 0; i <= 3; i++) {
      key = KeyScan();
			if(key == 'c')
		{
			if(i!=0)
			i--;
			CmdLCD(0XC0+i);
			CharLCD(' ');
		  CmdLCD(0XC0+i);
			user_password /=10;
			if(i!=0)
			i--;
		}
		else if (('0' <= key) && (key <= '9')) {
			
        CharLCD('*');
        user_password = (user_password * 10) + (key - 48);
      } else {
				if(i!= 0)
          i--;
      }
    }
    delay_ms(100);
    CmdLCD(0x01);
    StrLCD("conform password");
    user_password2 = 0;
    CmdLCD(0xc0);
    for (i = 0; i <= 3; i++) {
      key = KeyScan();
			if(key == 'c')
		{
			if(i!=0)
			i--;
			CmdLCD(0XC0+i);
			CharLCD(' ');
		  CmdLCD(0XC0+i);
			user_password /=10;
			if(i!=0)
			i--;
		}
    else if (('0' <= key) && (key <= '9')) {
        CharLCD('*');
        user_password2 = (user_password2 * 10) + (key - 48);
      } 
		else {
          i--;
      }
    }
    if (user_password2 != user_password) {
      CmdLCD(0x01);
      StrLCD("INVALID PASSWORD");
      CmdLCD(0xc0);
      StrLCD("try agsin");
      delay_ms(100);
      goto start;
    } else {

      CmdLCD(0x01);
      StrLCD("PASSWORD CHANGED");
      default_password = user_password;
			delay_ms(500);
			CmdLCD(0x01);
     }
  } else {
    CmdLCD(0x01);
    StrLCD("INVALID PASSWORD");
    CmdLCD(0xc0);
    StrLCD("try agsin");
    delay_ms(100);
    // if(KeyScan() == 'e')
    goto start1;
  }
}
void change_date()
{ 
	int i,date=0,month=0,year=0;
	char key;
		BuildCGRAM(forword_arrow,24);
	CmdLCD(0x01);
   CmdLCD(GOTO_LINE1_POS0+2);
	StrLCD("DD");
	 CmdLCD(GOTO_LINE1_POS0+7);
	StrLCD("MM");
	 CmdLCD(GOTO_LINE1_POS0+13);
	StrLCD("YYYY");
	date_again:	CmdLCD(0xc0);
	CharLCD(0);
	date=0;
	for(i=1;i<=2;i++)
	{
		CmdLCD(0xc0+1+i);
	   key=KeyScan();
   		CharLCD(key);
		date=(date*10)+(key-48);
	}
	delay_ms(100);
	if(!((1<=date)&&(date<=31)))
	{
	   CmdLCD(0xc3);
		 CharLCD(' ');
		 CmdLCD(0xc2);
		 CharLCD(' ');
		 goto date_again;
	}
	month_again:CmdLCD(0xc6);
	CharLCD(0);
	month=0;
	for(i=1;i<=2;i++)
	{
		CmdLCD(0xc0+6+i);
	   key=KeyScan();
		CharLCD(key);
		month=(month*10)+(key-48);
	}
	delay_ms(100);
	if(!((1<=month)&&(month<=12)))
	{
	   CmdLCD(0xc7);
		 CharLCD(' ');
		 CmdLCD(0xc8);
		 CharLCD(' ');
		 goto month_again;
	}
	CmdLCD(0xc0+11);
	CharLCD(0);
	for(i=1;i<=4;i++)
	{
		CmdLCD(0xc0+11+i);
	   key=KeyScan();
		CharLCD(key);
		year=(year*10)+(key-48);
	}
	delay_ms(500);
     SetRTCDateInfo(date, month,year);
	CmdLCD(0x01);
	StrLCD("DATE CHANGED");
  delay_ms(500);
	CmdLCD(0x01);
}
void change_day()
{
	char key;
  start: CmdLCD(0x01);
	StrLCD("0.SU 1.M 2.TU3.W");
	CmdLCD(0xc0);
	StrLCD("4.TH 5.F 6.SA");
	key=KeyScan();
	if(!(('0'<=key)&&(key<='6')))
		goto start;
	SetRTCDay((u32)(key-48));
	
	CmdLCD(0x01);
	StrLCD("DAY CHANGED");
	delay_ms(500);
	CmdLCD(0x01);
}
u32 change_login_time()
{
	  char  key;
	  time=0;
	start:CmdLCD(0xc0);
	      CharLCD(' ');
	       CmdLCD(0xc0);
	  key=KeyScan();
	  if(key=='0'||key=='1'||key=='2'){
		CharLCD(key);
			time = (key-48)*10;
		}
		else
		{
			goto start;
		}
		key=KeyScan();
	  if(('0'<=key)&&(key<='9'))
		{
		  CharLCD(key);
			time+=(key-48);
		}
		else if(key == 'c')
		{
		   goto start;
		}
		else
		{
	     goto	start;
		}
		start1:key = KeyScan();
		if(key == 'c')
		{
			CmdLCD(0xc1);
			CharLCD(' ');
			goto start;
		}
		else if(key == 'e')
		{
		   return time;
		}
		else 
		{
		  goto start1;
		}
		//return 0;
}
void open_edit_menu() __irq
{
	
   char key;
	CmdLCD(0x01);
	StrLCD("1.CP 2.CT 3.CDAY");
	CmdLCD(0xc0);
	StrLCD("4.CD 5.CLOGIN");
		key = KeyScan();
	switch(key)
	{
		case '1':change_password();
		         break;
	  case '2':change_time();
			     break;
		case '4':change_date();
		        break;
		case '3':change_day();
			       break;
		         
		case '5':CmdLCD(0x01);
		          StrLCD("ENTER LOGIN TIME");
			       login=change_login_time();
		         CmdLCD(0x01);
		         StrLCD("ENTER LOGOUTTIME");
		         logout=change_login_time();
		        CmdLCD(0x01);
	          StrLCD("TIME CHANGED");
	          delay_ms(500);
	          CmdLCD(0x01);
		        break;
		default:
						CmdLCD(0x01);
						display_RTC();
	}
	EXTINT = 1<<0;
	VICVectAddr =0;
}
u32 valid_entry_or_not()
{
      if((HOUR>=login) && (HOUR<=logout))
				return 1;
			return 0;
}
