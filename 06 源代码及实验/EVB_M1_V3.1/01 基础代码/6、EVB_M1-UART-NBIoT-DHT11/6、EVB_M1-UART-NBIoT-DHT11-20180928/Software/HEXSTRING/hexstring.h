#ifndef __HEXSTRING_H
#define __HEXSTRING_H

#include "stm32l4xx_hal.h"



/**************************************************************/
/**************         hexstring�ӿ�        ******************/
/**************************************************************/
unsigned char *StringToByte(char *pString,unsigned char *pByte);
char *ByteToString(unsigned char *pByte,char *pString,unsigned int len);
char *DecToString(unsigned int Dec, char *pString);
#endif
