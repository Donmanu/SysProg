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
			m->incrementAndAppend(c);
			m->setCurrentState(StateNumber::makeState());
			//m->incrementCounter();
		break;
		case 'A' ... 'Z':
		case 'a' ... 'z':
			m->incrementAndAppend(c);
			m->setCurrentState(StateIdentifier::makeState());
			//m->incrementCounter();
			break;

		/* Here come the single-sign operators, which we shortcut handle by making the according token right away,
		 * and staying in StateStart, ignoring counter increment */
		case '+':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenPlus);
			break;
		case '-':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenMinus);
			break;
		case '*':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenStar);
			break;
		case '<':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenLessThan);
			break;
		case '>':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenGreaterThan);
			break;
		case '(':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenParenthesisOpen);
			break;
		case ')':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenParenthesisClose);
			break;
		case '[':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsOpen);
			break;
		case ']':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenSquareBracketsClose);
			break;
		case '{':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracesOpen);
			break;
		case '}':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenCurlyBracesClose);
			break;
		case '!':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenExclamationMark);
			break;
		case ';':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenSemiColon);
			break;

		/* Here come the special operators, which have (perhaps) extra length, so we do increment the counter here */
		case '&':
			m->incrementAndAppend(c); // if it stands alone 'and' gets "unknown character"
			m->setCurrentState(StateAnd::makeState());
			break;
		case ':':
			m->setCurrentState(StateColon::makeState());
			m->incrementCounter();
			break;
		case '=':
			m->setCurrentState(StateEquals::makeState());
			m->incrementCounter();
			break;
		case '\n':
		case '\t':
		case ' ':
			/* Stay in Start */
			break;
		default:
			m->incrementAndAppend(c);
			m->getScanner()->mkToken(TokenType::TokenUnknown);
			/* and stay in Start */
	}
}

void StateRestart::read(char c, Automat* m) {
	// Purpose of this state is to make a controlled transition into start again
	//printf("Restart.read(): c is '%c', l=%d, col=%d\n", c, m->getLine(), m->getColumn());
	m->setCurrentState(StateStart::makeState());
	m->getScanner()->ungetChar(1); // 1 default works here. Else StateError should not be entered
	// TODO ungetting(1) means -> hey "start", read that again. So why not set to start directly and make an epsilon transition?
}

void StateNumber::read(char c, Automat* m) {
	switch (c) {
		case '0' ... '9':
			m->incrementAndAppend(c);
			m->setCurrentState(StateNumber::makeState());
			//m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenInteger);
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenInteger);
			m->setCurrentState(StateRestart::makeState());
			m->getCurrentState()->read(c, m);
	}
}

void StateIdentifier::read(char c, Automat* m) {
	switch (c) {
		case '0' ... '9':
		case 'A' ... 'Z':
		case 'a' ... 'z':
			m->incrementAndAppend(c);
			m->setCurrentState(StateIdentifier::makeState());
			//m->incrementCounter();
			break;
		case ' ':
		case '\n':
		case '\t':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenIdentifier);
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenIdentifier);
			m->setCurrentState(StateRestart::makeState());
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
			m->getScanner()->mkToken(TokenType::TokenAndAnd);
			m->incrementCounter();
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenUnknown);
			m->setCurrentState(StateRestart::makeState());
			m->getCurrentState()->read(c, m);
	}
}



/*
 *     in-between state for ":=" and ":*"
 * AND final state for ":"!
 */
void StateColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			// shortcut: make Token here already
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenColonEquals);
			m->incrementCounter();
			break;
		case '*':
			m->setCurrentState(StateCommentBegin::makeState());
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenColon);
			m->setCurrentState(StateRestart::makeState());
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
			m->incrementCounter();
			break;
		default:
			m->getScanner()->mkToken(TokenType::TokenEquals);
			m->setCurrentState(StateStart::makeState());
			m->getCurrentState()->read(c, m); // epsilon transition
	}
}

/*
 * in-between state:
 *   (=:) ---'='--> ((=:=))
 * vs
 *   (=:) ---'x'--> ((=)), ((:)), ((x))
 */
void StateEqualsColon::read(char c, Automat* m) {
	switch (c) {
		case '=':
			m->setCurrentState(StateStart::makeState());
			m->getScanner()->mkToken(TokenType::TokenEqualsColonEquals);
			m->incrementCounter();
			break;
		default:
			m->getScanner()->ungetChar(2); // read in ":" and c again
			m->getScanner()->mkToken(TokenType::TokenEquals);
			m->setCurrentState(StateStart::makeState());
	}
}

void StateCommentBegin::read(char c, Automat* m) {
	if (c == '*' || c == '\0') {
		m->setCurrentState(StateCommentEnd::makeState());
	}
	// else: nothing to do: stay in CommentBegin
}

/*
 * in-between state for actual comment end:
 *
 *                                    |-:--> (StateStart)
 *                                    |
 * :* bla bla   --*--> (commentEnd) --|-*--> (CommentEnd)
 * ^                                  |
 * (CommentBegin) <----default--------|
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
