/*
 * Automat.h
 *
 *  Created on: Jul 5, 2012
 *      Author: knad0001
 */

#ifndef Automat_H_
#define Automat_H_

#include <unistd.h>
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

private:
	IScanner* scanner;
	State* state_current;
	State* last_final_state;
	int counter;
	int column;
	int line;
};

#endif /* Automat_H_ */
