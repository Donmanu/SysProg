/*
 * State.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#include "../includes/State.h"

StateStart StateStart::instance;

State* StateStart::makeState() {
	return &instance;
}

void StateStart::read(char c, Automat* m) {
	m->resetCounter();
	printf("State: Start\n");
	switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			m->setCurrentState(StateNumber::makeState());
			m->setLastFinalState(StateNumber::makeState());
			m->incrementCounter();
			break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			m->setCurrentState(StateIdentifier::makeState());
			m->setLastFinalState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case '+':
		case '-':
		case '*':
		case '<':
		case '>':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '!':
		case '?':
		case ';':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->incrementCounter();
			break;
		case '&':
			m->setCurrentState(StateAnd::makeState());
			m->incrementCounter();
			break;
		case ':':
			m->setCurrentState(StateColon::makeState());
			m->setLastFinalState(StateColon::makeState());
			m->incrementCounter();
			break;
		case '=':
			m->setCurrentState(StateEquals::makeState());
			m->setLastFinalState(StateEquals::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			break;
		default:
			m->setCurrentState(StateUnknown::makeState());
			m->setLastFinalState(StateUnknown::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateError StateError::instance;

State* StateError::makeState() {
	return &instance;
}

void StateError::read(char c, Automat* m) {
	printf("State: Error\n");
	m->setCurrentState(StateStart::makeState());
	if (m->getScanner() != NULL) {
		m->ungetChar(m->getCounter());
	}
}

StateNumber StateNumber::instance;

State* StateNumber::makeState() {
	return &instance;
}

void StateNumber::read(char c, Automat* m) {
	printf("State: Number\n");
	switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			m->setCurrentState(StateNumber::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateIdentifier StateIdentifier::instance;

State* StateIdentifier::makeState() {
	return &instance;
}

void StateIdentifier::read(char c, Automat* m) {
	printf("State: Identifier\n");
	switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			m->setCurrentState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateSingleSign StateSingleSign::instance;

State* StateSingleSign::makeState() {
	return &instance;
}

void StateSingleSign::read(char c, Automat* m) {
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateAnd StateAnd::instance;

State* StateAnd::makeState() {
	return &instance;
}

void StateAnd::read(char c, Automat* m) {
	switch (c) {
		case '&':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateAnd::makeState()); //TODO: state_sign?
			m->incrementCounter();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateColon StateColon::instance;

State* StateColon::makeState() {
	return &instance;
}

void StateColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateColon::makeState()); //TODO: state_sign?
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateEquals StateEquals::instance;

State* StateEquals::makeState() {
	return &instance;
}

void StateEquals::read(char c, Automat* m) {
	switch (c) {
		case ':':
			m->setCurrentState(StateEqualsColon::makeState());
			m->setLastFinalState(StateEqualsColon::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateEqualsColon StateEqualsColon::instance;

State* StateEqualsColon::makeState() {
	return &instance;
}

void StateEqualsColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateEqualsColon::makeState()); //TODO: state_sign?
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateSlash StateSlash::instance;

State* StateSlash::makeState() {
	return &instance;
}

void StateSlash::read(char c, Automat* m) {
	switch (c) {
		case '*':
			m->setCurrentState(StateCommentBegin::makeState());
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			//TODO: m->mkToken(); ??
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateCommentBegin StateCommentBegin::instance;

State* StateCommentBegin::makeState() {
	return &instance;
}

void StateCommentBegin::read(char c, Automat* m) {
	if (c == '*') {
		m->setCurrentState(StateCommentEnd::makeState());
	}
}

StateCommentEnd StateCommentEnd::instance;

State* StateCommentEnd::makeState() {
	return &instance;
}

void StateCommentEnd::read(char c, Automat* m) {
	switch (c) {
		case '/':
			m->setCurrentState(StateStart::makeState());
			break;
		default:
			m->setCurrentState(StateCommentEnd::makeState());
	}
}

StateUnknown StateUnknown::instance;

State* StateUnknown::makeState() {
	return &instance;
}

void StateUnknown::read(char c, Automat* m){
	printf("reading in state unknown");
	m->setCurrentState(StateStart::makeState());
	//TODO: m->mkToken();
}

std::string StateUnknown::toString() {
	return "StateUnknown";
}
