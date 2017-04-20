#include "../includes/Automat.h"
#include "../../Scanner/includes/Scanner.h"

int main (int argc, char* argv[]) {
	Automat* automat;
	State* state;
	char* t = "empty.txt"; // param needed but not used

	automat = new Automat(new Scanner(t));
	state = StateStart::makeState();

	char* c = "#98765abc::**:v";
	int i = 0;
	do {
		state->read(c[i++], automat);
		state = automat->getCurrentState();
		printf("Char %c and counter %i\n\n", c, automat->getCounter());
	} while (*c != '\0');

	delete(automat);
}
