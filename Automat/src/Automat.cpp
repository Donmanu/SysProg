/*
 * Automat.cpp
 *
 */

#include "../includes/Automat.h"  // includes State.h, IScanner.h, etc

Automat::Automat(IScanner& scan) {
	this->scanner = &scan;
	this->state_current = StateStart::makeState();
	this->last_final_state = NULL;
	this->final_identifier = NULL;
	this->counter = 0;
	this->column = 0;
	this->line = 0;
}

Automat::~Automat() {
	// states are static
	// we don't want delete the scanner
	// rest is POD

	// so nothing to do here
}

void Automat::setScanner(IScanner& scanner) {
	this->scanner = &scanner;
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
	return this->column - this->counter + 1; // return in human readable: +1
}

int Automat::getLine() {
	return this->line + 1; // return in human readable: +1
}

void Automat::incrementCounter() {
	this->counter++;
}

void Automat::incrementNewline() {
	//this->counter += 1;
	this->line += 1;
	this->column = 0;
}

void Automat::incrementTabulator() {
	//this->counter += 1;
	this->column += TAB_WIDTH - (this->column % TAB_WIDTH); // http://c-for-dummies.com/blog/?p=424
}

int Automat::getCounter() {
	return this->counter;
}

void Automat::resetCounter() {
	this->counter = 0;
	// is this good here?:
	if (this->final_identifier != NULL) {
		delete[] this->final_identifier;
		this->final_identifier = NULL;
	}
}

void Automat::ungetChar(int count) {
	/*if (this->column == 0) { // should never happen!
		this->line--; // col = lastCol needed??
	} else */
	if (count > this->column) {
		// We assume we never go back a line
		//errno = ?;
		printf("[A] %d > %d\n", count, this->column);
		perror("[A] Un-getting more than a line!?");
		this->column = 0;
	} else {
		this->column -= count;
		//this->counter -= count; // needed?
	}

	// buffer->unget() is done by Scanner
}

/*
 * Feeds the next character to the state machine, which call the according scanner actions.
 * Also keeps track of line and column numbers
 */
void Automat::readChar(char c) {
	this->state_current->read(c, this); // calls this->setState() ...

	// increment after read!
	switch (c) {
	case '\n':
		this->incrementNewline();
		break;
	case '\t':
		this->incrementTabulator();
		break;
	case '\0':
		this->scanner->stop();
		break;
	default:
		this->column++;
	}
}

/*
 * Another approach would be to take the count variable and get the according
 * amount of characters from the buffer ...
 */
char* Automat::appendCharToString(char c) {
	int length;
	char* tmpString;
	if (this->final_identifier != NULL) {
		length = (int) strlen(this->final_identifier);
		tmpString = this->final_identifier;
	} else {
		length = 0;
		tmpString = "";
	}
	this->final_identifier = new char[length + 2];
	strcpy(this->final_identifier, tmpString);
	this->final_identifier[length] = c;
	this->final_identifier[length + 1] = '\0';
	//delete[] tmpString; //delete not needed for POD on function end ??
	printf("append() returning %s\n", this->final_identifier);
	return this->final_identifier; // nobody needs that ... ?
}

char* Automat::getFinalIdentifier() {
	return this->final_identifier;
}
