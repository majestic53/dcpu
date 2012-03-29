/*
 * mem.cpp
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
#include "_memory.hpp"

/*
 * Memory assignment operator
 */
_memory &_memory::operator=(const _memory &other) {

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
bool _memory::operator==(const _memory &other) {

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
 * Return a string representation of a given offset and range
 */
std::string _memory::dump(unsigned short offset, unsigned short range) {
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
 * Dump memory to file at a given path
 */
bool _memory::dump_to_file(unsigned short offset, unsigned short range, const std::string &path) {

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
void _memory::fill(unsigned short offset, unsigned short range, unsigned short value) {
	unsigned short finish = offset + range;

	// assign values
	for(unsigned short i = offset; i < finish; ++i)
		words[i] = value;
}
