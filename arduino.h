#include "types.h"

void pinMode(u32 pinNo,u32 pinDir);
void digitalWrite(u32 pinNo,u32 bit);
u32 digitalRead(u32 pinNo);
void portMode(u32 pinStartNo,u32 npins,u32 pinsDir);
void Write2pins(u32 pinStartNo,u32 npins,u32 data);
u32 readPins(u32 pinStartNo,u32 npins,u32 pinsDir);
