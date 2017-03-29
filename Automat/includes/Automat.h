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
#include "../../Scanner/includes/IScanner.h"

class State;

class Automat {
public:
	Automat();
	virtual ~Automat();
	void mkToken();
	void ungetChar(int counter);
	void setScanner(IScanner* scanner);
	void setCurrentState(State* state);
	void setLastFinalState(State* state);
	IScanner* getScanner();
	State* getCurrentState();
	State* getLastFinalState();
	void incrementCounter();
	int getCounter();
	void resetCounter();

private:
	IScanner* scanner;
	State *state_current;
	State* last_final_state;
	int counter;
};

#endif /* Automat_H_ */
