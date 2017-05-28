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
	this->keywords = new Key*[KEYWORD_ARRAY_LENGTH];
	this->initSymbols();
}

Scanner::~Scanner() {
	delete this->automat;
	delete this->buffer;
	delete this->symboltable;
	delete[] this->keywords;
}

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

Token Scanner::nextToken() {
	this->notoken = true;

	while (this->notoken) {
		this->automat->readChar(this->buffer->getChar()); // calls mkToken(), and therefore sets notoken = false;
	}
	return this->current_token;
}

void Scanner::freeToken() {
	// more like resetToken();
	// a.k.a. remove information pertaining only to the last token (value, lexem, ...)
	this->current_token.type = TokenType::TokenStop;
	this->current_token.line = -1337; // because some offset errors leading to -1, we set to some other magic number
	this->current_token.column = -1337;
	this->current_token.key = NULL;
	this->current_token.value = -1; // ??
}

void Scanner::mkToken(TokenType::Type type) {
	// Called by Automat (i.e. its States) to tell the Scanner when something was found.
	if (this->notoken == false) // already?
		perror("mkToken() called twice in a row!?");
	this->notoken = false;
	this->freeToken();

	if (type == TokenType::TokenUnknown) {
		this->current_token.value = (int)this->automat->getUnknownCharacter();
	}

	if (type == TokenType::TokenInteger) {
		this->current_token.value = this->automat->getIntegerValue();
	}

	if (type == TokenType::TokenIdentifier) {
		int position = KEYWORD_ARRAY_LENGTH;

		for (int i = 0; i < KEYWORD_ARRAY_LENGTH; i++) {
			if (this->keywords[i]->getInformation()->compareLexem(this->automat->getLastString())) {
				position = i;
				break;
			}
		}

		switch (position) {
			case 0:
				type = TokenType::TokenWrite;
				this->keywords[0]->getInformation()->incrementOccurrences();
				break;
			case 1:
				type = TokenType::TokenRead;
				this->keywords[1]->getInformation()->incrementOccurrences();
				break;
			case 2:
				type = TokenType::TokenIf;
				this->keywords[2]->getInformation()->incrementOccurrences();
				break;
			case 3:
				type = TokenType::TokenIf;
				this->keywords[3]->getInformation()->incrementOccurrences();
				break;
			case 4:
				type = TokenType::TokenIf;
				this->keywords[4]->getInformation()->incrementOccurrences();
				break;
			case 5:
				type = TokenType::TokenElse;
				this->keywords[5]->getInformation()->incrementOccurrences();
				break;
			case 6:
				type = TokenType::TokenIf;
				this->keywords[6]->getInformation()->incrementOccurrences();
				break;
			case 7:
				type = TokenType::TokenWhile;
				this->keywords[7]->getInformation()->incrementOccurrences();
				break;
			case 8:
				type = TokenType::TokenInt;
				this->keywords[8]->getInformation()->incrementOccurrences();
				break;
			default:
				// keep type = TokenType::TokenIdentifier
				this->current_token.key = this->symboltable->insert(this->automat->getLastString());
		}
	}

	this->current_token.type = type;
	this->current_token.line = this->automat->getLine();
	this->current_token.column = this->automat->getColumn();
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
	this->symboltable->debugPrint();
}
