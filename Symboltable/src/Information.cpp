/*
 * Information.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: donmanu
 */

#include "Information.h"

Key::Key() {
	this->information = NULL;
}

Key::~Key() {
	delete(this->information);
}

Information* Key::getInformation() {
	return this->information;
}

void Key::setInformation(Information* information) {
	this->information = information;
}



Information::Information(char* lexem, int value) {
	this->lexem = lexem;
	this->value = value;
}

Information::~Information() {
	delete(this->lexem);
}

bool Information::compareLexem(char* lexem) {
	if (strcmp(this->lexem, lexem) == 0) {
		return true;
	} else {
		return false;
	}
}

char* Information::getLexem(){
	return this->lexem;
}

int Information::getValue() {
	return this->value;
}

void Information::setLexem(char* lexem) {
	this->lexem = lexem;
}

void Information::setValue(int value) {
	this->value = value;
}
