/*
 * ParserTest.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h"
#include <iostream>
#include <fstream>

void parseArgs(int, char**);
void printHelp(void);
void printVersion(void);

int main(int argc, char **argv) {
	//TODO: parse arguments
	//printHelp();
	//printVersion();

	for (int i = 1; i < argc; i++) {
		printf("  --- Parsing file %s ---\n", argv[i]);
		Parser* parser = new Parser(argv[i]);

		parser->parse();

		printf("  --- Successfully parsed file %s ---\n\n", argv[i]);

//		parser->checkType();

		printf("  --- Successfully type-checked file %s ---\n\n", argv[i]);

		parser->debugPrint();

		delete parser;
	}
}

void parseArgs(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '-':
			if (argv[i][1] == '-') {
				// have long option
			}
			break;
		case '\0':

			break;
		default:

			break;
		}
	}

	if (errno) {
		printHelp();
		exit(EXIT_FAILURE);
	}
}

void printHelp(void) {
	const char* help =
			"Options could not be parsed.\n"
			"USAGE: parser [[-i] <inputFile>] [-c <codeOutputFile>]\n"
			"       [-t <tokenizerOutputFile>] [-d <debugOutputFile>]\n"
			"       [-e <errorOutputFile>] [-h] [-v]\n"
			"  The only necessary option is a valid (path to an) input file.\n"
			"\n"
		    "  The other options are:\n"
		    "    -i | --input   Which file to parse.\n"
		    "    -c | --code    Write the generated code into the given file.\n"
		    "    -t | --token   Write the output of the Tokenizer. If a file is given, it's\n"
			"                      written there. Else it's written in the console on stdOut.\n"
		    "    -d | --debug   Write additional information. If a file is given, it's\n"
			"                      written there. Else it's written in the console on stdOut.\n"
		    "    -e | --error   If a file is given, error output is written there instead of\n"
			"                      the default stdErr on the console.\n"
		    "    -v | --version Print information about authors and build settings.\n"
		    "    -h | --help    Print this help.\n"
		    "\n"
		    "  Example:\n"
		    "    parser subfolder/input.txt -h -c output.code -e errors.txt\n"
		    "\n"
		    "  Default behavior:\n"
		    "    Takes an input file and compiles its content. Prints it on stdOut.\n";
	printf(help);
}

void printVersion(void) {
	const char* vers =
			"Version information of 'parser':\n"
			"  Authors: Manuel Giesinger and Arthur Jagiella\n"
			"\n"
		    "  Setup:\n"
		    "    Buffer size: %d\n"
		    "    Assumed tabulator width: %d\n"
		    "    \n" // TODO SymTab: initSize, hash params  Parser: ?
		    "    ";
	printf(vers, 512 /*Buffer::BUFFER_SIZE is private*/, TAB_WIDTH);
}
