/*
 * ParserTest.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h"
#include <iostream>
#include <fstream>

void printHelp(void);

int main(int argc, char **argv) {
	if (argc < 2) {
		printHelp();
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		Parser* parser = new Parser(argv[i]);

		printf("  --- Parsing file %s ---\n", argv[i]);
		parser->parse();
		parser->debugPrint();
		printf("  --- Successfully parsed file %s ---\n\n", argv[i]);

		printf("  --- TypeChecking file %s ---\n", argv[i]);
		parser->checkType();
		parser->debugPrint();
		printf("  --- Successfully type-checked file %s ---\n\n", argv[i]);

		printf("  --- Making code for file %s ---\n", argv[i]);
		parser->makeCode(argv[i]);
		printf("  --- Successfully made code for file %s ---\n\n", argv[i]);

		delete parser;
	}
}

void printHelp(void) {
	const char* help =
			"Options could not be parsed. Please provide at least 1 valid input file name.\n"
			"USAGE: parser <inputFile> [<inputFile> ...]\n";
	printf(help);
}
