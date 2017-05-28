/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

/* ------------------- SymTabEntry ---------------------- */

SymTabEntry::SymTabEntry() {
	this->next = NULL;
	this->string_tab_node = NULL;
	this->key = NULL;
}

SymTabEntry::SymTabEntry(Key* key, StringTabNode* node) {
	this->key = key;
	this->string_tab_node = node;

	this->next = NULL;
}

SymTabEntry::~SymTabEntry() {
	delete this->next; // triggers a destructor chain
	delete this->string_tab_node;
	delete this->key;
}

bool SymTabEntry::hasNext() {
	return this->next != NULL;
}

SymTabEntry* SymTabEntry::getNext() {
	return this->next;
}

void SymTabEntry::setNext(SymTabEntry* next) {
	this->next = next;
}

void SymTabEntry::setStringTabNode(StringTabNode* node) {
	this->string_tab_node = node;
}

char* SymTabEntry::getLexem() {
	return this->key->getInformation()->getLexem();
}

Key* SymTabEntry::getKey() {
	return this->key;
}

void SymTabEntry::setKey(Key* key) {
	this->key = key;
}

/* ------------------- Symboltable ---------------------- */

Symboltable::Symboltable() {
	this->string_table = new StringTab[Symboltable::SYMBOL_TABLE_SIZE];
	this->table_size = Symboltable::SYMBOL_TABLE_SIZE;
	this->free_space = Symboltable::SYMBOL_TABLE_SIZE;
	this->entries = new SymTabEntry*[Symboltable::SYMBOL_TABLE_SIZE];
	for (int i = 0; i < Symboltable::SYMBOL_TABLE_SIZE; i++)
		this->entries[i] = NULL;
}

Symboltable::~Symboltable() {
	// delete entries:
	for (int i = 0; i < this->table_size; i++)
		delete this->entries[i]; // each triggers a delete chain
	// delete array of pointers to entries:
	delete[] this->entries;
	delete[] this->string_table;
}

Key* Symboltable::insert(char* lexem) {
	if (lexem == NULL) {
		errno = EINVAL; // invalid arg
		perror("NULL given to Symboltable::insert()");
		return NULL; // also bad, but meh ...
	}
	int hash = this->hash(lexem);
	SymTabEntry* current = this->entries[hash];

	if (current != NULL) {
		while (current->hasNext()) { // go to end of row. On the way: check, if entry already there. Return if so.
			if (current->getKey()->getInformation()->compareLexem(lexem))
				current->getKey()->getInformation()->incrementOccurrences();
				return current->getKey();
			current = current->getNext();
		}
	}
	// here we have a new Symbol!
	// current is now pointing to the last entry in the row, which is possibly null

	// TODO if (this->string_table->node_count > this->table_size/* * loadfactor 0.75 as in Java ??*/)
	//          this->resize();
	//
	// this->string_table->node_count; // string table is keeping track of the node count, so we don't introduce another int here ...

	Information* i = new Information(lexem);
	Key* k = new Key(i);
	StringTabNode* n = new StringTabNode(i->getLexem()); // insert into StringTab and let StringTabNode point to Information ...

	if (current != NULL) {
		current->setNext(new SymTabEntry(k, n)); // new Key(new Information(lexem)), new StringTabNode(lexem)
		current = current->getNext();
	} else {
		this->entries[hash] = new SymTabEntry(k, n);
		current = this->entries[hash];
	}

	this->string_table->insert(lexem, strlen(lexem));

	return current->getKey();
}

Information Symboltable::lookup(Key key) {
	return *key.getInformation();
}

void Symboltable::resize() {

	errno = ENOSYS;
	perror("WARNING! resize() does not work yet!");

	int new_table_size = this->table_size * 2;
	SymTabEntry** temp = new SymTabEntry*[new_table_size];
	memcpy(temp, this->entries, this->table_size);   // TODO Wrong! Need to rehash all entries with new modulo table_size
	this->table_size = new_table_size;
	this->free_space += SYMBOL_TABLE_SIZE;
	delete[] this->entries;
	this->entries = temp;
}

int Symboltable::hash(char* lexem) {
	// lexem = NULL won't work! Check beforehand!
	int hash = 0;
	int i = 0;

	while (lexem[i] != '\0') {
		hash += (SALT * lexem[i]) % this->table_size;
		// " % this->table_size" on every step keeps the hash number small, even for long identifiers
		// but also rehashing on resize must be done completely. Alternatively: https://stackoverflow.com/a/14251372
		// Example: http://www.algolist.net/Data_structures/Hash_table/Dynamic_resizing
		i++;
	}

	return hash % this->table_size;
}

void Symboltable::debugPrint() {
	int e = 0;
	SymTabEntry* s;

	printf("\n --- SYMBOL_TABLE: ---\n");

	for (; e < SYMBOL_TABLE_SIZE; e++) {
		s = this->entries[e];
		printf("[%d]", e);
		while (s != NULL) {
			printf("->(%s/%d)", s->getKey()->getInformation()->getLexem(), s->getKey()->getInformation()->getOccurrences());
			s = s->getNext();
		}
		printf("->( )\n");
	}

	printf("\n --- STRING_TABLE: ---\n");
	this->string_table->debugPrint();

	printf("\n");
}
