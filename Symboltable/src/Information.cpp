/*
 * Information.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/Information.h"

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

Information::Information(char* lexem) {
	this->occurrences = 0; // Caveat: when Info is created, it could also be a keyword without occurrences (yet)!
	if (lexem != NULL) {
		this->lexem = new char[strlen(lexem) + 1];
		strcpy(this->lexem, lexem);
	} else {
		errno = EINVAL; // invalid arg
		perror("NULL given as lexem to Information()! Doing fallback ...");
		this->lexem = new char[1];
		this->lexem[0] = '\0';
	}
}

Information::~Information() {
	delete[] this->lexem;
}

bool Information::compareLexem(char* lexem) {
	if (lexem == NULL) {
		errno = EINVAL;
		perror("NULL given to compareLexem()");
		return false;
	}
	return !strcmp(this->lexem, lexem);
}

char* Information::getLexem(){
	return this->lexem;
}

void Information::incrementOccurrences() {
	this->occurrences++;
}

/*
 * Really only needed for keywords
 */
void Information::decrementOccurrences() {
	this->occurrences--;
}

int Information::getOccurrences() {
	return this->occurrences;
}

