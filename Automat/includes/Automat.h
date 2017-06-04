/*
 * Automat.h
 *
 *  Created on: Jul 5, 2012
 *      Author: knad0001
 */

#ifndef Automat_H_
#define Automat_H_

#define TAB_WIDTH (8)

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
	virtual ~Automat(void);

	// interface for states:
	void      setCurrentState(State* state);
	IScanner* getScanner(void);
	State*    getCurrentState(void);
	int       getCounter(void);
	void      resetCounter(void);
	void      incrementCounter(void);
	void      incrementAndAppend(char c);

	// interface for scanner:
	void     ungetChar(int);
	void     readChar(char);
	int      getColumn(void);
	int      getLine(void);
	char*    getLastString(void);
	long int getIntegerValue(void);
	char     getUnknownCharacter(void);

private:
	void incrementNewline(void);
	void incrementTabulator(void);

	IScanner* scanner;
	State* state_current;
	char* last_string; // holder for: identifiers, integers and unknowns
	int counter;
	int column;
	int line;
	int blockIncrement;
	int string_buffer_length;

	static const int STRING_BUFFER_STEP = 20;
};

#endif /* Automat_H_ */
