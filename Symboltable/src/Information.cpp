/*
 * Information.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "../includes/Information.h"

Key::Key(Information* information) {
	this->information = information;
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

Information::Information(char* lexem) {
	if (lexem != NULL) {
		this->lexem = new char[strlen(lexem) + 1];
		strcpy(this->lexem, lexem);
	} else {
		errno = EINVAL; // invalid arg
		perror("NULL given as lexem to Information()");
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

