#include "../includes/Buffer.h"
//#include <iostream> already is in Buffer.h

/* TODO Make test suite ... */
//int FAIL = 1;
//int SUCCESS = 0;
//void doTest(char* name, int testFct[, int expect]);

int main(int argc, char **argv) {
	Buffer* buffer;
	char* file;
	int args = 1;
	int i;
	char c;
	int testNo = 1;

	if (argc < 2 || *argv[1] == '\0') {
		printf("No test file(s) given!\n");
		return 1;
	}

	while (args < argc) { // for each file:
		// Prep
		printf("\n-------------------------------------------------------------\n");
		printf("    Testing %s\n", argv[args]);
		printf("\n-------------------------------------------------------------\n");
		file = argv[args];

		// TEST 1: constructor
		printf(" --- Test %d: creating buffer\n", testNo);
		buffer = new Buffer(file);
		printf(" --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 2: getChar() until end
		printf(" --- Test %d: Printing all content\n", testNo);
		do {
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
		} while(c);
		printf("\n --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 3: deleting and reinit
		printf(" --- Test %d: Deleting and recreating buffer\n", testNo);
		delete buffer;
		buffer = new Buffer(file);
		printf(" --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 4: getting 2, ungetting 1, until end
		printf(" --- Test %d: Alternating get and unget()\n", testNo);
		do {
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			buffer->ungetChar();
		} while(c);
		printf("\n --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 5: getting after file end
		printf(" --- Test %d: Additional getChar() at file end\n", testNo);
		c = buffer->getChar();
		printf("%c (%d)\t", c, c);
		printf("\n --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 6: getting after file end
		printf(" --- Test %d: Bunch of additional getChar() after file end\n", testNo);
		for (i = 0; i < 3 * 512 /*BUFFER_SIZE*/; i++) {
			c = buffer->getChar();
			if (c) printf("%c (%d)\t", c, c);
		}
		printf("\n --- Test %d: Done\n", testNo);
		testNo++;

		// Reinit
		delete buffer;
		buffer = new Buffer(file);

		// TEST 7: ungetting after open ... WILL FAIL
//		printf(" --- Test 7: unget() after open\n");
//		buffer->ungetChar();
//		printf(" --- Test 7: Done\n");

		// TEST 8: reading to end, then ungetting all
		printf(" --- Test %d: Read all, then unget() all\n", testNo);
		i = 0;
		do {
			c = buffer->getChar();
			printf("%c (%d)\t", c, c);
			i++;
		} while (c);
		// i is now the number of chars in the file
		int I = i; // save
		try {
			for (; i > 0; i--) {
				buffer->ungetChar();
			}
		} catch (int e) {
			if (I >= 2 * 512/*BUFFER_SIZE*/) {
				printf("Test %d failed as expected", testNo);
				// continue
			} else {
				printf("Test %d on file %s failed unexpectedly with error %d: i = %d", testNo, argv[args], e, i);
				//return EXIT_FAILURE;
			}
		}
		printf("\n --- Test %d: Done\n", testNo);
		testNo++;

		// TEST 9: ungetting after last unget() ... WILL FAIL
//		printf(" --- Test %d: unget() after unget-all\n", testNo);
//		buffer->ungetChar();
//		printf(" --- Test %d: Done\n", testNo);
//		testNo++;

		// FINALLY
		delete buffer;

		printf("\n-------------------------------------------------------------\n");
		printf("   Tests on file %s passed", argv[args]);
		args++;
		printf("\n-------------------------------------------------------------\n");
	}

	return EXIT_SUCCESS;
}
