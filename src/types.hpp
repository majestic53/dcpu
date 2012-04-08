/*
 * types.hpp
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

#ifndef TYPES_HPP_
#define TYPES_HPP_

/*
 * Word size
 */
typedef unsigned short word;
typedef unsigned int dword;

/*
 * Minimum value
 */
static const word LOW = 0x0;

/*
 * Flag value
 */
static const word FLAG = 0x1;

/*
 * Maximum value
 */
static const word HIGH = 0xFFFF;

/*
 * Word count
 */
static const dword COUNT = 0x10000;

#endif
