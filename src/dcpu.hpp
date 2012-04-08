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
#include "types.hpp"

class dcpu {
public:

	/*
	 * Main register count
	 */
	static const word M_REG_COUNT = 0x08;

	/*
	 * System register count
	 */
	static const word S_REG_COUNT = 0x03;

	/*
	 * Literal count
	 */
	static const word LIT_COUNT = 0x20;

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
	static const word B_OP_LEN = 0x04;
	static const word NB_OP_LEN = 0x06;
	static const word INPUT_LEN = 0x06;

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
	 * Current state
	 */
	word state;

	/*
	 * Current cycle
	 */
	size_t cycle;

	/*
	 * Add B to A (sets overflow)
	 */
	void _add(word a, word b);

	/*
	 * Binary AND of A and B
	 */
	void _and(word a, word b);

	/*
	 * Binary OR of A and B
	 */
	void _bor(word a, word b);

	/*
	 * Division of A by B (sets overflow)
	 */
	void _div(word a, word b);

	/*
	 * Execute next instruction if ((A & B) != 0)
	 */
	void _ifb(word a, word b);

	/*
	 * Execute next instruction if (A == B)
	 */
	void _ife(word a, word b);

	/*
	 * Execute next instruction if (A > B)
	 */
	void _ifg(word a, word b);

	/*
	 * Execute next instruction if (A != B)
	 */
	void _ifn(word a, word b);

	/*
	 * Push the address of the next word onto the stack
	 */
	void _jsr(word a);

	/*
	 * Modulus of A by B
	 */
	void _mod(word a, word b);

	/*
	 * Multiplication of B from A (sets overflow)
	 */
	void _mul(word a, word b);

	/*
	 * Set A to B
	 */
	void _set(word a, word b);

	/*
	 * Shift-left A by B (sets overflow)
	 */
	void _shl(word a, word b);

	/*
	 * Shift-right A by B (sets overflow)
	 */
	void _shr(word a, word b);

	/*
	 * Subtraction of B from A (sets overflow)
	 */
	void _sub(word a, word b);

	/*
	 * Exclusive-OR of A and B
	 */
	void _xor(word a, word b);

	/*
	 * Execute a single command
	 */
	bool exec(word op);

	/*
	 * Execute a series of commands
	 */
	bool exec(std::vector<word> &op);

	/*
	 * Execute a series of commands starting at offset to range
	 */
	bool exec(word offset, word range, std::vector<word> &op);

	/*
	 * Return an address of a value at a given location
	 */
	word *get_address(word value);

	/*
	 * Return a value held at a given value
	 */
	word get_value(word value);

	/*
	 * Set a value held at a given location
	 */
	void set_value(word *location, word value);

	/*
	 * Perform a state change
	 */
	bool state_change(word state);

public:

	/*
	 * Main registers
	 */
	enum M_REG { A, B, C, X, Y, Z, I, J };

	/*
	 * System registers
	 */
	enum S_REG { SP, PC, OVERFLOW };

	/*
	 * Supported basic opcodes
	 */
	enum B_OP { NB, SET, ADD, SUB, MUL, DIV, MOD, SHL, SHR,
		AND, BOR, XOR, IFE, IFN, IFG, IFB };

	/*
	 * Supported non-basic opcodes
	 */
	enum NB_OP { RES, JSR };

	/*
	 * States
	 */
	enum STATE { INIT, RUN, HALT };

	/*
	 * Values types
	 */
	enum VALUE { L_REG, H_REG = 0x07, L_VAL, H_VAL = 0x0F, L_OFF, H_OFF = 0x17,
		POP, PEEK, PUSH, SP_VAL, PC_VAL, OVER_F, ADR_OFF, LIT_OFF,
		L_LIT = 0x20, H_LIT = 0x3F };

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
	dcpu(const reg16 (&m_reg)[M_REG_COUNT], const reg16 (&s_reg)[S_REG_COUNT], const mem128 &mem, word state, size_t cycle);

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
	 * Returns a Cpu cycle count
	 */
	size_t cycles(void);

	/*
	 * Return a string representation of a cpu
	 */
	std::string dump(void);

	/*
	 * Dump cpu to file at a givne path
	 */
	bool dump_to_file(const std::string &path);

	/*
	 * Halt a Cpu
	 */
	bool halt(void);

	/*
	 * Returns a Cpu running status
	 */
	bool is_running(void);

	/*
	 * Return a main register
	 */
	reg16 &m_register(word reg);

	/*
	 * Return memory
	 */
	mem128 &memory(void);

	/*
	 * Reset cpu
	 */
	void reset(void);

	/*
	 * Run a Cpu
	 */
	bool run(void);

	/*
	 * Return a system register
	 */
	reg16 &s_register(word reg);
};

#endif
