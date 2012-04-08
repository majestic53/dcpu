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
#include "types.hpp"

class mem128 {
private:

	/*
	 * Words
	 */
	word words[COUNT];

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
	mem128(const word (&words)[COUNT]);

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
	word &at(word offset);

	/*
	 * Clear mem
	 */
	void clear(void);

	/*
	 * Return a string representation of a given offset and range
	 */
	std::string dump(word offset, word range);

	/*
	 * Return a string representation of all memory
	 */
	std::string dump_all(void);

	/*
	 * Dump memory to file at a given path
	 */
	bool dump_to_file(word offset, word range, const std::string &path);

	/*
	 * Fill mem from offset to offset and range offset with a given value
	 */
	void fill(word offset, word range, word value);

	/*
	 * Fill mem with a given value
	 */
	void fill_all(word value);

	/*
	 * Set value at offset
	 */
	void set(word offset, word value);

	/*
	 * Set value at offset
	 */
	void set(word offset, word range, word *value);
};

#endif
