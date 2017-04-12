/*
 * Scanner.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Scanner.h"
#include "../../Automat/includes/Automat.h"
#include "../../Buffer/includes/Buffer.h"
#include "../../Symboltable/includes/Symboltable.h"

Scanner::Scanner(char* filename) {
	this->automat = new Automat(this);
	this->buffer = new Buffer(filename);
	this->symTable = new Symboltable();
}

Scanner::~Scanner() {
	// TODO Auto-generated destructor stub
	delete this->automat;
	delete this->buffer;
	delete this->symTable;
}

void Scanner::nextToken() {

}

void Scanner::freeToken() {

}

void Scanner::mkToken(TokenType::Type token_type) {

}

void Scanner::ungetChar(int count) {

}

void Scanner::stop() {

}
