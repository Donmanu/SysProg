#include "../includes/Symboltable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	Symboltable* symboltable;

	symboltable = new Symboltable();

	Key key1 = symboltable->insert("Test");
	Key key2 = symboltable->insert("Test");
	Key key3 = symboltable->insert("Test2");

	char* string1 = symboltable->lookup(key1).getLexem();
	char* string2 = symboltable->lookup(key2).getLexem();
	char* string3 = symboltable->lookup(key3).getLexem();

	printf("Test: %s\n", string1);
	printf("Test: %s\n", string2);
	printf("Test2: %s\n", string3);



	if (symboltable->lookup(key1).compareLexem(string2)) {
		printf("String %s and %s are Equal\n", string1, string2);
	}

	if (!symboltable->lookup(key1).compareLexem(string3)) {
		printf("String %s and %s are not Equal\n", string1, string3);
	}

	if (!symboltable->lookup(key3).compareLexem(string1)) {
		printf("String %s and %s are not Equal\n", string3, string1);
	}

	if(!symboltable->lookup(key2).compareLexem("test")) {
		printf("String %s and %s are not Equal\n", string2, "test");
	}

	delete(symboltable);

	printf("Test Success\n");

	exit(EXIT_SUCCESS);
}
