/*
 * _register.cpp
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
#include "_register.hpp"

/*
 * Register assignment operator
 */
_register &_register::operator=(const _register &other) {

	// check for self
	if(this == &other)
		return *this;

	// set attributes
	value = other.value;
	return *this;
}

/*
 * Register equals operator
 */
bool _register::operator==(const _register &other) {

	// check for self
	if(this == &other)
		return true;

	// set attributes
	return value == other.value;
}

/*
 * Return a bit at a given offset
 */
bool _register::bit(unsigned char offset) {

	// check offset
	if(offset >= 0x8)
		return false;
	return value & (1 << offset);
}

/*
 * Return a string representation of a register
 */
std::string _register::dump(void) {
	std::stringstream ss;

	// convert element into hex
	ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << (unsigned)(unsigned short) value;
	return ss.str();
}

/*
 * Dump memory to file at a given path
 */
bool _register::dump_to_file(const std::string &path) {

	// attempt to open file at path
	std::ofstream file(path.c_str(), std::ios::out | std::ios::ate | std::ios::binary);
	if(!file.is_open())
		return false;

	// write memory to file
	file.write(reinterpret_cast<const char *>(&value), sizeof(unsigned short));
	file.close();
	return true;
}

/*
 * Set a bit at a given offset
 */
void _register::set_bit(unsigned char offset) {

	// check offset
	if(offset >= 0x8)
		return;
	value ^= (1 << offset);
}
