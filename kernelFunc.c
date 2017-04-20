void kernelFunc(void) {

	const char* str = "My first kernel! Hello, World! :)";
	short* videoPtr = (short*)0xb8000;

	for (int i = 0; i < 4000; ++i) {

		videoPtr[i] = 0x0700 | ' ';

	}

	unsigned int i = 0;
	while (str[i] != '\0') {

		videoPtr[i] = 0x0700 | str[i];
		++i;

	}

}

