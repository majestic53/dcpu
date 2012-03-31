/*
 * dcpu.cpp
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
#include "dcpu.hpp"

/*
 * Cpu constructor
 */
dcpu::dcpu(void) { reset(); }

/*
 * Cpu constructor
 */
dcpu::dcpu(const dcpu &other) : m_reg(other.m_reg), s_reg(other.s_reg), mem(other.mem) {
	return;
}

/*
 * Cpu constructor
 */
dcpu::dcpu(const mem128 &mem) : mem(mem) {
	reset();
}

/*
 * Cpu constructor
 */
dcpu::dcpu(const reg16 (&m_reg)[M_REG_COUNT], const reg16 (&s_reg)[S_REG_COUNT], const mem128 &mem) : m_reg(m_reg),
		s_reg(s_reg), mem(mem) {
	return;
}

/*
 * Cpu destructor
 */
dcpu::~dcpu(void) {
	return;
}

/*
 * Cpu assignment operator
 */
dcpu &dcpu::operator=(const dcpu &other) {

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
bool dcpu::operator==(const dcpu &other) {

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
 * Cpu not-equals operator
 */
bool dcpu::operator!=(const dcpu &other) {
	return !(*this == other);
}

/*
 * Add B to A (sets overflow)
 */
void dcpu::_add(unsigned short a, unsigned short b) {
	unsigned int value;

	// check for overflow
	if((value = get_value(a) + get_value(b)) >= 0x10000)
		s_reg[OVERFLOW].set(1);
	set_value(a, value);
}

/*
 * Binary AND of A and B
 */
void dcpu::_and(unsigned short a, unsigned short b) {
	set_value(a, get_value(a) & get_value(b));
}

/*
 * Binary OR of A and B
 */
void dcpu::_bor(unsigned short a, unsigned short b) {
	set_value(a, get_value(a) | get_value(b));
}

/*
 * Division of A by B (sets overflow)
 */
void dcpu::_div(unsigned short a, unsigned short b) {
	unsigned int value;

	// check for overflow
	if((value = get_value(a) / get_value(b)) >= 0x10000)
		s_reg[OVERFLOW].set(1);
	set_value(a, value);
}

/*
 * Jump one instruction if !(A & B)
 */
void dcpu::_ifb(unsigned short a, unsigned short b) {
	if(!(get_value(a) & get_value(b)))
		s_reg[PC]++;
}

/*
 * Jump one instruction if (A != B)
 */
void dcpu::_ife(unsigned short a, unsigned short b) {
	if(get_value(a) != get_value(b))
		s_reg[PC]++;
}

/*
 * Jump one instruction if (A <= B)
 */
void dcpu::_ifg(unsigned short a, unsigned short b) {
	if(get_value(a) <= get_value(b))
		s_reg[PC]++;
}

/*
 * Jump one instruction if (A == B)
 */
void dcpu::_ifn(unsigned short a, unsigned short b) {
	if(get_value(a) == get_value(b))
		s_reg[PC]++;
}

/*
 * Modulus of A by B
 */
void dcpu::_mod(unsigned short a, unsigned short b) {
	set_value(a, get_value(a) % get_value(b));
}

/*
 * Multiplication of B from A (sets overflow)
 */
void dcpu::_mul(unsigned short a, unsigned short b) {
	unsigned int value;

	// check for overflow
	if((value = get_value(a) * get_value(b)) >= 0x10000)
		s_reg[OVERFLOW].set(1);
	set_value(a, value);
}

/*
 * Set A to B
 */
void dcpu::_set(unsigned short a, unsigned short b) {
	set_value(a, get_value(b));
}

/*
 * Shift-left A by B (sets overflow)
 */
void dcpu::_shl(unsigned short a, unsigned short b) {
	unsigned int value;

	// check for overflow
	if((value = get_value(a) << get_value(b)) >= 0x10000)
		s_reg[OVERFLOW].set(1);
	set_value(a, value);
}

/*
 * Shift-right A by B (sets overflow)
 */
void dcpu::_shr(unsigned short a, unsigned short b) {
	unsigned short value, a_val = get_value(a), b_val = get_value(b);

	// check for overflow
	if((value = a_val >> b_val) >= a_val)
		s_reg[OVERFLOW].set(1);
	set_value(a, value);
}

/*
 * Subtraction of B from A (sets overflow)
 */
void dcpu::_sub(unsigned short a, unsigned short b) {
	unsigned short a_val = get_value(a), b_val = get_value(b);

	// check for overflow
	if(b_val > a_val)
		s_reg[OVERFLOW].set(1);
	set_value(a, a_val - b_val);
}

/*
 * Exclusive-OR of A and B
 */
void dcpu::_xor(unsigned short a, unsigned short b) {
	set_value(a, get_value(a) ^ get_value(b));
}

/*
 * Return a string representation of a cpu
 */
std::string dcpu::dump(void) {
	std::stringstream ss;

	// print all system registers
	ss << "S_REG { ";
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		ss << s_reg[i].dump() << ", ";


	// print all main registers
	ss << "}" << std::endl << "M_REG { ";
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		ss << m_reg[i].dump() << ", ";
	ss << "}";
	return ss.str();
}

/*
 * Dump cpu to file at a givne path
 */
bool dcpu::dump_to_file(const std::string &path) {

	// write system registers to file
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		if(!s_reg[i].dump_to_file(path))
			return false;

	// write main registers to file
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		if(!m_reg[i].dump_to_file(path))
			return false;
	return true;
}

/*
 * Execute a single command
 */
bool dcpu::exec(unsigned short op) {
	unsigned char code = 0;
	unsigned short a = 0, b = 0;

	// parse opt-code, A & B from opt
	for(size_t i = 0; i < 8 * sizeof(short); ++i)

		// parse code
		if(i < OP_LEN) {
			if(op & (1 << i))
				code |= (1 << i);

		// parse A
		} else if(i >= OP_LEN && i < OP_LEN + INPUT_LEN) {
			if(op & (1 << i))
				a |= (1 << (i - OP_LEN));

		// parse B
		} else {
			if(op & (1 << i))
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
 * Execute a series of commands
 */
bool dcpu::exec(std::vector<unsigned short> &op) {
	return exec(0, op.size(), op);
}

/*
 * Execute a series of commands starting at offset to range
 */
bool dcpu::exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &op) {

	// execute all commands
	for(unsigned short i = 0; i < range; ++i)
		if(!exec(op.at(offset + i)))
			return false;
	return true;
}

/*
 * Return a value held at a given location
 */
unsigned short dcpu::get_value(unsigned short location) {

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

	// value at address in SP
	else if(location == PUSH)
		return mem.at((--s_reg[SP]).get());

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
		return mem.at(mem.at(s_reg[PC].get() + 1));

	// value of PC + 1
	else if(location == LIT_OFF)
		return mem.at(s_reg[PC].get() + 1);

	// Literal value from 0 - 31
	else if(location >= L_LIT && location <= H_LIT)
		return location % LIT_COUNT;
	return 0;
}

/*
 * Return a main register
 */
reg16 &dcpu::m_register(unsigned char reg) {
	return m_reg[reg];
}

/*
 * Return memory
 */
mem128 &dcpu::memory(void) {
	return mem;
}

/*
 * Reset cpu
 */
void dcpu::reset(void) {

	// clear main registers
	for(unsigned short i = 0; i < M_REG_COUNT; ++i)
		m_reg[i].clear();

	// clear system registers
	for(unsigned short i = 0; i < S_REG_COUNT; ++i)
		s_reg[i].clear();
}

/*
 * Return a system register
 */
reg16 &dcpu::s_register(unsigned char reg) {
	return s_reg[reg];
}

/*
 * Set a value held at a given location
 */
void dcpu::set_value(unsigned short location, unsigned short value) {

	// register value
	if(location >= L_REG && location <= H_REG)
		m_reg[location].set(value);

	// value at address in register
	else if(location >= L_VAL && location <= H_VAL)
		mem.set(m_reg[location % M_REG_COUNT].get(), value);

	// value at address ((PC + 1) + register value)
	else if(location >= L_OFF && location <= H_OFF)
		mem.set(mem.at(s_reg[PC].get() + 1) + m_reg[location % M_REG_COUNT].get(), value);

	// value at address in SP and increment SP
	else if(location == POP)
		mem.set(s_reg[SP]++.get(), value);

	// value at address in SP
	else if(location == PEEK)
		mem.set(s_reg[SP].get(), value);

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
		mem.set(mem.at(s_reg[PC].get() + 1), value);
}
