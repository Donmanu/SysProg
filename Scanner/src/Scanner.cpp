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
	this->notoken = true;
	//this->current_token;
}

Scanner::~Scanner() {
	// TODO Auto-generated destructor stub
	delete this->automat;
	delete this->buffer;
}

void Scanner::sayHello() {
	printf("Hello!\n");
}

Token Scanner::nextToken() {
	char c;

	printf("Finding token:\n");

	while (this->notoken) {
		c = this->buffer->getChar();
		//printf("<%c>\n", c);
		this->automat->readChar(c); // calls mkToken(), and therefore sets notoken = false;
	}
	// TODO handle error(token)
	return this->current_token;
}

void Scanner::freeToken() {
	//delete this->current_token; ??
}

void Scanner::mkToken(TokenType::Type type) {
	// Called by Automat (i.e. its States) to tell the Scanner when something was found.
	printf("Token found! Type %i", type);
	this->notoken = false;
	this->freeToken();
	this->current_token.type = type;
	this->current_token.line = this->automat->getLine();
	this->current_token.column = this->automat->getColumn(); // TODO Setting line/column this way is probably too late!
}

void Scanner::ungetChar(int count) {
	if (count < 0) {
		errno = EINVAL; // invalid argument
		perror("Cannot unget() negative amount");
		return;
	}
	for (int i = 0; i < count; ++i) {
		this->buffer->ungetChar();
		//this->automat->??
	}
}

void Scanner::stop() {
	this->notoken = false; // next call to nextToken() will return:
	this->current_token.type = TokenType::TokenStop;
	this->current_token.line = this->automat->getLine();
	this->current_token.column = this->automat->getColumn();
}
