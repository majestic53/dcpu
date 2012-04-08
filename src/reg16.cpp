/*
 * reg16.cpp
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
#include "reg16.hpp"

/*
 * Register constructor
 */
reg16::reg16(void) : value(LOW) {
	return;
}

/*
 * Register constructor
 */
reg16::reg16(const reg16 &other) : value(other.value) {
	return;
}

/*
 * Register constructor
 */
reg16::reg16(word reg) : value(reg) {
	return;
}

/*
 * Register destructor
 */
reg16::~reg16(void) {
	return;
}

/*
 * Register assignment operator
 */
reg16 &reg16::operator=(const reg16 &other) {

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
bool reg16::operator==(const reg16 &other) {

	// check for self
	if(this == &other)
		return true;

	// set attributes
	return value == other.value;
}

/*
 * Register not-equals operator
 */
bool reg16::operator!=(const reg16 &other) {
	return !(*this == other);
}

/*
 * Register addition operator
 */
reg16 reg16::operator+(const reg16 &other) {
	return reg16(value + other.value);
}

/*
 * Register subtraction operator
 */
reg16 reg16::operator-(const reg16 &other) {
	return reg16(value - other.value);
}

/*
 * Register multiplication operator
 */
reg16 reg16::operator*(const reg16 &other) {
	return reg16(value * other.value);
}

/*
 * Register division operator
 */
reg16 reg16::operator/(const reg16 &other) {
	return reg16(value / other.value);
}

/*
 * Register left shift operator
 */
reg16 reg16::operator>>(const reg16 &other) {
	return reg16(value >> other.value);
}

/*
 * Register right shift operator
 */
reg16 reg16::operator<<(const reg16 &other) {
	return reg16(value << other.value);
}

/*
 * Register modulus operator
 */
reg16 reg16::operator%(const reg16 &other) {
	return reg16(value % other.value);
}

/*
 * Register binary AND operator
 */
reg16 reg16::operator&(const reg16 &other) {
	return reg16(value & other.value);
}

/*
 * Register binary OR operator
 */
reg16 reg16::operator|(const reg16 &other) {
	return reg16(value | other.value);
}

/*
 * Register binary XOR operator
 */
reg16 reg16::operator^(const reg16 &other) {
	return reg16(value ^ other.value);
}

/*
 * Register logical AND operator
 */
bool reg16::operator&&(const reg16 &other) {
	return value && other.value;
}

/*
 * Register logical OR operator
 */
bool reg16::operator||(const reg16 &other) {
	return value || other.value;
}

/*
 * Register unary negaition operator
 */
reg16 reg16::operator!(void) {
	return reg16(HIGH - value);
}

/*
 * Register unary increment operator (prefix)
 */
reg16 reg16::operator++(void) {
	return reg16(++value);
}

/*
 * Register unary increment operator (postfix)
 */
reg16 reg16::operator++(int i) {
	return reg16(value++);
}

/*
 * Register unary decrement operator
 */
reg16 reg16::operator--(void) {
	return reg16(--value);
}

/*
 * Register unary decrement operator (postfix)
 */
reg16 reg16::operator--(int i) {
	return reg16(value--);
}

/*
 * Return a bit at a given offset
 */
bool reg16::bit(word offset) {

	// check offset
	if(offset >= 0x8)
		return false;
	return value & (1 << offset);
}

/*
 * Clear register
 */
void reg16::clear(void) {
	value = LOW;
}

/*
 * Return a string representation of a register
 */
std::string reg16::dump(void) {
	std::stringstream ss;

	// convert element into hex
	ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << (unsigned)(word) value;
	return ss.str();
}

/*
 * Dump memory to file at a given path
 */
bool reg16::dump_to_file(const std::string &path) {

	// attempt to open file at path
	std::ofstream file(path.c_str(), std::ios::out | std::ios::ate | std::ios::binary);
	if(!file.is_open())
		return false;

	// write memory to file
	file.write(reinterpret_cast<const char *>(&value), sizeof(word));
	file.close();
	return true;
}

/*
 * Return a register value
 */
word &reg16::get(void) {
	return value;
}

/*
 * Return if a register value is zero
 */
bool reg16::is_zero(void) {
	return value == LOW;
}

/*
 * Set a register value
 */
void reg16::set(word value) {
	this->value = value;
}

/*
 * Set a bit at a given offset
 */
void reg16::set_bit(word offset) {

	// check offset
	if(offset >= 0x8)
		return;
	value ^= (1 << offset);
}
