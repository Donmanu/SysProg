#include "../includes/Scanner.h"

int main(int argc, char **argv) {

	Scanner* scanner;

	if (argc < 1) {
		printf("Please provide a file as first argument! (argc: %d)", argc);
		exit(EXIT_FAILURE);
	}
	scanner = new Scanner(argv[1]);

	delete(scanner);
}

