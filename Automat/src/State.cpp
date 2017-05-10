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

		/* Here come the single-sign operators, which we shortcut handle by making the according token,
		 * but also going into StateStart and ignoring counter increment */
		case '+':
			m->setCurrentState(StateError::makeState()); // By going over Error to Start, we allow mkToken() to have effect
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenPlus);
			break;
		case '-':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenMinus);
			break;
		case '*':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenStar);
			break;
		case '<':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenLessThan);
			break;
		case '>':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenGreaterThan);
			break;
		case '(':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenParenthesisOpen);
			break;
		case ')':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenParenthesisClose);
			break;
		case '[':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsOpen);
			break;
		case ']':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsClose);
			break;
		case '{':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracesOpen);
			break;
		case '}':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracesClose);
			break;
		case '!':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenExclamationMark);
			break;
		case ';':
			m->setCurrentState(StateError::makeState());
			m->setLastFinalState(StateSingleSign::makeState());
			m->getScanner()->mkToken(TokenType::TokenSemiColon);
			break;

		/* Here come the special operators, which have (perhaps) extra length, so we do increment the counter here */
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
	m->getScanner()->ungetChar(1); // 1 works for now. Later we need a reliable value for Automat.counter ... TODO
	//m->getScanner()->ungetChar(m->getCounter()); // < too much magic!
	// Can we do anything useful here?!? If not, this state can be removed ...
	//m->getCurrentState()->read(c, m);
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
	/* this state really should not read anything */
	perror("Reading in StateSingleSign!\n");
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
