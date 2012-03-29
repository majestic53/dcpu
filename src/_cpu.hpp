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
	 * Cpu constructor
	 */
	_cpu(void);

	/*
	 * Cpu constructor
	 */
	_cpu(const _cpu &other);

	/*
	 * Cpu constructor
	 */
	_cpu(const _memory &mem);

	/*
	 * Cpu constructor
	 */
	_cpu(const _register (&m_reg)[M_REG_COUNT], const _register (&s_reg)[S_REG_COUNT], const _memory &mem);

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
	void exec(unsigned short opt);

	/*
	 * Execute a series of commands
	 */
	void exec(std::vector<unsigned short> &opt);

	/*
	 * Execute a series of commands starting at offset to range
	 */
	void exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &opt);

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
	 * Return a system register
	 */
	_register &s_register(unsigned char reg) { return s_reg[reg]; }
};

#endif
