/*
 * State.cpp
 *
 * Using case ranges here, which are an GCC feature!
 *
 *  Created on: Mar 24, 2017
 *      Author: donmanu
 */

#include "../includes/State.h"
#include "../includes/StateStaticDefinitions.h"

void StateStart::read(char c, Automat* m) {
	m->resetCounter();
	switch (c) {
		case '0' ... '9':
			m->setCurrentState(StateNumber::makeState());
			m->setLastFinalState(StateNumber::makeState());
			m->incrementCounter();
		break;
		case 'A' ... 'Z':
		case 'a' ... 'z':
			m->setCurrentState(StateIdentifier::makeState());
			m->setLastFinalState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case '+':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenPlus);
			m->incrementCounter(); // TODO increment counter on single sign?? Or do we rather make the according token and go back to Start??
			break;
		case '-':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenMinus);
			m->incrementCounter();
			break;
		case '*':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenStar);
			m->incrementCounter();
			break;
		case '<':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenLessThan);
			m->incrementCounter();
			break;
		case '>':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenGreaterThan);
			m->incrementCounter();
			break;
		case '(':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenBracketsOpen);
			m->incrementCounter();
			break;
		case ')':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenBracketsClose);
			m->incrementCounter();
			break;
		case '[':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsOpen);
			m->incrementCounter();
			break;
		case ']':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsClose);
			m->incrementCounter();
			break;
		case '{':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracketsOpen);
			m->incrementCounter();
			break;
		case '}':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracketsClose);
			m->incrementCounter();
			break;
		case '!':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenExclamationMark);
			m->incrementCounter();
			break;
		case ';':
			m->setCurrentState(StateSingleSign::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSemiColon);
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
		case '\t':
		case ' ':
			/* Stay in Start */
			break;
		default:
			m->setCurrentState(StateUnknown::makeState());
			m->setLastFinalState(StateUnknown::makeState());
			m->getCurrentState()->read(c, m);
	}
}

void StateError::read(char c, Automat* m) {
	// Purpose of this state is to make a controlled transition into start again
	m->setCurrentState(StateStart::makeState());
	//m->getScanner()->ungetChar(m->getCounter()); // < too much magic!
	// Can we do anything useful here?!? If not, this state can be removed ...
	m->getCurrentState()->read(c, m);
}

void StateNumber::read(char c, Automat* m) {
	switch (c) {
		case '0' ... '9':
			m->setCurrentState(StateNumber::makeState());
			m->setLastFinalState(StateNumber::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenInteger);
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenInteger);
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

void StateIdentifier::read(char c, Automat* m) {
	switch (c) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z':
			m->setCurrentState(StateIdentifier::makeState());
			m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenIdentifier);
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenIdentifier);
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

void StateSingleSign::read(char c, Automat* m) {
	switch (c) {
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			break;
		default:
			// if it's really a single sign (sign!, sign+, sign] and so on), StateStart handles it already
			// so don't make a token here
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

/*
 * in-between state for ( ) -> (&) -> ((&&))
 */
void StateAnd::read(char c, Automat* m) {
	switch (c) {
		case '&':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateAnd::makeState());
			m->getScanner()->mkToken(TokenType::TokenAndAnd);
			m->incrementCounter();
			break;
		default:
			m->setCurrentState(StateUnknown::makeState()); // < this refers to the '&' that came before!
			m->getCurrentState()->read(c, m);
	}
}

/*
 * in-between state AND final state!
 */
void StateColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateColon::makeState());
			m->getScanner()->mkToken(TokenType::TokenColonEquals);
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
			m->getScanner()->mkToken(TokenType::TokenColon);
			break;
		default:
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

/*
 * final state
 */
void StateColonEquals::read(char c, Automat* m) {
	switch(c) {
	default:
		m->setCurrentState(StateStart::makeState());
		m->getScanner()->mkToken(TokenType::TokenColon);
		m->getCurrentState()->read(c, m);
	}
}

/*
 * in-between state for ((=:=)) and final state for ((=))
 */
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
			m->getScanner()->mkToken(TokenType::TokenEquals);
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenEquals);
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

/*
 * in-between state:
 *   (=:) --'='-> ((=:=))
 * vs
 *   (=:) --'x'-> ((=)), ((:)), ((x))
 */
void StateEqualsColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->setLastFinalState(StateEqualsColonEquals::makeState());
			m->getScanner()->mkToken(TokenType::TokenEqualsColonEquals);
			m->incrementCounter();
			break;
		default:
			// is that really enough: TODO
			m->setCurrentState(StateError::makeState());
			m->getCurrentState()->read(c, m);
	}
}

void StateEqualsColonEquals::read(char c, Automat* m) {
	m->setCurrentState(StateStart::makeState());
	m->getCurrentState()->read(c, m);
}

void StateCommentBegin::read(char c, Automat* m) {
	if (c == '*' || c == '\0') {
		m->setCurrentState(StateCommentEnd::makeState());
	}
}

/*
 * in-between state for actual comment end:
 *
 *                                    |:--> (StateStart)
 *                                    |
 * :* bla bla   --*--> (commentEnd) --|*--> (CommentEnd)
 * ^                                  |
 * (CommentBegin) <--------_----------|
 *
 */
void StateCommentEnd::read(char c, Automat* m) {
	switch (c) {
		case '\0': // EOF
		case ':':
			// comment finished
			m->setCurrentState(StateStart::makeState());
			// no token for comments to create
			break;
		case '*':
			// another star after star  :****:
			// nothing to do: stay in CommentEnd
			break;
		default:
			// back to beginning
			m->setCurrentState(StateCommentBegin::makeState());
	}
}

void StateUnknown::read(char c, Automat* m){
	m->setCurrentState(StateStart::makeState());
	m->getScanner()->mkToken(TokenType::TokenUnknown);
	//m->incrementCounter();
}
