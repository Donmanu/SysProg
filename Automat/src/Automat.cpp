/*
 * Automat.cpp
 *
 */

#include "../includes/Automat.h"

Automat::Automat() {
	// TODO Auto-generated constructor stub
	this->scanner = NULL;
	this->state_current = NULL;
	this->last_final_state = NULL;
	this->counter = 0;
}

Automat::~Automat() {
	// TODO Auto-generated destructor stub
}

void Automat::setScanner(IScanner* scanner) {
	this->scanner = scanner;
}

void Automat::setCurrentState(State* state) {
	this->state_current = state;
}

void Automat::setLastFinalState(State* state) {
	this->last_final_state = state;
}

IScanner* Automat::getScanner() {
	return this->scanner;
}

State* Automat::getCurrentState() {
	return this->state_current;
}

State* Automat::getLastFinalState() {
	return this->last_final_state;
}

void Automat::incrementCounter() {
	this->counter += 1;
}

int Automat::getCounter() {
	return this->counter;
}

void Automat::resetCounter() {
	this-> counter = 0;
}
