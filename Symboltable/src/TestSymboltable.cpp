#include "../includes/Symboltable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	Symboltable* symboltable;

	symboltable = new Symboltable();

	printf("Test Success");

	delete(symboltable);

	exit(EXIT_SUCCESS);
}
