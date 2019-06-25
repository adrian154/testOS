void cmain() {                    

	unsigned short *ptr = (unsigned short *)0x8B000;
	*ptr = 0x0756;                

	while(1);
} 
