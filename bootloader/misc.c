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
