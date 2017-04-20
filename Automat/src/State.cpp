/*
 * State.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#include "../includes/State.h"

StateStart StateStart::instance;

State* StateStart::makeState() {
	return &this->instance;
}

void StateStart::read(char c, Automat* m) {
	m->resetCounter();
	printf("State: Start\n");
	switch (c) {
		case 48 ... 57:
			m->setCurrentState(StateNumber::makeState());
			m->setLastFinalState(StateNumber::makeState());
			m->incrementCounter();
		break;
		case 65 ... 90:
		case 97 ... 122:
			m->setCurrentState(StateIdentifier::makeState());
			m->setLastFinalState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case '+':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenPlus);
			m->incrementCounter();
			break;
		case '-':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenMinus);
			m->incrementCounter();
			break;
		case '*':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenStar);
			m->incrementCounter();
			break;
		case '<':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenLessThan);
			m->incrementCounter();
			break;
		case '>':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenGreaterThan);
			m->incrementCounter();
			break;
		case '(':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenBracketsOpen);
			m->incrementCounter();
			break;
		case ')':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenBracketsClose);
			m->incrementCounter();
			break;
		case '[':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenSquareBracketsOpen);
			m->incrementCounter();
			break;
		case ']':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenSquareBracketsClose);
			m->incrementCounter();
			break;
		case '{':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenCurlyBracketsOpen);
			m->incrementCounter();
			break;
		case '}':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenCurlyBracketsClose);
			m->incrementCounter();
			break;
		case '!':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenExclamationMark);
			m->incrementCounter();
			break;
		case ';':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
//			m->getScanner()->mkToken(TokenType::TokenSemiColon);
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
		case '\n':
			m->incrementNewline();
			break;
		case '\t':
			m->incrementTabulator();
			break;
		case ' ':
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
		m->getScanner()->ungetChar(m->getCounter());
	}
}

StateNumber StateNumber::instance;

State* StateNumber::makeState() {
	return &instance;
}

void StateNumber::read(char c, Automat* m) {
	printf("State: Number\n");
	switch (c) {
		case 48 ... 57:
			m->setCurrentState(StateNumber::makeState());
			m->setLastFinalState(StateNumber::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
//			m->getScanner()->mkToken(TokenType::TokenInteger);
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
		case 48 ... 57:
		case 65 ... 90:
		case 97 ... 122:
			m->setCurrentState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
//			m->getScanner()->mkToken(TokenType::TokenIdentifier);
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
	printf("State: Single Sign\n");
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
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
	printf("State: And\n");
	switch (c) {
		case '&':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateAnd::makeState());
//			m->getScanner()->mkToken(TokenType::TokenAndAnd);
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
	printf("State: Colon\n");
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateColon::makeState());
//			m->getScanner()->mkToken(TokenType::TokenColonEquals);
			m->incrementCounter();
			break;
		case '*':
			m->setCurrentState(StateCommentBegin::makeState());
			m->setLastFinalState(StateColon::makeState());
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
//			m->getScanner()->mkToken(TokenType::TokenColon);
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateColonEquals StateColonEquals::instance;

State* StateColonEquals::makeState() {
	return &instance;
}

void StateColonEquals::read(char c, Automat* m) {
	printf("State: Colon Equals\n");
}

StateEquals StateEquals::instance;

State* StateEquals::makeState() {
	return &instance;
}

void StateEquals::read(char c, Automat* m) {
	printf("State: Equals\n");
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
//			m->getScanner()->mkToken(TokenType::TokenEquals);
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
	printf("State: Equals Colon");
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateEqualsColonEquals::makeState());
//			m->getScanner()->mkToken(TokenType::TokenEqualsColonEquals);
			m->incrementCounter();
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

StateEqualsColonEquals StateEqualsColonEquals::instance;

State* StateEqualsColonEquals::makeState() {
	return &instance;
}

void StateEqualsColonEquals::read(char c, Automat* m) {
	printf("State: Equals Colon Equals\n");
}

StateCommentBegin StateCommentBegin::instance;

State* StateCommentBegin::makeState() {
	return &instance;
}

void StateCommentBegin::read(char c, Automat* m) {
	printf("State: Comment Begin\n");
	if (c == '*') {
		m->setCurrentState(StateCommentEnd::makeState());
	}
}

StateCommentEnd StateCommentEnd::instance;

State* StateCommentEnd::makeState() {
	return &instance;
}

void StateCommentEnd::read(char c, Automat* m) {
	printf("State: Comment End\n");
	switch (c) {
		case ':':
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
//	m->getScanner()->mkToken(TokenType::TokenUnknown);
}
