/*
 * ParserTest.cpp
 *
 *  Created on: May 18, 2017
 *      Author: donmanu
 */

#include "../includes/Parser.h"
#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
	//TODO: parse arguments

	Parser parser = new Parser(argv[1]);
	parser.prog(); // TODO ?
}
