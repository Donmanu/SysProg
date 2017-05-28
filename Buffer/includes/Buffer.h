/*
 * Buffer.h
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>

class Buffer {
public:
	Buffer(char*);
	virtual ~Buffer();
	char getChar();
	char ungetChar();

private:
	void allocateMemory(char **buffer);
	void openFile(char *file_name);
	void readFile(char **buffer);
	void swapBuffer();

	static const size_t BUFFER_SIZE = 512;
	static const size_t ALIGNMENT = 512;
	char *buffer_current;
	char *buffer_previous;
	ssize_t bytes_read;
	char current_char;
	int file_handle;
	unsigned int position;
	bool buffer_swapped_back;
};

#endif /* BUFFER_H_ */
