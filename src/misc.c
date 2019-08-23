unsigned int strlen(const char *string) {
	unsigned int length = 0;
	
	for(length = 0; string[length] != 0; length++);
	
	return length;
}

void memset(void *destination, unsigned char value, unsigned int size) {
	unsigned char *pointer = destination;

	for(unsigned int i = 0; i < size; i++) {
		pointer[i] = value;
	}
}

unsigned char inb(unsigned short port) {
	unsigned char value;
	__asm__ __volatile__ ("inb %1, %0" : "=a"(value) : "dN"(port));
	return value;
}

void outb(unsigned short port, unsigned char data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN"(port), "a"(data));
}

