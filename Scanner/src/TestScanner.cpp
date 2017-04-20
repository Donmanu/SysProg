#include "../includes/Scanner.h"

int main(int argc, char **argv) {

	Scanner* scanner;
	Token t;

	if (argc < 1) {
		printf("Please provide a file as first argument! (argc: %d)", argc);
		exit(EXIT_FAILURE);
	} else {
		printf("Scanning file %s\n", argv[1]);
	}

	/* TEST 1: scan the given file for tokens until TokenUnknown encountered */
	scanner = new Scanner(argv[1]);
	do {
		t = scanner->nextToken();
		printf("Found token type %i", t.type);
	} while (t.type != TokenType::TokenUnknown);

	delete(scanner);
}

