/*
 * _memory.hpp
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

#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <cstring>
#include <string>

class _memory {
public:

	/*
	 * Zero
	 */
	static const unsigned short ZERO = 0x0;

	/*
	 * Word count
	 */
	static const unsigned int COUNT = 0x10000;

	/*
	 * Word boundaries
	 */
	static const unsigned short LOW = 0x0;
	static const unsigned short HIGH = COUNT - 1;

private:

	/*
	 * Words
	 */
	unsigned short words[COUNT];

public:

	/*
	 * Mem constructor
	 */
	_memory(void) { clear(); }

	/*
	 * Mem constructor
	 */
	_memory(const _memory &other) { memcpy(words, other.words, COUNT); }

	/*
	 * Mem constructor
	 */
	_memory(const unsigned short (&words)[COUNT]) { memcpy(this->words, words, COUNT); }

	/*
	 * Mem destructor
	 */
	virtual ~_memory(void) { return; }

	/*
	 * Mem assignment operator
	 */
	_memory &operator=(const _memory &other);

	/*
	 * Mem equals operator
	 */
	bool operator==(const _memory &other);

	/*
	 * Mem not-equals operator
	 */
	bool operator!=(const _memory &other) { return !(*this == other); }

	/*
	 * Return value at offset
	 */
	unsigned short at(unsigned short offset) { return words[offset]; }

	/*
	 * Clear mem
	 */
	void clear(void) { fill_all(ZERO); }

	/*
	 * Return a string representation of a given offset and range
	 */
	std::string dump(unsigned short offset, unsigned short range);

	/*
	 * Return a string representation of all memory
	 */
	std::string dump_all(void) { return dump(LOW, HIGH); }

	/*
	 * Dump memory to file at a given path
	 */
	bool dump_to_file(unsigned short offset, unsigned short range, const std::string &path);

	/*
	 * Fill mem from offset to offset and range offset with a given value
	 */
	void fill(unsigned short offset, unsigned short range, unsigned short value);

	/*
	 * Fill mem with a given value
	 */
	void fill_all(unsigned short value) { fill(LOW, HIGH, value); }

	/*
	 * Set value at offset
	 */
	void set(unsigned short offset, unsigned short value) { words[offset] = value; }
};

#endif
