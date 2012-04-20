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

#include <fstream>
#include <iostream>
#include <vector>
#include "dcpu.hpp"
#include "mem128.hpp"
#include "reg16.hpp"
#include "types.hpp"

/*
 * Supported input flags
 */
enum FLAG { NONE, PRINT_REG, PRINT_MEM, OUTPUT, INPUT };

/*
 * Determine if an input is a flag
 */
int is_flag(const std::string &flag) {
	if(flag == "-r")
		return PRINT_REG;
	else if(flag == "-m")
		return PRINT_MEM;
	else if(flag == "-d")
		return OUTPUT;
	else if(flag == "-p")
		return INPUT;
	return NONE;
}

/*
 * Main
 */
int main(int argc, char *argv[]) {
	dcpu cpu;
	halfword low, high;
	std::ifstream file;
	std::vector<word> prog;
	int output = NONE, path = NONE;
	bool print_reg = false, print_mem = false;

	// check input
	if(argc < 2) {
		std::cerr << "Usage: " << argv[0] << " [-r | -m] [-d PATH] -p PATH..." << std::endl;
		return 1;
	}
	for(int i = 1; i < argc; ++i)
		switch(is_flag(argv[i])) {
			case PRINT_REG: print_reg = true;
				break;
			case PRINT_MEM: print_mem = true;
				break;
			case OUTPUT:
				if(i == (argc - 1)) {
					std::cerr << "Exception: Parameter \'-d\' missing operand" << std::endl;
					return 1;
				}
				output = ++i;
				break;
			case INPUT:
				if(i == (argc - 1)) {
					std::cerr << "Exception: Parameter \'-p\' missing operand" << std::endl;
					return 1;
				}
				path = ++i;
				break;
			default: std::cerr << "Exception: Invalid parameter \'" << argv[i] << "\'" << std::endl;
				return 1;
		}

	// check if input path was given
	if(!path) {
		std::cerr << "Exception: No input path specified" << std::endl;
		return 1;
	}

	// attempt to open file
	file.open(argv[path], std::ios::in | std::ios::binary);
	if(!file.is_open()) {
		std::cerr << "Exception: \'" << argv[path] << "\' (file does not exist)" << std::endl;
		return 1;
	}

	// check file size
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);
	if(!size
			|| (size % 2)) {
		std::cerr << "Exception: Invalid binary size" << std::endl;
		return 1;
	}

	// read in file
	while((file >> high)
			&& (file >> low)) {

		// big endian!
		prog.push_back((word) ((high << 8) | low));
	}
	file.close();

	// add instructions to memory
	for(size_t i = 0; i < prog.size(); ++i)
		cpu.memory().set(i, prog.at(i));

	// run cpu
	cpu.run();

	// print cpu info & memory
	if(print_reg)
		std::cout << cpu.dump() << std::endl;
	if(print_mem)
		std::cout << cpu.memory().dump_all() << std::endl;

	// write cpu info & memory to file
	if(output)
		if(!cpu.memory().dump_to_file(LOW, HIGH, argv[output])) {
			std::cerr << "Exception: Failed to write memory to path" << std::endl;
			return 1;
		}
	return 0;
}
