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



Information::Information(char* lexem, int* value) {
	this->lexem = lexem;
	this->value = value;
}

Information::~Information() {
	delete(this->lexem);
}

bool Information::compareLexem(char* lexem) {
	return !strcmp(this->lexem, lexem);
}

char* Information::getLexem(){
	return this->lexem;
}

int* Information::getValue() {
	return this->value;
}

void Information::setValue(int* value) {
	this->value = value;
}
