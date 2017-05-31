/*
 * Automat.h
 *
 *  Created on: Jul 5, 2012
 *      Author: knad0001
 */

#ifndef Automat_H_
#define Automat_H_

#include <unistd.h>
#include <string.h>
#include <cstdlib> // strtol()
#include <climits> // LONG_MAX, LONG_MIN
#include "State.h"
#include "../../Scanner/includes/ScannerForAutomat.h"
#include "../../Scanner/includes/Token.h"

class State;

class Automat {
public:
	Automat(IScanner&);
	virtual ~Automat();
	void setScanner(IScanner& scanner);
	void setCurrentState(State* state);
	void setLastFinalState(State* state);
	IScanner* getScanner();
	State* getCurrentState();
	int getColumn();
	int getLine();
	State* getLastFinalState();
	void incrementCounter();
	void incrementNewline();
	void incrementTabulator();
	int getCounter();
	void resetCounter();
	void ungetChar(int);
	void readChar(char);
	void appendCharToString(char c);
	char* getLastString();
	long int getIntegerValue();
	char getUnknownCharacter();

private:
	IScanner* scanner;
	State* state_current;
	State* last_final_state;
	char* last_string; // holder, identifiers, integers and unknowns
	int last_string_len;
	int counter; // basically the same as last_string_len TODO refactor to merge counter with last_string_len (appendToString(c){... counter++; ...} )
	int column;
	int line;
	int blockIncrement;
};

#endif /* Automat_H_ */
