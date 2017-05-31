/*
 * Automat.cpp
 *
 */

#include "../includes/Automat.h"  // includes State.h, IScanner.h, etc

Automat::Automat(IScanner& scan) {
	this->scanner = &scan;
	this->state_current = StateStart::makeState();
	this->last_final_state = NULL;
	this->string_buffer_length = Automat::STRING_BUFFER_STEP;
	this->last_string = new char[Automat::STRING_BUFFER_STEP];
	this->last_string_len = 0;
	this->blockIncrement = 0;
	this->counter = 0;
	this->column = 0;
	this->line = 0;
}

Automat::~Automat() {
	delete[] this->last_string;
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

/*
 * "incrementWithoutAppendChar"
 */
void Automat::incrementCounter() {
	this->counter++;
}

void Automat::incrementNewline() {
	this->line += 1;
	this->column = 0;
}

void Automat::incrementTabulator() {
	this->column += TAB_WIDTH - (this->column % TAB_WIDTH); // http://c-for-dummies.com/blog/?p=424
}

int Automat::getCounter() {
	return this->counter;
}

void Automat::resetCounter() {
	this->counter = 0;
	memset(this->last_string, 0, this->string_buffer_length);
	this->last_string_len = 0;
}

void Automat::ungetChar(int count) {
	if (count > this->column || count < 0) {
		// We never go back a line
		errno = EINVAL;
		perror("[A] Illegal unget()");
		this->column = 0;
	} else {
		this->column -= count;
		this->counter -= count;
		//this->blockIncrement = count;
	}
	// buffer->unget() has been done by Scanner for us
}

/*
 * Feeds the next character to the state machine, which call the according scanner actions.
 * Also keeps track of line and column numbers
 */
void Automat::readChar(char c) {
	this->state_current->read(c, this); // calls this->setState() ...

	// increment after read!
	if (!this->blockIncrement) {
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
	} else {
		// the char we just pass to our state has been ungotten
		// so wait for next 'real' new read
		this->blockIncrement--;
	}
}

/*
 * Another approach would be to take the count variable and get the according
 * amount of characters from the buffer ...
 *
 * TODO At least we should use some better mechanism like in the StringTable.resize() ...
 * TODO Should be possible to merge the meaning of last_string_len and counter -> this func becomes "incrementWithAppendChar"
 *
 * O(n^2) with n length of identifier/number ...
 */
void Automat::appendCharToString(char c) {
	if (this->last_string_len >= this->string_buffer_length) {
		char* tmp = this->last_string;
		this->string_buffer_length += Automat::STRING_BUFFER_STEP;
		this->last_string = new char[this->string_buffer_length];
		memset(this->last_string, 0, this->string_buffer_length);
		strcpy(this->last_string, tmp);
		delete[] tmp;
	}
	this->last_string[this->last_string_len] = c; // overwrite last '\0'
	this->last_string_len++;
}

char* Automat::getLastString() {
	return this->last_string;
}

long int Automat::getIntegerValue() {
	/* strtol:
	On success, the function returns the converted integral number as a long int value.
	If no valid conversion could be performed, a zero value is returned (0L).
	If the value read is out of the range of representable values by a long int,
		the function returns LONG_MAX or LONG_MIN (defined in <climits>), and errno is set to ERANGE.
	 */
	long int result = strtol(this->last_string, NULL, 10);

	if (errno == ERANGE) {
		// TODO ?
		printf("'%s' in line %d column %d is out of range! Automatically set to %ld (%s)\n", this->last_string, this->getLine(), this->getColumn(), result, result == LONG_MAX ? "LONG_MAX" : "LONG_MIN");
		perror("Parsing number failed");
		errno = 0; // regard as handled
	}
	//for (int i = 0; this->last_string[i] != '\0'; i++) {
	//	result = result * 10 + this->last_string[i] - '0';
	//}
	return result;
}

char Automat::getUnknownCharacter() {
	return this->last_string[0];
}
