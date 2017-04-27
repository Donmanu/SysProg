/*
 * Automat.cpp
 *
 */

#include "../includes/Automat.h"  // includes State.h
#include "../../Scanner/includes/IScanner.h"

Automat::Automat(IScanner* scan) {
	this->scanner = scan;
	this->state_current = StateStart::makeState();
	this->last_final_state = NULL;
	this->counter = 0;
	this->column = 0;
	this->line = 0;
}

Automat::~Automat() {
	// TODO Auto-generated destructor stub
	// Don't delete the scanner!

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
int Automat::getColumn() {
	return this->column; // - counter ?? TODO
}

int Automat::getLine() {
	return this->line;
}

void Automat::incrementCounter() {
	this->counter += 1;
}

void Automat::incrementNewline() {
	this->counter += 1;
	this->line += 1;
	this->column = 0;
}

void Automat::incrementTabulator() {
	this->counter += 1;
	this->column += TAB_WIDTH - (this->column % TAB_WIDTH); // http://c-for-dummies.com/blog/?p=424
}

int Automat::getCounter() {
	return this->counter;
}

void Automat::resetCounter() {
	this-> counter = 0;
	// TODO what to do with line/column? column -= counter??
}

void Automat::readChar(char c) {
	// TODO Do some state changing and/or ground braking thing here ...
	this->state_current->read(c, this); // calls makeState() ...
}
