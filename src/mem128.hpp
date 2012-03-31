/*
 * mem128.hpp
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

#ifndef MEM128_HPP_
#define MEM128_HPP_

#include <cstring>
#include <string>

class mem128 {
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
	mem128(void);

	/*
	 * Mem constructor
	 */
	mem128(const mem128 &other);

	/*
	 * Mem constructor
	 */
	mem128(const unsigned short (&words)[COUNT]);

	/*
	 * Mem destructor
	 */
	virtual ~mem128(void);

	/*
	 * Mem assignment operator
	 */
	mem128 &operator=(const mem128 &other);

	/*
	 * Mem equals operator
	 */
	bool operator==(const mem128 &other);

	/*
	 * Mem not-equals operator
	 */
	bool operator!=(const mem128 &other);

	/*
	 * Return value at offset
	 */
	unsigned short at(unsigned short offset);

	/*
	 * Clear mem
	 */
	void clear(void);

	/*
	 * Return a string representation of a given offset and range
	 */
	std::string dump(unsigned short offset, unsigned short range);

	/*
	 * Return a string representation of all memory
	 */
	std::string dump_all(void);

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
	void fill_all(unsigned short value);

	/*
	 * Set value at offset
	 */
	void set(unsigned short offset, unsigned short value);

	/*
	 * Set value at offset
	 */
	void set(unsigned short offset, unsigned short range, unsigned short *value);
};

#endif
