/*
 * StateStaticDefinitions.h
 *
 * This is really more like a .cpp  than a .h file.
 *
 * It contains the very generic and repetitive stuff for state.cpp, which only bloats the code.
 *
 *  Created on: Apr 27, 2017
 *      Author: arthur
 */

#ifndef AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_
#define AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_

#ifndef AUTOMAT_INCLUDES_STATE_H_
#error Error! State.h must be included before StateStaticDefinitions.h
#endif /* AUTOMAT_INCLUDES_STATE_H_ */

StateStart StateStart::instance;

State* StateStart::makeState() {
	return &StateStart::instance;
}

StateRestart StateRestart::instance;

State* StateRestart::makeState() {
	return &StateRestart::instance;
}

StateNumber StateNumber::instance;

State* StateNumber::makeState() {
	return &StateNumber::instance;
}

StateIdentifier StateIdentifier::instance;

State* StateIdentifier::makeState() {
	return &StateIdentifier::instance;
}

StateSingleSign StateSingleSign::instance;

State* StateSingleSign::makeState() {
	return &StateSingleSign::instance;
}

StateAnd StateAnd::instance;

State* StateAnd::makeState() {
	return &StateAnd::instance;
}

StateColon StateColon::instance;

State* StateColon::makeState() {
	return &StateColon::instance;
}

StateColonEquals StateColonEquals::instance;

State* StateColonEquals::makeState() {
	return &StateColonEquals::instance;
}

StateEquals StateEquals::instance;

State* StateEquals::makeState() {
	return &StateEquals::instance;
}

StateEqualsColon StateEqualsColon::instance;

State* StateEqualsColon::makeState() {
	return &StateEqualsColon::instance;
}

StateEqualsColonEquals StateEqualsColonEquals::instance;

State* StateEqualsColonEquals::makeState() {
	return &StateEqualsColonEquals::instance;
}

StateCommentBegin StateCommentBegin::instance;

State* StateCommentBegin::makeState() {
	return &StateCommentBegin::instance;
}

StateCommentEnd StateCommentEnd::instance;

State* StateCommentEnd::makeState() {
	return &StateCommentEnd::instance;
}

StateUnknown StateUnknown::instance;

State* StateUnknown::makeState() {
	return &StateUnknown::instance;
}

#endif /* AUTOMAT_INCLUDES_STATESTATICDEFINITIONS_H_ */
