/*
 * Buffer.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: knad0001
 */

#include "../includes/Buffer.h"

/*
 * The constructor takes a file as input parameter,
 * initializes the variables and calls the allocateMemory()
 * functions for both buffers. Then calls the openFile()
 * and readFile() functions.
 *
 * @input: the name of the file to be opened.
 */
Buffer::Buffer(char *file_name) {
	this->buffer_current = NULL;
	this->buffer_previous = NULL;
	this->current_char = '\0';
	this->bytes_read = 0;
	this->file_handle = 0;
	this->position = 0;
	this->buffer_swapped_back = true;       // regard buffers as if back-swapped initially!
	this->allocateMemory(& this->buffer_current);
	this->allocateMemory(& this->buffer_previous);
	this->openFile(file_name);
	this->readFile(& this->buffer_current);
	// as we start 'buffer_swapped_back', we also read next chunk, if there is something
	if (this->bytes_read == (int) Buffer::BUFFER_SIZE)
		this->readFile(& this->buffer_previous);
}

/*
 * the destructor deletes the two buffers
 * and closes the file
 */
Buffer::~Buffer() {
	//printf("[B] freeing memory and closing file ...\n");
	free(this->buffer_current); // the counterpart to posix_memalign() is free(), not delete[]
	free(this->buffer_previous);
	close(this->file_handle);
}

/*
 * this function takes the address of a buffer as input
 * parameter and allocates memory of the size{BUFFER_SIZE}
 * with the posix_memalign method.
 *
 * @input: the address of the buffer.
 * @error: memory could not be allocated.
 */
void Buffer::allocateMemory(char **buffer) {
	//printf("[B] allocating memory ...\n");
	errno = posix_memalign((void**) buffer, Buffer::ALIGNMENT, Buffer::BUFFER_SIZE);
	if (errno) { // EINVAL (error: invalid alignment, not a power of 2) or ENOMEM (error: no memory)
		perror("Error allocating memory!");
		throw errno;
	} else {
		// not really needed, but it silents valgrind in regards to "uninitialized value(s)"-errors
		memset(*buffer, 0, Buffer::BUFFER_SIZE);
	}
}

/*
 * this function takes a filename{file_name} as input
 * parameter and tries to open this file and saves the
 * result in the file handle{file_handle}.
 *
 * @input: the file to be opened.
 * @error: file could not be opened.
 */
void Buffer::openFile(char *file_name) {
	//printf("[B] opening file %s ...\n", file_name);
	this->file_handle = open(file_name, O_DIRECT);
	if (this->file_handle == -1) {
		errno = ENOENT; // error: no entity
		perror("Error opening File!\n");
		throw errno;
	}
}

/*
 * this function takes the address of a buffer as input
 * parameter and reads the next block of characters from
 * the file and stores them in the input buffer.
 *
 * @input: the address of the buffer.
 * @error: file could not be read.
 */
void Buffer::readFile(char **buffer) {
	//printf("[B] reading file ...\n");
	this->bytes_read = read(this->file_handle, *buffer, Buffer::BUFFER_SIZE); // should actually always be buffer_current
	if (bytes_read == -1) {
		errno = EBADFD; // error: bad file descriptor
		perror("[B] Error reading File!\n");
		throw errno;
	}
}

/*
 * this method increments the position{position} for
 * each call and returns the current character{current_char}
 *
 * If needed, the buffers will be swapped and the previous buffer
 * will be overwritten with the next block of characters
 * from the file.
 *
 * @return: current character{current_char}
 */
char Buffer::getChar() {
	if (this->position >= Buffer::BUFFER_SIZE) {
		if (buffer_swapped_back) {
			// just swap without new file read
			this->swapBuffer();
		} else {
			// swap with new file read
			this->swapBuffer();
			memset(this->buffer_current, 0, Buffer::BUFFER_SIZE);
			this->readFile(& this->buffer_current);
		}
		this->position = 0;
	}
	this->current_char = this->buffer_current[this->position++];
	return this->current_char;
}

/*
 * this function decrements the position{position}
 * or sets the previous buffer{buffer_previous} as the
 * current buffer{buffer_current} if the position{position}
 * is zero. Throws an error if the buffer was already
 * swapped and the buffer would have to be swapped again.
 */
char Buffer::ungetChar() {
	if (this->position > 0) {
		this->position--;
	} else if (!this->buffer_swapped_back) {
		this->swapBuffer();
		this->buffer_swapped_back = true;
		this->position = Buffer::BUFFER_SIZE - 1;
	} else {
		printf("[B] Error!\tCurrent char: '%c'.\tCurrent position: %d\n", this->buffer_current[this->position], this->position);
		errno = ENOBUFS; // error: no buffer space
		perror("[B] can't go back two buffers");
		throw errno;
	}

	return buffer_current[this->position]; // if anybody is interested ...
}

/*
 * this function sets the current buffer{buffer_current}
 * as the previous buffer{buffer_previous} and sets
 * the position{position} to zero and the flag that
 * indicates if the buffer had to be swapped due to
 * ungetChar() to false.
 */
void Buffer::swapBuffer() {
	//printf("[B] Swapping buffers %s ...\n", buffer_swapped_back ? "backwards" : "forwards"); // Un-Swapping or Swapping?
	// the print above is wrong the first time!
	char *buffer_temp = this->buffer_previous;
	this->buffer_previous = this->buffer_current;
	this->buffer_current = buffer_temp;
	this->buffer_swapped_back = false;
}
