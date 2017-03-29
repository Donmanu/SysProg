#include "../includes/Buffer.h"
//#include <iostream>

int main(int argc, char **argv) {
	Buffer* buffer;
	char* file;
	int args = 1;
	int i;
	char c;

	if (argc < 2 || *argv[1] == '\0') {
		printf("No test file(s) given!\n");
		return 1;
	}

	while (args < argc) {
		// Prep
		printf("----------------------------\n");
		printf("Testing %s\n", argv[args]);
		printf("----------------------------\n");
		file = argv[args++];

		// TEST 1: constructor
		printf(" --- Test 1: creating buffer\n");
		buffer = new Buffer(file);
		printf(" --- Test 1: Done\n");

		// TEST 2: getChar() until end
		printf(" --- Test 2: Printing all content\n");
		do {
			c = buffer->getChar();
			printf("%c (%d)\n", c, c);
		} while(c);
		printf(" --- Test 2: Done\n");

		// TEST 3: deleting and reinit
		printf(" --- Test 3: Deleting and recreating buffer\n");
		delete buffer;
		buffer = new Buffer(file);
		printf(" --- Test 3: Done\n");

		// TEST 4: getting 2, ungetting 1, until end
		printf(" --- Test 4: Alternating get and unget()\n");
		do {
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			buffer->ungetChar();
		} while(c);
		printf(" --- Test 4: Done\n");

		// TEST 5: getting after file end
		printf(" --- Test 5: Additional getChar() at file end\n");
		c = buffer->getChar();
		printf("%c (%d)\t", c, c);
		printf(" --- Test 5: Done\n");

		// TEST 6: getting after file end
		printf(" --- Test 6: Additional 100k getChar() after file end\n");
		for (i = 0; i < 100000; i++) {
			c = buffer->getChar();
			if (c) printf("%c (%d)\t", c, c);
		}
		printf(" --- Test 6: Done\n");

		// Reinit
		delete buffer;
		buffer = new Buffer(file);

		// TEST 7: ungetting after open
		printf(" --- Test 7: unget() after open\n");
		buffer->ungetChar();
		printf(" --- Test 7: Done\n");

		// TEST 8: reading to end, then ungetting all
		printf(" --- Test 8: Read all, then unget() all\n");
		i = 0;
		do {
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			i++;
		} while (c);
		for (; i > 0; i--) {
			buffer->ungetChar();
		}
		printf(" --- Test 8: Done\n");

		// TEST 9: ungetting after open
		printf(" --- Test 9: unget() after unget-all\n");
		buffer->ungetChar();
		printf(" --- Test 9: Done\n");

		// FINALLY
		delete buffer;
	}
}
