/*
 * _cpu.hpp
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

#ifndef CPU_HPP_
#define CPU_HPP_

#include <string>
#include <vector>
#include "_memory.hpp"
#include "_register.hpp"

class _cpu {
public:

	/*
	 * Main register count
	 */
	static const unsigned char M_REG_COUNT = 7;

	/*
	 * System register count
	 */
	static const unsigned char S_REG_COUNT = 3;

	/*
	 * Opt-code section lengths
	 *
	 * 	--------------------------
	 * 	| BBBBBB | AAAAAA | OOOO |
	 * 	--------------------------
	 *
	 * 0x00	   0x06		0x0C	0x16
	 *
	 */
	static const unsigned char OP_LEN = 4;
	static const unsigned char INPUT_LEN = 6;

private:

	/*
	 * Main registers (A - J)
	 */
	_register m_reg[M_REG_COUNT];

	/*
	 * System registers (PC - Overflow)
	 */
	_register s_reg[S_REG_COUNT];

	/*
	 * Memory (128Kb)
	 */
	_memory mem;

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
	 * Jump one instruction if !(A & B)
	 */
	void _ifb(unsigned short a, unsigned short b);

	/*
	 * Jump one instruction if (A != B)
	 */
	void _ife(unsigned short a, unsigned short b);

	/*
	 * Jump one instruction if (A <= B)
	 */
	void _ifg(unsigned short a, unsigned short b);

	/*
	 * Jump one instruction if (A == B)
	 */
	void _ifn(unsigned short a, unsigned short b);

	/*
	 * Modulus of A by B
	 */
	void _mod(unsigned short a, unsigned short b);

	/*
	 * Multiplication of B from A (sets overflow)
	 */
	void _mul(unsigned short a, unsigned short b);

	/*
	 * Set B to A
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
	enum M_REG { A, B, C, X, Y, I, J };

	/*
	 * System registers
	 */
	enum S_REG { PC, SP, OVERFLOW };

	/*
	 * Supported opt-codes
	 */
	enum OPT { SET = 1, ADD, SUB, MUL, DIV, MOD, SHL, SHR,
		AND, BOR, XOR, IFE, IFN, IFG, IFB };

	/*
	 * Reserved values
	 */
	enum SPEC { L_REG, H_REG = 7, L_VAL, H_VAL = 15, L_OFF, H_OFF = 23,
		POP, PEEK, PUSH, SP_VAL, PC_VAL, OVER_F, ADR_OFF, LIT_OFF,
		L_LIT = 32, H_LIT = 63 };

	/*
	 * Cpu constructor
	 */
	_cpu(void) { reset(); }

	/*
	 * Cpu constructor
	 */
	_cpu(const _cpu &other) : m_reg(other.m_reg), s_reg(other.s_reg), mem(other.mem) { return; }

	/*
	 * Cpu constructor
	 */
	_cpu(const _memory &mem) : mem(mem) { reset_registers(); }

	/*
	 * Cpu constructor
	 */
	_cpu(const _register (&m_reg)[M_REG_COUNT], const _register (&s_reg)[S_REG_COUNT], const _memory &mem) : m_reg(m_reg),
			s_reg(s_reg), mem(mem) { return; }

	/*
	 * Cpu destructor
	 */
	virtual ~_cpu(void) { return; }

	/*
	 * Cpu assignment operator
	 */
	_cpu &operator=(const _cpu &other);

	/*
	 * Cpu equals operator
	 */
	bool operator==(const _cpu &other);

	/*
	 * Cpu not-equals operator
	 */
	bool operator!=(const _cpu &other) { return !(*this == other); }

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
	bool exec(unsigned short opt);

	/*
	 * Execute a series of commands
	 */
	bool exec(std::vector<unsigned short> &opt) { return exec(0, opt.size(), opt); }

	/*
	 * Execute a series of commands starting at offset to range
	 */
	bool exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &opt);

	/*
	 * Return a main register
	 */
	_register &m_register(unsigned char reg) { return m_reg[reg]; }

	/*
	 * Return memory
	 */
	_memory &memory(void) { return mem; }

	/*
	 * Reset cpu
	 */
	void reset(void);

	/*
	 * Reset all registers
	 */
	void reset_registers(void);

	/*
	 * Return a system register
	 */
	_register &s_register(unsigned char reg) { return s_reg[reg]; }
};

#endif
