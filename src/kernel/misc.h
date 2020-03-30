#ifndef __MISC_H
#define __MISC_H

#include "types.h"
#include "textmode.h"

/* Debug macros */
#define DEBUG(message) (debugMessage(__func__, message, WHITE))
#define WARNING(message) (debugMessage(__func__, message, BRIGHT_YELLOW))
#define ERROR(message) (debugMessage(__func__, message, BRIGHT_RED))

extern void hang();
extern unsigned int strlen(const char *string);
extern void memset(void *destination, unsigned char value, unsigned int size);
extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);
extern void debugMessage(const char *function, const char *message, uint8 color);

#endif