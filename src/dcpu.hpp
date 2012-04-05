/*
 * dcpu.hpp
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

#ifndef DCPU_HPP_
#define DCPU_HPP_

#include <string>
#include <vector>
#include "mem128.hpp"
#include "reg16.hpp"

class dcpu {
public:

	/*
	 * Main register count
	 */
	static const unsigned char M_REG_COUNT = 0x08;

	/*
	 * System register count
	 */
	static const unsigned char S_REG_COUNT = 0x03;

	/*
	 * Literal count
	 */
	static const unsigned char LIT_COUNT = 0x20;

	/*
	 * Basic opcode section lengths
	 *
	 * 	--------------------------
	 * 	| BBBBBB | AAAAAA | OOOO |
	 * 	--------------------------
	 *
	 * 0x00	   0x06		0x0C	0x16
	 *
	 *
	 * Non-Basic opcode section lengths
	 *
	 * 	--------------------------
	 * 	| AAAAAA | oooooo | 0000 |
	 * 	--------------------------
	 *
	 * 0x00	   0x06		0x0C	0x16
	 *
	 */
	static const unsigned char B_OP_LEN = 0x04;
	static const unsigned char NB_OP_LEN = 0x06;
	static const unsigned char INPUT_LEN = 0x06;

private:

	/*
	 * Main registers (A - J)
	 */
	reg16 m_reg[M_REG_COUNT];

	/*
	 * System registers (PC - Overflow)
	 */
	reg16 s_reg[S_REG_COUNT];

	/*
	 * Memory (128Kb)
	 */
	mem128 mem;

	/*
	 * Add B to A (sets overflow)
	 */
	void _add(unsigned short a, unsigned short b);

	/*
	 * Binary AND of A and B
	 */
	void _and(unsigned short a, unsigned short b);

	/*
	 * Binary OR of A and B
	 */
	void _bor(unsigned short a, unsigned short b);

	/*
	 * Division of A by B (sets overflow)
	 */
	void _div(unsigned short a, unsigned short b);

	/*
	 * Return a value held at a given location
	 */
	unsigned short get_value(unsigned short location);

	/*
	 * Execute next instruction if ((A & B) != 0)
	 */
	void _ifb(unsigned short a, unsigned short b);

	/*
	 * Execute next instruction if (A == B)
	 */
	void _ife(unsigned short a, unsigned short b);

	/*
	 * Execute next instruction if (A > B)
	 */
	void _ifg(unsigned short a, unsigned short b);

	/*
	 * Execute next instruction if (A != B)
	 */
	void _ifn(unsigned short a, unsigned short b);

	/*
	 * Push the address of the next word onto the stack
	 */
	void _jsr(unsigned short a);

	/*
	 * Modulus of A by B
	 */
	void _mod(unsigned short a, unsigned short b);

	/*
	 * Multiplication of B from A (sets overflow)
	 */
	void _mul(unsigned short a, unsigned short b);

	/*
	 * Set A to B
	 */
	void _set(unsigned short a, unsigned short b);

	/*
	 * Set a value held at a given location
	 */
	void set_value(unsigned short location, unsigned short value);

	/*
	 * Shift-left A by B (sets overflow)
	 */
	void _shl(unsigned short a, unsigned short b);

	/*
	 * Shift-right A by B (sets overflow)
	 */
	void _shr(unsigned short a, unsigned short b);

	/*
	 * Subtraction of B from A (sets overflow)
	 */
	void _sub(unsigned short a, unsigned short b);

	/*
	 * Exclusive-OR of A and B
	 */
	void _xor(unsigned short a, unsigned short b);

public:

	/*
	 * Main registers
	 */
	enum M_REG { A, B, C, X, Y, Z, I, J };

	/*
	 * System registers
	 */
	enum S_REG { PC, SP, OVERFLOW };

	/*
	 * Supported basic opcodes
	 */
	enum B_OP { NB, SET, ADD, SUB, MUL, DIV, MOD, SHL, SHR,
		AND, BOR, XOR, IFE, IFN, IFG, IFB };

	/*
	 * Supported non-basic opcodes
	 */
	enum NB_OP { JSR = 0x01 };

	/*
	 * Reserved values
	 */
	enum SPEC { L_REG, H_REG = 0x07, L_VAL, H_VAL = 0x0F, L_OFF, H_OFF = 0x17,
		POP, PEEK, PUSH, SP_VAL, PC_VAL, OVER_F, ADR_OFF, LIT_OFF,
		L_LIT = 0x20, H_LIT = 0x3F };

	/*
	 * Overflow
	 */
	enum OVER { NOT_OVFLOW = 0x0, L_OVFLOW = 0x1, H_OVFLOW = 0xFFFF };

	/*
	 * Cpu constructor
	 */
	dcpu(void);

	/*
	 * Cpu constructor
	 */
	dcpu(const dcpu &other);

	/*
	 * Cpu constructor
	 */
	dcpu(const mem128 &mem);

	/*
	 * Cpu constructor
	 */
	dcpu(const reg16 (&m_reg)[M_REG_COUNT], const reg16 (&s_reg)[S_REG_COUNT], const mem128 &mem);

	/*
	 * Cpu destructor
	 */
	virtual ~dcpu(void);

	/*
	 * Cpu assignment operator
	 */
	dcpu &operator=(const dcpu &other);

	/*
	 * Cpu equals operator
	 */
	bool operator==(const dcpu &other);

	/*
	 * Cpu not-equals operator
	 */
	bool operator!=(const dcpu &other);

	/*
	 * Return a string representation of a cpu
	 */
	std::string dump(void);

	/*
	 * Dump cpu to file at a givne path
	 */
	bool dump_to_file(const std::string &path);

	/*
	 * Execute a single command
	 */
	bool exec(unsigned short op);

	/*
	 * Execute a series of commands
	 */
	bool exec(std::vector<unsigned short> &op);

	/*
	 * Execute a series of commands starting at offset to range
	 */
	bool exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &op);

	/*
	 * Return a main register
	 */
	reg16 &m_register(unsigned char reg);

	/*
	 * Return memory
	 */
	mem128 &memory(void);

	/*
	 * Reset cpu
	 */
	void reset(void);

	/*
	 * Return a system register
	 */
	reg16 &s_register(unsigned char reg);
};

#endif
