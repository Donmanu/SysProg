#include "../includes/Scanner.h"
//#include "/usr/include/c++/4.7/ostream"
#include <iostream>
#include <fstream> // after iostream
#include <ctime>

int main(int argc, char **argv) {
	int args = 1;

	Scanner* scanner;
	Token t;
	std::ofstream out;

	if (argc < 2) {
		printf("Please provide files to read and write as arguments! (argc: %d)", argc);
		exit(EXIT_FAILURE);
	} else {
		out.open("out.txt"/*, ios::out*/);
	}

	for (;args < argc; args++) {
		/* TEST 1: scan the given file for tokens until scanner stopped by automat */
		printf("-----------------------------------------------------\n");
		printf("Testing file %s\n", argv[args]);

		// would be cool to open a new file for each test.in, but this fucking const char* stuff sucks soo hard ...

		time_t start = clock();
		scanner = new Scanner(argv[args]); // need to make new scanner for every file
		do {
			t = scanner->nextToken();

			//scanner->debugPrint();

			// Write file
			out << TokenType::tokenNameAligned[t.type] << " in line " << t.line << "\tin column " << t.column;
			switch (t.type) {
			case TokenType::TokenUnknown:
				out << "\tSymbol: " << (char)t.value << std::endl;
				break;
			case TokenType::TokenIdentifier:
				out << "\tLexem: " << t.key->getInformation()->getLexem() << std::endl;
				break;
			case TokenType::TokenInteger:
				out << "\tValue: " << t.value << std::endl;
				break;
			default:
				out << std::endl;
			}

		} while (t.type != TokenType::TokenStop);

		// Add-on:
		//t = scanner->nextToken(); out << TokenType::tokenNameAligned[t.type] << " in line " << t.line << "\tin column " << t.column << std::endl;
		//t = scanner->nextToken(); out << TokenType::tokenNameAligned[t.type] << " in line " << t.line << "\tin column " << t.column << std::endl;
		//t = scanner->nextToken(); out << TokenType::tokenNameAligned[t.type] << " in line " << t.line << "\tin column " << t.column << std::endl;

		printf("Test ended. All tokens from %s fetched.\n", argv[args]);
		out <<  std::endl << "    --- END OF " << argv[args] << " ---" <<  std::endl <<  std::endl;

		time_t end = clock();
		printf("Took %f s\n", double(end - start) / CLOCKS_PER_SEC);
		std::ifstream file(argv[args], std::ifstream::ate | std::ifstream::binary);
		printf("Crunched with %f MB/s\n", file.tellg() * CLOCKS_PER_SEC / double(end - start) / 1e6 /*MB*/);

		//scanner->debugPrint();
		delete scanner;
	}
	out.close();

	printf("--- TEST RUN FINISHED ---\n");
}

