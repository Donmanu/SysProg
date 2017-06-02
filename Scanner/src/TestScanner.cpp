#include "../includes/Scanner.h"
//#include "/usr/include/c++/4.7/ostream"
#include <iostream>
#include <fstream> // after iostream
#include <ctime>

int main(int argc, char **argv) {
	int args = 1;
	const char* tokenTypeToName[] = {
		"TokenUnknown\t\t", // a.k.a. TokenError		//0
		"TokenInteger\t\t",								//1
		"TokenIdentifier\t\t",							//2
		"TokenPlus\t\t",								//3
		"TokenMinus\t\t",								//4
		"TokenColon\t\t",								//5
		"TokenStar\t\t",								//6
		"TokenLessThan\t\t",							//7
		"TokenGreaterThan\t",							//8
		"TokenEquals\t\t",								//9
		"TokenColonEquals\t",							//10
		"TokenEqualsColonEquals\t",						//11
		"TokenExclamationMark\t",						//12
		"TokenAndAnd\t\t",								//13
		"TokenSemiColon\t\t",							//14
		"TokenParenthesisOpen\t",						//15
		"TokenParenthesisClose\t",						//16
		"TokenSquareBracketsOpen\t",					//17
		"TokenSquareBracketsClose", // 24 chars = 6 \t	//18
		"TokenCurlyBracesOpen\t",						//19
		"TokenCurlyBracesClose\t",						//20
		"TokenWrite\t\t",								//21
		"TokenRead\t\t",								//22
		"TokenIf\t\t\t",								//23
		"TokenWhile\t\t",								//24
		"TokenElse\t\t",								//25
		"TokenInt\t\t",									//26
		"TokenStop\t\t"									//27
	};

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

			scanner->debugPrint(); // TODO remove debug

			// Write file
			out << tokenTypeToName[t.type] << " in line " << t.line << "\tin column " << t.column;
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

		printf("Test ended. All tokens from %s fetched.\n", argv[args]);
		out <<  std::endl << "    --- END OF " << argv[args] << " ---" <<  std::endl <<  std::endl;
		delete scanner;

		time_t end = clock();
		printf("Took %f s\n", double(end - start) / CLOCKS_PER_SEC);
		std::ifstream file(argv[args], std::ifstream::ate | std::ifstream::binary);
		printf("Crunched with %f MB/s\n", file.tellg() * CLOCKS_PER_SEC / double(end - start) / 1e6 /*MB*/);
	}
	out.close();

	printf("--- TEST RUN FINISHED ---\n");
}

