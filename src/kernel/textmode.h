#ifndef __TEXTMODE_H
#define __TEXTMODE_H

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define BRIGHT_BLUE 9
#define BRIGHT_GREEN 10
#define	BRIGHT_CYAN 11
#define BRIGHT_RED 12
#define BRIGHT_MAGENTA 13
#define BRIGHT_YELLOW 14
#define WHITE 15

#include "types.h"

extern unsigned short terminalX, terminalY;
extern unsigned char terminalForeground, terminalBackground;
extern void resetTerminal();
extern void clearScreen();
extern void putChar(char character);
extern void printString(const char *string);
extern unsigned int strlen(const char *string);
extern void printByte(uint8 byte);
extern void printWord(uint16 word);
extern void printDword(uint32 dword);
extern void printQword(uint64 qword);
extern void moveCursor(unsigned int x, unsigned int y);

#endif