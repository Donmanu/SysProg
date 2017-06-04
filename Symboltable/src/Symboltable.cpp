/*
 * Symboltable.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Symboltable.h"

/* ------------------- SymTabEntry ---------------------- */

SymTabEntry::SymTabEntry(Key* key) {
	this->key = key;
	this->next = NULL;
}

SymTabEntry::~SymTabEntry() {
	delete this->next; // triggers a destructor chain
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

bool SymTabEntry::compareLexem(const char* lexem) {
	return this->key->getInformation()->compareLexem(lexem);
}

const char* SymTabEntry::getLexem() {
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

Key* Symboltable::insert(const char* lexem) {
	// check parameter and prepare table
	if (lexem == NULL) {
		errno = EINVAL; // invalid arg
		perror("NULL given to Symboltable::insert()");
		exit(EXIT_FAILURE);
	}
	if (this->string_table->getNodeCount() > (int) this->table_size * Symboltable::LOADFACTOR) {
		// we save on a check, if the new size would overload again ...
		this->resize();
		// TODO Minor optimization idea: while resizing (and touching every entry anyway), we could insert lexem directly ...
	}

	// hash
	unsigned int fullHash = this->hash(lexem);
	unsigned int hash = fullHash % this->table_size;
	SymTabEntry* last = this->entries[hash];

	// look up if already contained
	if (last != NULL) { // if slot not empty
		do { // go to end of slot.
			if (last->compareLexem(lexem)) { // On the way: check, if entry already there.
				last->getKey()->getInformation()->incrementOccurrences();
				return last->getKey(); // Return its key, if so.
			} else {
				if (last->hasNext()) {
					last = last->getNext();
					// continue
				} else {
					break;
				}
			}
		} while (true);
	}

	// here we have a new Symbol! current is now pointing to the last entry in the slot, possibly null if empty
	Key* k = new Key(fullHash, new Information(this->string_table->insert(lexem)));
	SymTabEntry* s = new SymTabEntry(k);

	if (last != NULL) {
		// append to end
		last->setNext(s); // new SymTabEntry(new Key(new Information(lexem)), new StringTabNode(lexem))
	} else {
		// init slot
		this->entries[hash] = s;
	}

	return k;
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
void Symboltable::reinsert(SymTabEntry* s) {
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

	//printf("Resizing SymTab %d -> %d\n", previous_table_size, this->table_size);

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
			this->reinsert(entry);
			entry = oldNext;
		}
	}
	delete[] previousEntries; // only the array!
}

unsigned int Symboltable::hash(const char* lexem) {
	// lexem = NULL won't work! Check beforehand!
	unsigned int hash = 0; // SEED = 0
	int i = 0;

	while (lexem[i]) {
		hash = (hash << 6) + (hash << 16) - hash + lexem[i]; // optimized SALT = 65599 = 2^16 + 2^6 - 1
		i++;
	}
	return hash;

	// Jenkins 'One At A Time' Hash:
	/*
	unsigned int hash = 0;
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
	return hash;
	*/
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

	printf("\n--- SYMBOL_TABLE: ---\n");

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
		red_dragon += (double) (chain * (chain + 1)); // constants are calculated only at the end ...

		// reset
		chain = 0;
	}

	red_dragon /= 2.0;
	red_dragon /= this->string_table->getNodeCount() / (2.0 * this->table_size);
	red_dragon /= this->string_table->getNodeCount() + 2.0 * this->table_size - 1.0;

	printf("\n  -- SYMBOL_TABLE, overall statistics --\n");
	printf("  Table size          : %d\n", this->table_size);
	printf("  Loadfactor          : %f %%\n", Symboltable::LOADFACTOR * 100.0);
	printf("  Expected Loadfactor : %f %%\n", this->string_table->getNodeCount() / (double) this->table_size * 100.0);
	printf("  Actual Load(factor) : %d (%f %%)\n", this->table_size - empties , 100.0 * (double)(this->table_size - empties) / this->table_size);
	printf("  Empty buckets       : %d (%f %%)\n", empties, 100.0 * empties / (double) this->table_size);
	printf("  Longest chain       : %d\n", max_chain);
	printf("  Avg chain           : %f\n", avg);
	printf("  red_dragon quality  : %f (1.0 is perfect, higher is better than lower)\n\n", red_dragon);

	this->string_table->debugPrint();

	printf("\n");
}
