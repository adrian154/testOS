#ifndef __MISC_H
#define __MISC_H

extern void hang();
extern unsigned int strlen(const char *string);
extern void memset(void *destination, unsigned char value, unsigned int size);
extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);

#endif