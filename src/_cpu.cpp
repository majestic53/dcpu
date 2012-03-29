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
 * Cpu constructor
 */
_cpu::_cpu(void) {

	// TODO

}

/*
 * Cpu constructor
 */
_cpu::_cpu(const _cpu &other) {

	// TODO

}

/*
 * Cpu constructor
 */
_cpu::_cpu(const _memory &mem) {

	// TODO

}

/*
 * Cpu constructor
 */
_cpu::_cpu(const _register (&m_reg)[M_REG_COUNT], const _register (&s_reg)[S_REG_COUNT], const _memory &mem) {

	// TODO

}

/*
 * Cpu assignment operator
 */
_cpu &_cpu::operator=(const _cpu &other) {

	// TODO

}

/*
 * Cpu equals operator
 */
bool _cpu::operator==(const _cpu &other) {

	// TODO

}

/*
 * Return a string representation of a cpu
 */
std::string _cpu::dump(void) {

	// TODO

}

/*
 * Dump cpu to file at a givne path
 */
bool _cpu::dump_to_file(const std::string &path) {

	// TODO

}

/*
 * Execute a single command
 */
void _cpu::exec(unsigned short opt) {

	// TODO

}

/*
 * Execute a series of commands
 */
void _cpu::exec(std::vector<unsigned short> &opt) {

	// TODO

}

/*
 * Execute a series of commands starting at offset to range
 */
void _cpu::exec(unsigned short offset, unsigned short range, std::vector<unsigned short> &opt) {

	// TODO

}

/*
 * Reset cpu
 */
void _cpu::reset(void) {

	// TODO

}
