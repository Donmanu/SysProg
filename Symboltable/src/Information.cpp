/*
 * Information.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/Information.h"

/*---------------------- Key --------------------------*/

Key::Key(unsigned int h, Information* info) {
	this->information = info;
	this->hash = h;
}

Key::~Key() {
	delete this->information;
}

Information* Key::getInformation() {
	return this->information;
}

void Key::setInformation(Information* information) {
	this->information = information;
}

unsigned int Key::getHash() {
	return this->hash;
}

void Key::setHash(unsigned int h) {
	this->hash = h;
}


/*------------------- Information -----------------------*/

Information::Information(StringTabNode* lexem) {
	this->lexemWrapper = lexem;
	this->occurrences = 1;
	this->type = DataType::noType;
}

Information::~Information() {
	delete this->lexemWrapper;
}

bool Information::compareLexem(const char* lexem) {
	if (lexem == NULL) {
		errno = EINVAL;
		perror("NULL given to compareLexem()");
		exit(EXIT_FAILURE);
	}
	return !strcmp(this->lexemWrapper->getLexem(), lexem);
}

const char* Information::getLexem(){
	return this->lexemWrapper->getLexem();
}

void Information::incrementOccurrences() {
	this->occurrences++;
}

/* Really only needed for keywords */
void Information::decrementOccurrences() {
	this->occurrences--;
}

int Information::getOccurrences() {
	return this->occurrences;
}

void Information::setType(DataType::Type type) {
	this->type = type;
}
DataType::Type Information::getType(void) {
	return this->type;
}
