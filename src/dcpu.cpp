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

#include <iostream>

#include <sstream>
#include "dcpu.hpp"

/*
 * Cpu constructor
 */
dcpu::dcpu(void) {
	reset();
}

/*
 * Cpu constructor
 */
dcpu::dcpu(const dcpu &other) : m_reg(other.m_reg), s_reg(other.s_reg), mem(other.mem),
		state(other.state), cycle(other.cycle) {
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
dcpu::dcpu(const reg16 (&m_reg)[M_REG_COUNT], const reg16 (&s_reg)[S_REG_COUNT], const mem128 &mem,
		word state, size_t cycle) : m_reg(m_reg), s_reg(s_reg), mem(mem), state(state), cycle(cycle) {
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
	for(word i = 0; i < M_REG_COUNT; ++i)
		m_reg[i] = other.m_reg[i];
	for(word i = 0; i < S_REG_COUNT; ++i)
		s_reg[i] = other.s_reg[i];
	mem = other.mem;
	state = other.state;
	cycle = other.cycle;
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
	for(word i = 0; i < M_REG_COUNT; ++i)
		if(m_reg[i] != other.m_reg[i])
			return false;
	for(word i = 0; i < S_REG_COUNT; ++i)
		if(s_reg[i] != other.s_reg[i])
			return false;
	return mem == other.mem
			&& state == other.state
			&& cycle == other.cycle;
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
void dcpu::_add(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);
	dword res = a_val + b_val;

	// set overflow
	if(res >= HIGH)
		s_reg[OVERFLOW].set(FLAG);
	else
		s_reg[OVERFLOW].set(LOW);

	// perform addition
	set_value(a_addr, res);
	cycle += 2;
}

/*
 * Binary AND of A and B
 */
void dcpu::_and(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// perform binary operation
	set_value(a_addr, a_val & b_val);
	++cycle;
}

/*
 * Binary OR of A and B
 */
void dcpu::_bor(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// perform binary operation
	set_value(a_addr, a_val | b_val);
	++cycle;
}

/*
 * Division of A by B (sets overflow)
 */
void dcpu::_div(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check if b value is zero
	if(!b_val) {

		// set overflow
		s_reg[OVERFLOW].set(LOW);
		set_value(a_addr, LOW);
	} else {

		// set overflow
		s_reg[OVERFLOW].set(((a_val << 16) / b_val) & HIGH);

		// perform addition
		set_value(a_addr, a_val / b_val);
	}
	cycle += 3;
}

/*
 * Execute next instruction if ((A & B) != 0)
 */
void dcpu::_ifb(word a, word b) {

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check condition
	if(a_val & b_val)
		exec(s_reg[PC].get());
	else

		// add cycle on fail
		++cycle;
	cycle += 2;
}

/*
 * Execute next instruction if (A == B)
 */
void dcpu::_ife(word a, word b) {

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check condition
	if(a_val == b_val)
		exec(s_reg[PC].get());
	else

		// add cycle on fail
		++cycle;
	cycle += 2;
}

/*
 * Execute next instruction if (A > B)
 */
void dcpu::_ifg(word a, word b) {

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check condition
	if(a_val > b_val)
		exec(s_reg[PC].get());
	else

		// add cycle on fail
		++cycle;
	cycle += 2;
}

/*
 * Execute next instruction if (A != B)
 */
void dcpu::_ifn(word a, word b) {

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check condition
	if(a_val != b_val)
		exec(s_reg[PC].get());
	else

		// add cycle on fail
		++cycle;
	cycle += 2;
}

/*
 * Push the address of the next word onto the stack
 */
void dcpu::_jsr(word a) {
	mem.set((--s_reg[SP]).get(), s_reg[PC].get());
	s_reg[PC].set(a);
	cycle += 2;
}

/*
 * Modulus of A by B
 */
void dcpu::_mod(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// check if b value is zero
	if(!b_val)
		set_value(a_addr, LOW);
	else

		// perform division
		set_value(a_addr, a_val % b_val);
	cycle += 3;
}

/*
 * Multiplication of B from A (sets overflow)
 */
void dcpu::_mul(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// set overflow
	s_reg[OVERFLOW].set(((a_val * b_val) >> 16) & HIGH);

	// perform multiplication
	set_value(a_addr, a_val * b_val);
	cycle += 2;
}

/*
 * Set A to B
 */
void dcpu::_set(word a, word b) {

	// retrieve info
	word *addr = get_address(a);
	word value = get_value(b);

	// perform set
	set_value(addr, value);
	++cycle;
}

/*
 * Shift-left A by B (sets overflow)
 */
void dcpu::_shl(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// set overflow
	s_reg[OVERFLOW].set(((a_val << b_val) >> 16) & HIGH);

	// perform shift
	set_value(a_addr, a_val << b_val);
	cycle += 2;
}

/*
 * Shift-right A by B (sets overflow)
 */
void dcpu::_shr(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// set overflow
	s_reg[OVERFLOW].set(((a_val << 16) >> b_val) & HIGH);

	// perform shift
	set_value(a_addr, a_val >> b_val);
	cycle += 2;
}

/*
 * Subtraction of B from A (sets overflow)
 */
void dcpu::_sub(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// set overflow
	if(b_val > a_val)
		s_reg[OVERFLOW].set(HIGH);
	else
		s_reg[OVERFLOW].set(LOW);

	// perform subtraction
	set_value(a_addr, a_val - b_val);
	cycle += 2;
}

/*
 * Exclusive-OR of A and B
 */
void dcpu::_xor(word a, word b) {

	// retrieve address
	word *a_addr = get_address(a);

	// properly set PC
	if((a >= L_OFF && a <= H_OFF)
			|| a == ADR_OFF
			|| a == LIT_OFF)
		--s_reg[PC];

	// retrieve values
	word a_val = get_value(a);
	word b_val = get_value(b);

	// perform binary operation
	set_value(a_addr, a_val ^ b_val);
	++cycle;
}

/*
 * Returns a Cpu cycle count
 */
size_t dcpu::cycles(void) {
	return cycle;
}

/*
 * Return a string representation of a cpu
 */
std::string dcpu::dump(void) {
	std::stringstream ss;

	// print attributes
	ss << "STATE: ";
	switch(state) {
		case INIT:
			ss << "INIT";
			break;
		case RUN:
			ss << "RUNNING";
			break;
		case HALT:
			ss << "HALTED";
			break;
		default:
			ss << "UNKNOWN";
			break;
	}
	ss << ", CYCLE: " << cycle << std::endl;

	// print all system registers
	ss << "S_REG { ";
	for(word i = 0; i < S_REG_COUNT; ++i)
		ss << s_reg[i].dump() << ", ";

	// print all main registers
	ss << "}" << std::endl << "M_REG { ";
	for(word i = 0; i < M_REG_COUNT; ++i)
		ss << m_reg[i].dump() << ", ";
	ss << "}";
	return ss.str();
}

/*
 * Dump cpu to file at a givne path
 */
bool dcpu::dump_to_file(const std::string &path) {

	// write system registers to file
	for(word i = 0; i < S_REG_COUNT; ++i)
		if(!s_reg[i].dump_to_file(path))
			return false;

	// write main registers to file
	for(word i = 0; i < M_REG_COUNT; ++i)
		if(!m_reg[i].dump_to_file(path))
			return false;
	return true;
}

/*
 * Execute a single command
 */
bool dcpu::exec(word op) {
	word code = 0, a = 0, b = 0;

	// check state
	if(!is_running())
		return false;

	// parse opt-code, A & B from opt
	for(size_t i = 0; i < 8 * sizeof(word); ++i)

		// parse code
		if(i < B_OP_LEN) {
			if(op & (1 << i))
				code |= (1 << i);

		// parse A
		} else if(i >= B_OP_LEN && i < B_OP_LEN + INPUT_LEN) {
			if(op & (1 << i))
				a |= (1 << (i - B_OP_LEN));

		// parse B
		} else {
			if(op & (1 << i))
				b |= (1 << (i - (B_OP_LEN + INPUT_LEN)));
		}

	// increment pc by one
	s_reg[PC]++;

	// execute command based on code
	switch(code) {
		case NB:
			switch(a) {
				case JSR: _jsr(b);
					break;
				default: return false;
			}
			break;
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
bool dcpu::exec(std::vector<word> &op) {
	return exec(0, op.size(), op);
}

/*
 * Execute a series of commands starting at offset to range
 */
bool dcpu::exec(word offset, word range, std::vector<word> &op) {

	// execute all commands
	for(word i = 0; i < range; ++i)
		if(!exec(op.at(offset + i)))
			return false;
	return true;
}

/*
 * Return an address of a value at a given location
 */
word *dcpu::get_address(word value) {

	// register value
	if(value >= L_REG && value <= H_REG) {
		++cycle;
		return &m_reg[value].get();
	}

	// value at address in register
	else if(value >= L_VAL && value <= H_VAL) {
		++cycle;
		return &mem.at(m_reg[value % M_REG_COUNT].get());
	}

	// value at address ((PC + 1) + register value)
	else if(value >= L_OFF && value <= H_OFF) {
		cycle += 2;
		return &mem.at(mem.at(s_reg[PC]++.get()) + m_reg[value % M_REG_COUNT].get());
	}

	// value at address in SP and increment SP
	else if(value == POP) {
		++cycle;
		return &mem.at(s_reg[SP]++.get());
	}

	// value at address in SP
	else if(value == PEEK) {
		++cycle;
		return &mem.at(s_reg[SP].get());
	}

	// value at address in SP
	else if(value == PUSH) {
		++cycle;
		return &mem.at((--s_reg[SP]).get());
	}

	// value in SP
	else if(value == SP_VAL)
		return &s_reg[SP].get();

	// value in PC
	else if(value == PC_VAL)
		return &s_reg[PC]++.get();

	// value in overflow
	else if(value == OVER_F)
		return &s_reg[OVERFLOW].get();

	// value of address at PC + 1
	else if(value == ADR_OFF) {
		cycle += 2;
		return &mem.at(mem.at(s_reg[PC]++.get()));
	}

	// value at PC + 1
	else if(value == LIT_OFF) {
		++cycle;
		return &mem.at(s_reg[PC]++.get());
	}
	return NULL;
}

/*
 * Return a value held at a given location
 */
word dcpu::get_value(word value) {

	// register value
	if(value >= L_REG && value <= H_REG) {
		++cycle;
		return m_reg[value].get();
	}

	// value at address in register
	else if(value >= L_VAL && value <= H_VAL) {
		++cycle;
		return mem.at(m_reg[value % M_REG_COUNT].get());
	}

	// value at address ((PC + 1) + register value)
	else if(value >= L_OFF && value <= H_OFF) {
		cycle += 2;
		return mem.at(mem.at(s_reg[PC]++.get()) + m_reg[value % M_REG_COUNT].get());
	}

	// value at address in SP and increment SP
	else if(value == POP) {
		++cycle;
		return mem.at(s_reg[SP]++.get());
	}

	// value at address in SP
	else if(value == PEEK) {
		++cycle;
		return mem.at(s_reg[SP].get());
	}

	// value at address in SP
	else if(value == PUSH) {
		++cycle;
		return mem.at((--s_reg[SP]).get());
	}

	// value in SP
	else if(value == SP_VAL)
		return s_reg[SP].get();

	// value in PC
	else if(value == PC_VAL)
		return s_reg[PC]++.get();

	// value in overflow
	else if(value == OVER_F)
		return s_reg[OVERFLOW].get();

	// value of address at PC + 1
	else if(value == ADR_OFF) {
		cycle += 2;
		return mem.at(mem.at(s_reg[PC]++.get()));
	}

	// value at PC + 1
	else if(value == LIT_OFF) {
		++cycle;
		return mem.at(s_reg[PC]++.get());
	}

	// Literal value from 0 - 31
	else if(value >= L_LIT && value <= H_LIT)
		return value % LIT_COUNT;
	return 0;
}

/*
 * Halt a Cpu
 */
bool dcpu::halt(void) {
	return state_change(HALT);
}

/*
 * Returns a Cpu running status
 */
bool dcpu::is_running(void) {
	return state == RUN;
}

/*
 * Return a main register
 */
reg16 &dcpu::m_register(word reg) {
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
	for(word i = 0; i < M_REG_COUNT; ++i)
		m_reg[i].clear();

	// clear system registers
	for(word i = 0; i < S_REG_COUNT; ++i)
		s_reg[i].clear();

	// clean attributes
	state = INIT;
	cycle = 0;
}

/*
 * Run a Cpu
 */
bool dcpu::run(void) {

	// attempt to change state
	if(!state_change(RUN))
		return false;

	// run until no more commands are found
	// or a malformed command is found
	while(exec(mem.at(s_reg[PC].get())));
	halt();
	return true;
}

/*
 * Return a system register
 */
reg16 &dcpu::s_register(word reg) {
	return s_reg[reg];
}

/*
 * Set a value held at a given location
 */
void dcpu::set_value(word *location, word value) {
	*location = value;
}

/*
 * Perform a state change
 */
bool dcpu::state_change(word state) {

	// check if already in state
	if(this->state == state)
		return false;

	// assign new state
	this->state = state;
	return true;
}
