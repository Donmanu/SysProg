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
	this->free_space = Symboltable::SYMBOL_TABLE_SIZE; // currently not used! Alternative to Symboltable::LOADFACTOR
	this->entries = new SymTabEntry*[Symboltable::SYMBOL_TABLE_SIZE];
	for (unsigned int i = 0; i < Symboltable::SYMBOL_TABLE_SIZE; i++)
		this->entries[i] = NULL;
}

Symboltable::~Symboltable() {
	// delete entries:
	for (unsigned int i = 0; i < this->table_size; i++)
		delete this->entries[i]; // each triggers a delete chain
	// delete array of pointers to entries:
	delete[] this->entries;
	delete[] this->string_table;
}

Key* Symboltable::insert(char* lexem) {
	if (lexem == NULL) {
		errno = EINVAL; // invalid arg
		perror("NULL given to Symboltable::insert()");
		return NULL; // also bad, but meh. Should ever happen anyway
	}
	if (this->string_table->getNodeCount() > (int) this->table_size * Symboltable::LOADFACTOR) {
		// we save on the check, if new size would overflow ...
		this->resize();
	}

	unsigned int fullHash = this->hash(lexem);
	unsigned int hash = fullHash % this->table_size;
	SymTabEntry* current = this->entries[hash];

	if (current != NULL) {
		while (current->hasNext()) { // go to end of row. On the way: check, if entry already there. Return if so.
			if (current->getKey()->getInformation()->compareLexem(lexem)) {
				current->getKey()->getInformation()->incrementOccurrences();
				return current->getKey();
			}
			current = current->getNext();
		}
	}
	// here we have a new Symbol!
	// current is now pointing to the last entry in the row, possibly null

	Information* i = new Information(lexem);
	i->incrementOccurrences();
	Key* k = new Key(fullHash, i);
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

/*
 * Insert for the purpose to use on resize. So we can save on some stuff, that we already know:
 *
 *  + inserted entries are going to be unique
 *  + are already in string_table
 *  + new Key, Information, StringTabNode all are already done
 *  + hash has been calculated and saved
 *
 */
void Symboltable::quickInsert(SymTabEntry* s) {
	int hash = s->getKey()->getHash() % this->table_size;
	SymTabEntry* current = this->entries[hash];

	if (current == NULL) {
		this->entries[hash] = s;
		return;
	}

	while (current->hasNext()) { // go to end of row. Not really needed, but it's consistent. Else every resize would switch the order
		current = current->getNext();
	}
	current->setNext(s); // put at end
}

Information Symboltable::lookup(Key key) {
	return *key.getInformation();
}

void Symboltable::resize() {
	unsigned int h;
	unsigned int previous_table_size = this->table_size;
	this->table_size *= 2;

	printf("RESIZING SymTab %d -> %d\n", previous_table_size, this->table_size);

	SymTabEntry** previousEntries = this->entries;
	this->entries = new SymTabEntry*[this->table_size];
	for (h = 0; h < this->table_size; h++)
		this->entries[h] = NULL; // valgrind <3

	SymTabEntry* oldNext;
	for (h = 0; h < previous_table_size; h++) {
		SymTabEntry* entry = previousEntries[h];
		while (entry != NULL) {
			oldNext = entry->getNext(); // save that, as it is going to be reordered.
			entry->setNext(NULL);
			this->quickInsert(entry);
			entry = oldNext;
		}
	}
	delete[] previousEntries; // only the array!

	this->free_space += (this->table_size - previous_table_size); // fool prove, even if resize x3, x4, etc.
}

int Symboltable::hash(char* lexem) {
	// lexem = NULL won't work! Check beforehand!
	// 'sdbm' used in gawk
	unsigned int hash = 0; // SEED = 0
	int i = 0;

	while (lexem[i]) {
		hash = (hash << 6) + (hash << 16) - hash + lexem[i]; // optimized SALT = 65599 = 2^16 + 2^6 - 1
		i++;
	}
	return hash;

	// Jenkins 'One At A Time' Hash:
	/*unsigned int hash = 0;
	int i = 0;

	while (lexem[i])
	{
		hash += lexem[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
		i++;
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash % this->table_size;*/



	/*
	unsigned int hash = Symboltable::SEED;
	int i = 0;

	while (lexem[i]) {
		hash += (hash * Symboltable::SALT) * lexem[i]; // % this->table_size;
		i++;
	}

	return (int) hash % this->table_size;*/
}

void Symboltable::debugPrint() {
	unsigned int e = 0;
	int empties = 0;
	int chain = 0;
	int max_chain = 0;
	double chains = 0.0;
	double avg = 0.0;

	double red_dragon = 0.0; // https://www.strchr.com/hash_functions -> middle of page, formula for measuring hash quality

	SymTabEntry* s;

	printf("\n --- SYMBOL_TABLE: ---\n");

	for (; e < this->table_size; e++) {
		this->entries[e] == NULL ? empties++ : 0;
		s = this->entries[e];
		printf("[%d]", e);
		while (s != NULL) {
			chain++;
			chains += 1.0;
			printf("->(%s/%d)", s->getLexem(), s->getKey()->getInformation()->getOccurrences());
			s = s->getNext();
		}
		printf("->( )\n");
		// update longest chain
		chain > max_chain ? max_chain = chain : 0;

		// update avg
		chain ? avg = (avg * (chains - 1) + chain) / chains: 0;

		// update quality measure
		red_dragon += (double) (chain * (chain + 1)); // constants are only calculated at the end ...

		// reset
		chain = 0;
	}

	red_dragon /= 2.0;
	red_dragon /= this->string_table->getNodeCount() / 2.0 * this->table_size;
	red_dragon /= this->string_table->getNodeCount() + 2.0 * this->table_size - 1.0;

	printf("\n -- Statistics --\n");
	printf("  Loadfactor          : %f %%\n", Symboltable::LOADFACTOR * 100.0);
	printf("  Expected Loadfactor : %f %%\n", this->string_table->getNodeCount() / (double) this->table_size * 100.0);
	printf("  Actual Loadfactor   : %f %%\n", 100.0 * (double)(this->table_size - empties) / this->table_size);
	printf("  Empty buckets       : %d (%f %%)\n", empties, 100.0 * empties / (double) this->table_size);
	printf("  Longest chain       : %d\n", max_chain);
	printf("  Avg chain           : %f\n", avg);
	printf("  red_dragon quality  : %f (1.0 is perfect)\n", red_dragon);

	printf("\n --- STRING_TABLE: ---\n");
	this->string_table->debugPrint();

	printf("\n");
}
