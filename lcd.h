#ifndef __LCD_H__
#define __LCD_H__

#include "types.h"

void Init_LCD(void);
void CmdLCD(u8 cmdByte);
void CharLCD(u8 asciival);
void WriteLCD(u8 byte);
void StrLCD(s8 *);
void u32LCD(u32);
void s32LCD(s32);
void f32LCD(f32 fn,u32 nDP);
void BuildCGRAM(u8 *,u8);

#endif
