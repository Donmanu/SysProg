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

// initializing static const ...
const char *const Scanner::KEYWORDS[] = {"write", "read", "if", "IF", "else", "ELSE", "while", "WHILE", "int"};

Scanner::Scanner(char* filename) {
	this->notoken = true;
	this->automat = new Automat(*this);
	this->buffer = new Buffer(filename);
	this->symboltable = new Symboltable();
	this->keywords = new Key*[Scanner::KEYWORD_ARRAY_LENGTH];
	this->initSymbols();
}

Scanner::~Scanner() {
	delete this->automat;
	delete this->buffer;
	delete this->symboltable;
	delete[] this->keywords; // delete only array, keys get deleted with symboltable
}

void Scanner::initSymbols() {
	for (int i = 0; i < Scanner::KEYWORD_ARRAY_LENGTH; i++) {
		this->keywords[i] = this->symboltable->insert(Scanner::KEYWORDS[i]);
		this->keywords[i]->getInformation()->decrementOccurrences(); // revert default insert-action
	}
}

Token Scanner::nextToken() {
	this->notoken = true;

	while (this->notoken) {
		this->automat->readChar(this->buffer->getChar()); // calls mkToken(), and therefore sets notoken = false;
	}
	return this->current_token;
}

/*
 * Removes information pertaining only to the last token in case it has no meaning on the new one (value, lexem, ...)
 */
void Scanner::freeToken() {
	this->current_token.type = TokenType::TokenStop;
	this->current_token.line = -1337; // because some offset errors leading to -1, we set to some other magic number
	this->current_token.column = -1337;
	this->current_token.key = NULL;
	this->current_token.value = -1l; // integer tokens are always positive, so -1 is OK to indicate void value.
}

void Scanner::mkToken(TokenType::Type type) {
	// Called by Automat (i.e. its States) to tell the Scanner when something was found.
	if (this->notoken == false) {// already?
		errno = EBADMSG; // 74: Not a data message (what else??)
		printf("LAST: %d \t NEXT: %d\n", this->current_token.type, type);
		perror("mkToken() called twice in a row!?");
		exit(EXIT_FAILURE); // critical!
	}
	this->notoken = false;
	this->freeToken();
	this->filterToken(type);
	this->current_token.line = this->automat->getLine();
	this->current_token.column = this->automat->getColumn();
}

void inline Scanner::filterToken(TokenType::Type type) {
	int position;
	unsigned int hash;
	this->current_token.type = type;
	switch (type) {
	case TokenType::TokenUnknown:
		this->current_token.value = (long int) this->automat->getUnknownCharacter();
		break;

	case TokenType::TokenInteger:
		this->current_token.value = this->automat->getIntegerValue();
		break;

	case TokenType::TokenIdentifier:
		position = Scanner::KEYWORD_ARRAY_LENGTH;
		hash = this->symboltable->hash(this->automat->getLastString());
		// search keywords for match
		for (int i = 0; i < Scanner::KEYWORD_ARRAY_LENGTH; i++) {
			if (this->keywords[i]->getHash() == hash) { // only fully compare, if hash is equal
				if (this->keywords[i]->getInformation()->compareLexem(this->automat->getLastString())) {
					position = i;
					break;
				}
			}
		}
		switch (position) {
			case 0:
				this->current_token.type = TokenType::TokenWrite;
				this->current_token.key = this->keywords[0];
				this->keywords[0]->getInformation()->incrementOccurrences();
				break;
			case 1:
				this->current_token.type = TokenType::TokenRead;
				this->current_token.key = this->keywords[1];
				this->keywords[1]->getInformation()->incrementOccurrences();
				break;
			case 2:
				this->current_token.type = TokenType::TokenIf;
				this->current_token.key = this->keywords[2];
				this->keywords[2]->getInformation()->incrementOccurrences();
				break;
			case 3:
				this->current_token.type = TokenType::TokenIf;
				this->current_token.key = this->keywords[3];
				this->keywords[3]->getInformation()->incrementOccurrences();
				break;
			case 4:
				this->current_token.type = TokenType::TokenElse;
				this->current_token.key = this->keywords[4];
				this->keywords[4]->getInformation()->incrementOccurrences();
				break;
			case 5:
				this->current_token.type = TokenType::TokenElse;
				this->current_token.key = this->keywords[5];
				this->keywords[5]->getInformation()->incrementOccurrences();
				break;
			case 6:
				this->current_token.type = TokenType::TokenWhile;
				this->current_token.key = this->keywords[6];
				this->keywords[6]->getInformation()->incrementOccurrences();
				break;
			case 7:
				this->current_token.type = TokenType::TokenWhile;
				this->current_token.key = this->keywords[7];
				this->keywords[7]->getInformation()->incrementOccurrences();
				break;
			case 8:
				this->current_token.type = TokenType::TokenInt;
				this->current_token.key = this->keywords[8];
				this->keywords[8]->getInformation()->incrementOccurrences();
				break;
			default: // on position
				// keep            type = TokenType::TokenIdentifier
				this->current_token.key = this->symboltable->insert(this->automat->getLastString());
		}
		break;
	default: // on type
		// nothing
		break;
	}
}

void Scanner::ungetChar(int count) {
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

void Scanner::debugPrint() {
	this->symboltable->debugPrint();
}
