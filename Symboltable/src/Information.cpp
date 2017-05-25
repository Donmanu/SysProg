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
	int length = strlen(lexem);
	this->lexem = new char[length];
	strcpy(this->lexem, lexem);
}

Information::~Information() {
}

bool Information::compareLexem(char* lexem) {
	return !strcmp(this->lexem, lexem);
}

char* Information::getLexem(){
	return this->lexem;
}

