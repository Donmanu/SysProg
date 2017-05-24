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
	this->notoken = true;
	this->automat = new Automat(*this);
	this->buffer = new Buffer(filename);
	//this->current_token;
	this->symboltable = new Symboltable();
//	this->initSymbols();
}

Scanner::~Scanner() {
	// TODO Auto-generated destructor stub
	delete this->automat;
	delete this->buffer;
	delete this->symboltable;
}

/*
void Scanner::initSymbols() {
	this->keywords[0] = this->symboltable->insert("write");
	this->keywords[1] = this->symboltable->insert("read");
	this->keywords[2] = this->symboltable->insert("if");
	this->keywords[3] = this->symboltable->insert("IF");
	this->keywords[4] = this->symboltable->insert("else");
	this->keywords[5] = this->symboltable->insert("ELSE");
	this->keywords[6] = this->symboltable->insert("while");
	this->keywords[7] = this->symboltable->insert("WHILE");
	this->keywords[8] = this->symboltable->insert("int");
}
*/

Token Scanner::nextToken() {
	this->notoken = true;

	while (this->notoken) {
		this->automat->readChar(this->buffer->getChar()); // calls mkToken(), and therefore sets notoken = false;
	}
	return this->current_token;
}

void Scanner::freeToken() {
	//delete this->current_token; ??
}

void Scanner::mkToken(TokenType::Type type) {
	// Called by Automat (i.e. its States) to tell the Scanner when something was found.
	if (this->notoken == false) // already?
		perror("mkToken() called twice in a row!?");
	this->notoken = false;
	this->freeToken();

	if (type == TokenType::TokenIdentifier) {
		int position = 0;

//		for (int i = 0; i < KEYWORD_ARRAY_LENGTH; i++) {
//			if (this->keywords[i].getInformation()->compareLexem(this->automat->getFinalIdentifier())) {
//				position = i;
//			}
//		}

		switch (position) {
			case 0:
				type = TokenType::TokenWrite;
				break;
			case 1:
				type = TokenType::TokenRead;
				break;
			case 2:
			case 3:
				type = TokenType::TokenIf;
				break;
			case 4:
			case 5:
				type = TokenType::TokenElse;
				break;
			case 6:
			case 7:
				type = TokenType::TokenWhile;
				break;
			case 8:
				type = TokenType::TokenInt;
		}
	}

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
	}
	this->automat->ungetChar(count);
}

void Scanner::stop() {
	this->notoken = false; // next call to nextToken() will return:
	this->current_token.type = TokenType::TokenStop;
	this->current_token.line = this->automat->getLine();
	this->current_token.column = this->automat->getColumn();
}
