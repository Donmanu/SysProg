#include "../includes/Symboltable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	Symboltable* symboltable;

	symboltable = new Symboltable();

	Key key1 = symboltable->insert("Test", 12);
	Key key2 = symboltable->insert("Test", 4);

	printf("Test, 12: %s, %i\n", symboltable->lookup(key1).getLexem(), symboltable->lookup(key1).getValue());
	printf("Test, 4: %s, %i\n", symboltable->lookup(key2).getLexem(), symboltable->lookup(key2).getValue());

	delete(symboltable);

	printf("Test Success\n");

	exit(EXIT_SUCCESS);
}
