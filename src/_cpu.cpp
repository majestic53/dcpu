/*
 * _cpu.cpp
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

#include <sstream>
#include "_cpu.hpp"

/*
 * Cpu assignment operator
 */
_cpu &_cpu::operator=(const _cpu &other) {

	// check for self
	if(this == &other)
		return *this;

	// set attributes
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		m_reg[i] == other.m_reg[i];
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		s_reg[i] == other.s_reg[i];
	mem == other.mem;
	return *this;
}

/*
 * Cpu equals operator
 */
bool _cpu::operator==(const _cpu &other) {

	// check for self
	if(this == &other)
		return true;

	// check attributes
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		if(m_reg[i] != other.m_reg[i])
			return false;
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		if(s_reg[i] != other.s_reg[i])
			return false;
	return mem == other.mem;
}

/*
 * Add B to A (sets overflow)
 */
void _cpu::_add(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Binary AND of A and B
 */
void _cpu::_and(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Binary OR of A and B
 */
void _cpu::_bor(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Division of A by B (sets overflow)
 */
void _cpu::_div(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Return a string representation of a cpu
 */
std::string _cpu::dump(void) {
	std::stringstream ss;

	// print all system registers
	ss << "S_REG { ";
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		ss << s_reg[i].dump() << ", ";


	// print all main registers
	ss << "}" << std::endl << "M_REG { ";
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		ss << m_reg[i].dump() << ", ";
	ss << "}" << std::endl << "--MEM--" << std::endl;

	// print memory
	ss << mem.dump(_memory::LOW, _memory::HIGH);
	return ss.str();
}

/*
 * Dump cpu to file at a givne path
 */
bool _cpu::dump_to_file(const std::string &path) {

	// write system registers to file
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		if(!s_reg[i].dump_to_file(path))
			return false;

	// write main registers to file
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		if(!m_reg[i].dump_to_file(path))
			return false;

	// write memory to file
	return mem.dump_to_file(_memory::LOW, _memory::HIGH, path);
}

/*
 * Execute a single command
 */
bool _cpu::exec(unsigned short opt) {
	unsigned char code = 0;
	unsigned short a = 0, b = 0;

	// parse opt-code, A & B from opt
	for(size_t i = 0; i < 8 * sizeof(short); ++i)

		// parse code
		if(i < OP_LEN) {
			if(opt & (1 << i))
				code |= (1 << i);

		// parse A
		} else if(i > OP_LEN && i < OP_LEN + INPUT_LEN) {
			if(opt & (1 << i))
				a |= (1 << (i - OP_LEN));

		// parse B
		} else {
			if(opt & (1 << i))
				b |= (1 << (i - (OP_LEN + INPUT_LEN)));
		}

	// execute command based on code
	switch(code) {
		case SET: _set(a, b);
			break;
		case ADD: _add(a, b);
			break;
		case SUB: _sub(a, b);
			break;
		case MUL: _mul(a, b);
			break;
		case DIV: _div(a, b);
			break;
		case MOD: _mod(a, b);
			break;
		case SHL: _shl(a, b);
			break;
		case SHR: _shr(a, b);
			break;
		case AND: _and(a, b);
			break;
		case BOR: _bor(a, b);
			break;
		case XOR: _xor(a, b);
			break;
		case IFE: _ife(a, b);
			break;
		case IFN: _ifn(a, b);
			break;
		case IFG: _ifg(a, b);
			break;
		case IFB: _ifb(a, b);
			break;
		default: return false;
	}
	return true;
}

/*
 * Execute a series of commands starting at offset to range
 */
bool _cpu::exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &opt) {

	// execute all commands
	for(unsigned short i = 0; i < range; ++i)
		if(!exec(opt.at(offset + i)))
			return false;
	return true;
}

/*
 * Return a value held at a given location
 */
unsigned short _cpu::get_value(unsigned short location) {

	// register value
	if(location >= L_REG && location <= H_REG)
		return m_reg[location].get();

	// value at address in register
	else if(location >= L_VAL && location <= H_VAL)
		return mem.at(m_reg[location % M_REG_COUNT].get());

	// value at address ((PC + 1) + register value)
	else if(location >= L_OFF && location <= H_OFF)
		return mem.at(mem.at(s_reg[PC].get() + 1) + m_reg[location % M_REG_COUNT].get());

	// value at address in SP and increment SP
	else if(location == POP)
		return mem.at(s_reg[SP]++.get());

	// value at address in SP
	else if(location == PEEK)
		return mem.at(s_reg[SP].get());

	// value in SP
	else if(location == SP_VAL)
		return s_reg[SP].get();

	// value in PC
	else if(location == PC_VAL)
		return s_reg[PC].get();

	// value in overflow
	else if(location == OVER_F)
		return s_reg[OVERFLOW].get();

	// value of address at PC + 1
	else if(location == ADR_OFF)
		return mem.at(s_reg[PC].get() + 1);

	// value of PC + 1
	else if(location == LIT_OFF)
		return s_reg[PC].get() + 1;

	// Literal value from 0 - 31
	else if(location >= L_LIT && location <= H_LIT)
		return location % LIT_COUNT;
	return 0;
}

/*
 * Jump one instruction if !(A & B)
 */
void _cpu::_ifb(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Jump one instruction if (A != B)
 */
void _cpu::_ife(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Jump one instruction if (A <= B)
 */
void _cpu::_ifg(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Jump one instruction if (A == B)
 */
void _cpu::_ifn(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Modulus of A by B
 */
void _cpu::_mod(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Multiplication of B from A (sets overflow)
 */
void _cpu::_mul(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Reset cpu
 */
void _cpu::reset(void) {

	// reset all components
	reset_registers();
	mem.clear();
}

/*
 * Reset all registers
 */
void _cpu::reset_registers(void) {

	// clear main registers
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		m_reg[i].clear();

	// clear system registers
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		s_reg[i].clear();
}

/*
 * Set B to A
 */
void _cpu::_set(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Set a value held at a given location
 */
void _cpu::set_value(unsigned short location, unsigned short value) {

	// register value
	if(location >= L_REG && location <= H_REG)
		m_reg[location].set(value);

	// value at address in register
	else if(location >= L_VAL && location <= H_VAL)
		mem.set(m_reg[location % M_REG_COUNT].get(), value);

	// value at address ((PC + 1) + register value)
	else if(location >= L_OFF && location <= H_OFF)
		mem.set(mem.at(s_reg[PC].get() + 1) + m_reg[location % M_REG_COUNT].get(), value);

	// value at address in SP
	else if(location == PUSH)
		mem.set((--s_reg[SP]).get(), value);

	// value in SP
	else if(location == SP_VAL)
		s_reg[SP].set(value);

	// value in PC
	else if(location == PC_VAL)
		s_reg[PC].set(value);

	// value in overflow
	else if(location == OVER_F)
		s_reg[OVERFLOW].set(value);

	// value of address at PC + 1
	else if(location == ADR_OFF)
		mem.set(s_reg[PC].get() + 1, value);

}

/*
 * Shift-left A by B (sets overflow)
 */
void _cpu::_shl(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Shift-right A by B (sets overflow)
 */
void _cpu::_shr(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Subtraction of B from A (sets overflow)
 */
void _cpu::_sub(unsigned short a, unsigned short b) {

	// TODO

}

/*
 * Exclusive-OR of A and B
 */
void _cpu::_xor(unsigned short a, unsigned short b) {

	// TODO

}
