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
	this->current_token = NULL;
}

Scanner::~Scanner() {
	// TODO Auto-generated destructor stub
	delete this->automat;
	delete this->buffer;
}

Token Scanner::nextToken() {
	// TODO:
	while (notoken) {
		this->automat->readChar(this->buffer->getChar()); // sets notoken and token
	}
	return this->current_token;
}

void Scanner::freeToken() {
	delete this->current_token;
}

void Scanner::mkToken(TokenType::Type type) {
	// Called by Automat to tell the Scanner something was found.
	this->notoken = false;
	this->freeToken();
	this->current_token = new Token;
	this->current_token.type = type;
	this->current_token.line = this->automat->line;
	this->current_token.column = this->automat->column; // TODO Setting line/column this way is probably too late!
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
	// ?? What to do here? Delete everything?
}
