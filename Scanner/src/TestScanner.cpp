#include "../includes/Scanner.h"
//#include "/usr/include/c++/4.7/ostream"
#include <iostream>
#include <fstream> // after iostream

int main(int argc, char **argv) {

	const char* tokenTypeToName[] = {
		"TokenUnknown\t\t", // a.k.a. TokenError
		"TokenInteger\t\t",
		"TokenIdentifier\t\t",
		"TokenPlus\t\t\t",
		"TokenMinus\t\t\t",
		"TokenColon\t\t\t",
		"TokenStar\t\t",
		"TokenLessThan\t\t",
		"TokenGreaterThan\t",
		"TokenEquals\t\t",
		"TokenColonEquals\t",
		"TokenEqualsColonEquals\t",
		"TokenExclamationMark\t",
		"TokenAndAnd\t\t",
		"TokenSemiColon\t\t",
		"TokenBracketsOpen\t",
		"TokenBracketsClose\t",
		"TokenSquareBracketsOpen\t",
		"TokenSquareBracketsClose", // 24 chars = 6 \t
		"TokenCurlyBracketsOpen\t",
		"TokenCurlyBracketsClose\t",
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
		out.open(argv[2]/*, ios::out*/);
		printf("Scanning file ... %s\n", argv[1]);
	}

	/* TEST 1: scan the given file for tokens until scanner stopped by automat */
	scanner = new Scanner(argv[1]);
	do {
		t = scanner->nextToken();

		// Write file
		out << tokenTypeToName[t.type] << " in line " << t.line << "\tin column " << t.column;
		switch (t.type) {
		case TokenType::TokenUnknown:
			out << "\twith symbol TODO get symbol of tokenUnknown" << std::endl;
			break;
		default:
			out << std::endl;
		}

	} while (t.type != TokenType::TokenStop);

	printf("Test ended. All tokens fetched.\n");
	delete(scanner);
	out.close();

	printf("_done_");
}

