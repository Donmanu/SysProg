#include "../includes/Scanner.h"
//#include "/usr/include/c++/4.7/ostream"
#include <iostream>
#include <fstream> // after iostream

int main(int argc, char **argv) {

	int args = 1;
	const char* tokenTypeToName[] = {
		"TokenUnknown\t\t", // a.k.a. TokenError
		"TokenInteger\t\t",
		"TokenIdentifier\t\t",
		"TokenPlus\t\t",
		"TokenMinus\t\t",
		"TokenColon\t\t",
		"TokenStar\t\t",
		"TokenLessThan\t\t",
		"TokenGreaterThan\t",
		"TokenEquals\t\t",
		"TokenColonEquals\t",
		"TokenEqualsColonEquals\t",
		"TokenExclamationMark\t",
		"TokenAndAnd\t\t",
		"TokenSemiColon\t\t",
		"TokenParenthesisOpen\t",
		"TokenParenthesisClose\t",
		"TokenSquareBracketsOpen\t",
		"TokenSquareBracketsClose", // 24 chars = 6 \t
		"TokenCurlyBracesOpen\t",
		"TokenCurlyBracesClose\t",
		"TokenIf\t\t\t",
		"TokenWhile\t\t",
		"TokenStop\t\t"
	};

	Scanner* scanner;
	Token t;
	std::ofstream out;

	if (argc < 2) {
		printf("Please provide files to read and write as arguments! (argc: %d)", argc);
		exit(EXIT_FAILURE);
	} else {
		out.open("out.txt"/*, ios::out*/);
		printf("Scanning file ... %s\n", argv[1]);
	}

	for (;args < argc; args++) {
		/* TEST 1: scan the given file for tokens until scanner stopped by automat */
		printf("\n-----------------------------------------------------\n");
		printf("Testing file %s\n", argv[args]);
		scanner = new Scanner(argv[args]); // need to make new scanner for every file
		do {
			t = scanner->nextToken();

			// Write file
			out << tokenTypeToName[t.type] << " in line " << t.line << "\tin column " << t.column;
			switch (t.type) {
			case TokenType::TokenUnknown:
				out << "\tSymbol: TODO" << std::endl;
				break;
			case TokenType::TokenIdentifier:
				out << "\tLexem: TODO" <<  std::endl;
				break;
			case TokenType::TokenInteger:
				out << "\tValue: TODO" <<  std::endl;
				break;
			default:
				out << std::endl;
			}

		} while (t.type != TokenType::TokenStop);

		printf("Test ended. All tokens from %s fetched.\n", argv[args]);
		out <<  std::endl << "    --- END OF " << argv[args] << " ---" <<  std::endl <<  std::endl;
		delete(scanner);
	}
	out.close();

	printf("_done_\n");
}

