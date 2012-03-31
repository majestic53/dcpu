/*
 * mem128.cpp
 * Copyright (C) 2012 David Jolly
 * ----------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iomanip>
#include <sstream>
#include "mem128.hpp"

/*
 * Mem constructor
 */
mem128::mem128(void) {
	clear();
}

/*
 * Mem constructor
 */
mem128::mem128(const mem128 &other) {
	memcpy(words, other.words, COUNT);
}

/*
 * Mem constructor
 */
mem128::mem128(const unsigned short (&words)[COUNT]) {
	memcpy(this->words, words, COUNT);
}

/*
 * Mem destructor
 */
mem128::~mem128(void) {
	return;
}

/*
 * Memory assignment operator
 */
mem128 &mem128::operator=(const mem128 &other) {

	// check for self
	if(this == &other)
		return *this;

	// set attributes
	memcpy(words, other.words, COUNT);
	return *this;
}

/*
 * Memory equals operator
 */
bool mem128::operator==(const mem128 &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	for(unsigned short i = 0; i < COUNT; ++i)
		if(words[i] != other.words[i])
			return false;
	return true;
}

/*
 * Mem not-equals operator
 */
bool mem128::operator!=(const mem128 &other) {
	return !(*this == other);
}

/*
 * Return value at offset
 */
unsigned short mem128::at(unsigned short offset) {
	return words[offset];
}

/*
 * Clear mem
 */
void mem128::clear(void) {
	fill_all(ZERO);
}

/*
 * Return a string representation of a given offset and range
 */
std::string mem128::dump(unsigned short offset, unsigned short range) {
	std::stringstream ss;

	// iterate through elements
	for(unsigned short i = 0; i < range; ++i) {
		if(!(i % 16)) {
			if(i)
				ss << std::endl;
			ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << offset << " | ";
			offset += 16;
		}

		// convert each element into hex
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << (unsigned)(unsigned short) words[i] << " ";
	}
	return ss.str();
}

/*
 * Return a string representation of all memory
 */
std::string mem128::dump_all(void) {
	return dump(LOW, HIGH);
}

/*
 * Dump memory to file at a given path
 */
bool mem128::dump_to_file(unsigned short offset, unsigned short range, const std::string &path) {

	// attempt to open file at path
	std::ofstream file(path.c_str(), std::ios::out | std::ios::ate | std::ios::binary);
	if(!file.is_open())
		return false;

	// write memory to file
	for(unsigned short i = 0; i < range; ++i)
		file.write(reinterpret_cast<const char *>(&words[offset + i]), sizeof(unsigned short));
	file.close();
	return true;
}

/*
 * Fill mem from start to end offset with a given value
 */
void mem128::fill(unsigned short offset, unsigned short range, unsigned short value) {
	unsigned short finish = offset + range;

	// assign values
	for(unsigned short i = offset; i < finish; ++i)
		words[i] = value;
}

/*
 * Fill mem with a given value
 */
void mem128::fill_all(unsigned short value) {
	fill(LOW, HIGH, value);
}

/*
 * Set value at offset
 */
void mem128::set(unsigned short offset, unsigned short value) {
	words[offset] = value;
}

/*
 * Set value at offset
 */
void mem128::set(unsigned short offset, unsigned short range, unsigned short *value) {

	// assign values
	for(unsigned short i = 0; i < range; ++i)
		words[i + offset] = value[i];
}
