/*
 * main.cpp
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
#include "dcpu.hpp"
#include "mem128.hpp"
#include "reg16.hpp"
#include "types.hpp"

int main(int argc, char *argv[]) {
	dcpu cpu;

	// Notch's sample program
	word len = 28;
	word prog[] = {
		0x7c01, 0x0030, 0x7de1, 0x1000, 0x0020, 0x7803, 0x1000, 0xc00d,
		0x7dc1, 0x001a, 0xa861, 0x7c01, 0x2000, 0x2161, 0x2000, 0x8463,
		0x806d, 0x7dc1, 0x000d, 0x9031, 0x7c10, 0x0018, 0x7dc1, 0x001a,
		0x9037, 0x61c1, 0x7dc1, 0x001a,
	};

	// add instructions to memory
	for(word i = 0; i < len; ++i)
		cpu.memory().set(i, prog[i]);

	// run cpu
	cpu.run();

	// print cpu info
	std::cout << cpu.dump() << std::endl;
	std::cout << cpu.memory().dump(0x0, 0x30) << std::endl;
	std::cout << cpu.memory().dump(0x1000, 0x10) << std::endl;

	std::cout << "DONE." << std::endl;
	return 0;
}
