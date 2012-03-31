/*
 * _register.hpp
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

#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

#include <string>

class _register {
private:

	/*
	 * Value
	 */
	unsigned short value;

public:

	/*
	 * Zero
	 */
	static const unsigned short LOW = 0x0;

	/*
	 * Maximum value
	 */
	static const unsigned short HIGH = 0xFFFF;

	/*
	 * Register constructor
	 */
	_register(void) : value(LOW) { return; }

	/*
	 * Register constructor
	 */
	_register(const _register &other) : value(other.value) { return; }

	/*
	 * Register constructor
	 */
	_register(unsigned short reg) : value(reg) { return; }

	/*
	 * Register destructor
	 */
	virtual ~_register(void) { return; }

	/*
	 * Register assignment operator
	 */
	_register &operator=(const _register &other);

	/*
	 * Register addition operator
	 */
	_register operator+(const _register &other) { return _register(value + other.value); }

	/*
	 * Register subtraction operator
	 */
	_register operator-(const _register &other) { return _register(value - other.value); }

	/*
	 * Register multiplication operator
	 */
	_register operator*(const _register &other) { return _register(value * other.value); }

	/*
	 * Register division operator
	 */
	_register operator/(const _register &other) { return _register(value / other.value); }

	/*
	 * Register left shift operator
	 */
	_register operator>>(const _register &other) { return _register(value >> other.value); }

	/*
	 * Register right shift operator
	 */
	_register operator<<(const _register &other) { return _register(value << other.value); }

	/*
	 * Register modulus operator
	 */
	_register operator%(const _register &other) { return _register(value % other.value); }

	/*
	 * Register binary AND operator
	 */
	_register operator&(const _register &other) { return _register(value & other.value); }

	/*
	 * Register binary OR operator
	 */
	_register operator|(const _register &other) { return _register(value | other.value); }

	/*
	 * Register binary XOR operator
	 */
	_register operator^(const _register &other) { return _register(value ^ other.value); }

	/*
	 * Register logical AND operator
	 */
	bool operator&&(const _register &other) { return value && other.value; }

	/*
	 * Register logical OR operator
	 */
	bool operator||(const _register &other) { return value || other.value; }

	/*
	 * Register unary negaition operator
	 */
	_register operator!(void) { return _register(HIGH - value); }

	/*
	 * Register unary increment operator (prefix)
	 */
	_register operator++(void) { return _register(++value); }

	/*
	 * Register unary increment operator (postfix)
	 */
	_register operator++(int i) { return _register(value++); }

	/*
	 * Register unary decrement operator
	 */
	_register operator--(void) { return _register(--value); }

	/*
	 * Register unary decrement operator (postfix)
	 */
	_register operator--(int i) { return _register(value--); }

	/*
	 * Register equals operator
	 */
	bool operator==(const _register &other);

	/*
	 * Register not-equals operator
	 */
	bool operator!=(const _register &other) { return !(*this == other); }

	/*
	 * Return a bit at a given offset
	 */
	bool bit(unsigned char offset);

	/*
	 * Clear register
	 */
	void clear(void) { value = LOW; }

	/*
	 * Return a string representation of a register
	 */
	std::string dump(void);

	/*
	 * Dump a register to file at a given path
	 */
	bool dump_to_file(const std::string &path);

	/*
	 * Return a register value
	 */
	unsigned short get(void) { return value; }

	/*
	 * Return if a register value is zero
	 */
	bool is_zero(void) { return value == LOW; }

	/*
	 * Set a register value
	 */
	void set(unsigned short value) { this->value = value; }

	/*
	 * Set a bit at a given offset
	 */
	void set_bit(unsigned char offset);
};

#endif
