/*
 * reg16.hpp
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

#ifndef REG16_HPP_
#define REG16_HPP_

#include <string>

class reg16 {
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
	reg16(void);

	/*
	 * Register constructor
	 */
	reg16(const reg16 &other);

	/*
	 * Register constructor
	 */
	reg16(unsigned short reg);

	/*
	 * Register destructor
	 */
	virtual ~reg16(void);

	/*
	 * Register assignment operator
	 */
	reg16 &operator=(const reg16 &other);

	/*
	 * Register addition operator
	 */
	reg16 operator+(const reg16 &other);

	/*
	 * Register subtraction operator
	 */
	reg16 operator-(const reg16 &other);

	/*
	 * Register multiplication operator
	 */
	reg16 operator*(const reg16 &other);

	/*
	 * Register division operator
	 */
	reg16 operator/(const reg16 &other);

	/*
	 * Register left shift operator
	 */
	reg16 operator>>(const reg16 &other);

	/*
	 * Register right shift operator
	 */
	reg16 operator<<(const reg16 &other);

	/*
	 * Register modulus operator
	 */
	reg16 operator%(const reg16 &other);

	/*
	 * Register binary AND operator
	 */
	reg16 operator&(const reg16 &other);

	/*
	 * Register binary OR operator
	 */
	reg16 operator|(const reg16 &other);

	/*
	 * Register binary XOR operator
	 */
	reg16 operator^(const reg16 &other);

	/*
	 * Register logical AND operator
	 */
	bool operator&&(const reg16 &other);

	/*
	 * Register logical OR operator
	 */
	bool operator||(const reg16 &other);

	/*
	 * Register unary negaition operator
	 */
	reg16 operator!(void);

	/*
	 * Register unary increment operator (prefix)
	 */
	reg16 operator++(void);

	/*
	 * Register unary increment operator (postfix)
	 */
	reg16 operator++(int i);

	/*
	 * Register unary decrement operator
	 */
	reg16 operator--(void);

	/*
	 * Register unary decrement operator (postfix)
	 */
	reg16 operator--(int i);

	/*
	 * Register equals operator
	 */
	bool operator==(const reg16 &other);

	/*
	 * Register not-equals operator
	 */
	bool operator!=(const reg16 &other);

	/*
	 * Return a bit at a given offset
	 */
	bool bit(unsigned char offset);

	/*
	 * Clear register
	 */
	void clear(void);

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
	unsigned short get(void);

	/*
	 * Return if a register value is zero
	 */
	bool is_zero(void);

	/*
	 * Set a register value
	 */
	void set(unsigned short value);

	/*
	 * Set a bit at a given offset
	 */
	void set_bit(unsigned char offset);
};

#endif
