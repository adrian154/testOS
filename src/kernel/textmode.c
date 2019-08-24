/* textmode.c: interface with VGA text mode. */

#include "textmode.h"
#include "misc.h"

unsigned short *buffer;
unsigned short terminalX, terminalY;
unsigned char terminalForeground, terminalBackground;

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

/* Move cursor */
void moveCursor(unsigned int x, unsigned int y) {
	unsigned short position = y * 80 + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char) (position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
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
	terminalForeground = WHITE;
	terminalBackground = BLACK;

}

void putChar(char character) {
	
	/* If the cursor is offscreen , simulate a newline. */
	if(terminalX == 80) {
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
	
	/* Simulate scroll. */	
	if(terminalY == 25) {
		for(unsigned short y = 1; y < 25; y++) {
			for(unsigned short x = 0; x < 80; x++) {
				buffer[(y - 1) * 80 + x] = buffer[y * 80 + x];
			}
		}
		for(unsigned short x = 0; x < 80; x++) {
			buffer[24 * 80 + x] = encodeCharacter(terminalForeground, terminalBackground, ' ');	
		}
		terminalY = 24;
	}
	
	moveCursor(terminalX, terminalY);
	
}

void printString(const char *string) {
	
	for(unsigned int index = 0; index < strlen(string); index++) {
		putChar(string[index]);	
	}
	
}

void printByte(unsigned char byte) {
	unsigned char high = byte >> 4;
	unsigned char low = byte & 0x0F;
	putChar(high > 9 ? high - 0xA + 'A' : high + '0');
	putChar(low > 9 ? low - 0xA + 'A' : low + '0');
}

void printWord(unsigned short word) {
	printByte(word >> 8);
	printByte(word & 0xFF);
}

void printDword(unsigned int dword) {
	printWord(dword >> 16);
	printWord(dword & 0xFFFF);
}

