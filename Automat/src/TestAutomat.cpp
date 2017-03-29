#include "../includes/Automat.h"
#include "../../Scanner/includes/Scanner.h"

int main (int argc, char* argv[]) {
	Automat* automat;
	State* state;

	automat = new Automat();
	state = StateStart::makeState();

	char c = '#';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = '9';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());
	c = '9';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());
	c = '9';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());
	c = '9';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = 'a';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());
	c = 'a';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = ':';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = ':';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = '*';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = '#';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = '*';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = ':';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char %c and counter %i\n", c, automat->getCounter());

	c = 'v';
	state->read(c, automat);
	state = automat->getCurrentState();
	printf("Char: %c and counter %i\n", c, automat->getCounter());


	delete(automat);
}
