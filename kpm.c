#include "types.h"
#include "kpm_defines.h"
#include <lpc21xx.h>
#include "lcd.h"
u8 KpmLUT[4][4]={{'7','8','9','s'},{'4','5','6','+'},{'1','2','3','-'},
								{'c','0','=','e'}};
void Init_kpm(void)
{
        //rows output &grounded
        IODIR1 |=15<< ROW0;
        //cols input &high anyway by default
}
u32 ColScan(void)
{
        u32 status;
        status=(((IOPIN1>>COL0)&15)<15)?0:1;
        return status;
}
u32 RowCheck(void)
{
        u32 r;
        for(r=0;r<=3;r++)
        {
                IOPIN1 = ((IOPIN1&~(15<<ROW0))|(~(1<<r)<<ROW0));
                //check if then key was pressed in that row
                if(ColScan()==0)
                {
                        break;
                }
        }
        IOCLR1 = 15<<ROW0;
        return r;
}
u32 ColCheck(void)
{
        u32 c;
        for(c=0;c<=3;c++)
        {
                //starting with col0 upto col3
                //check on col at a time for low
                if(((IOPIN1>>(COL0+c))&1)==0)
                {
                break;
                }
        }
        return c;
}
u32 KeyScan(void)
{
        u32 r,c,KeyV;
        //initialize kpm
        //Init_kpm();

        //wait for any key press detection
        //while(ColScan()); //upon key press detection
				//if(ColScan() == 1)     // no key pressed
        //return 0;
					while(ColScan());  
        //identify row in which key was pressed
        r= RowCheck();
        //identify col in which key was pressed
        c=ColCheck();
			
        //extract key value from LUT
        KeyV=KpmLUT[r][c];
					while(!(ColScan()));
        return KeyV;

}
void ReadNum(u32 *num,u32 *lastkey)
{
	u32 key;
	*num=0;
	while(1)
	{
		key=KeyScan();
		if(key>='0' && key<='9')
		{
			*num =(* num*10)+(key-'0');
			while(ColScan()==0);
		}
		else
		{
			*lastkey=key;
			while(ColScan()==0);
			break;
		}
	}

}

















