/* textmode.c: interface with VGA text mode. */

/*
* VGA text mode is the default graphics mode that x86 CPUs launch into.
* Writing characters to the screen in VGA text mode is very easy. 
* VGA text mode will always have a "resolution" of 80x25 characters.
*
* The screen is stored as a buffer of two byte characters with the following encoding:
* 	byte 0: color byte
*		bit 7: depends (on some CPUs, a blink bit; on others, 4th bit of background color
*		bit 6,5,4: background color
*		bit 3,2,1,0: foreground color
*	byte 1: codepoint (actual character)
*		nb: generally ASCII but may vary
*
* The buffer is always located at 0xB8000.
*/

#include "textmode.h"

unsigned short *buffer;
unsigned short terminalX, terminalY;
unsigned short terminalForeground, terminalBackground;

/* Given foreground and background, encode a color. */
unsigned char encodeColor(unsigned char foreground, char background) {
	return background << 4 | foreground;
}

/* Given foreground, background, and codepoint, encode a character. */
unsigned short encodeCharacter(unsigned char foreground, unsigned char background, char character) {
	return (unsigned short)encodeColor(foreground, background) << 8 | (unsigned short)character;
}

/* Fill screen with spaces. */
void clearScreen() {
	for(unsigned short x = 0; x < 80; x++) {
		for(unsigned short y = 0; y < 25; y++) {
			buffer[y * 80 + x] = encodeCharacter(terminalForeground, terminalBackground, ' ');
		}
	}
}

/* Reset terminal. */
void resetTerminal() {
	
	buffer = (unsigned short *)0xB8000;
	
	/* Reset location of cursor. */
	terminalX = 0;
	terminalY = 0;
	
	/* Clear screen. */
	clearScreen();
	
	/* Reset black and white color scheme. */
	terminalForeground = 15;
	terminalBackground = 0x0;

}

void putChar(char character) {
	
	/* If the cursor is offscreen , simulate a newline. */
	if(terminalX > 80) {
		terminalX = 0;
		terminalY++;
	}
	
	/* Simulate carriage return and newline. */
	else if(character == '\r') {
		terminalX = 0;
	}
	
	/* Simulate newline, DOS style. */
	else if(character == '\n') {
		terminalX = 0;
		terminalY++;
	}
	
	/* Otherwise, draw character. */
	else {
		buffer[terminalY * 80 + terminalX] = encodeCharacter(terminalForeground, terminalBackground, character);
		terminalX++;
	}
	
	
}

unsigned int strlen(const char *string) {
	unsigned int length = 0;
	
	for(length = 0; string[length] != 0; length++);
	
	return length;
}

void printString(const char *string) {
	
	for(unsigned int index = 0; index < strlen(string); index++) {
		putChar(string[index]);	
	}
	
}

