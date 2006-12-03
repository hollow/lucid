// Copyright 2006 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

/*!
 * @mainpage lucid Documentation
 *
 * The lucid library combines a lot of useful functions i wrote for my
 * projects. There are a lot of custom functions for strings,
 * doubly-linked lists, bitmaps and flag lists, input/output, cryptogrpahic
 * digests, and tcp connections.
 *
 * Some of these functions, especially string and list functions, are
 * completely self-contained and do not rely on libc. This makes integration
 * in foreign projects as easy as possible.
 *
 * On the other hand, functions for input/output or chroot are just wrappers
 * around libc library functions, but may still be usefull to others as well.
 *
 * The size of functions range from a few hundred bytes to about 30K.
 *
 * @section why Why another library?
 *
 * This library was written for my own projects. As the number of foreign
 * libraries i included grew, i have collected all functions i needed - and
 * meanwhile most of them reimplemented - and made an own library.
 *
 * @section status Current Status
 *
 * Actually 4 projects (libvserver, vcd, vstatd, vwrappers) are using lucid now,
 * so an own shared library was necessary. The library contains a test suite for
 * the most important functions. Functions not explicitely tested are either
 * tested implicitely by other functions and a test may be written in the
 * future, or the function is so simple that you can just hope it works ;)
 *
 * @section doc Documentation
 *
 * All function are documented with an inline source browser for easy learning
 * and reference. To get an overview of function families please start at the
 * Modules page. Experienced users may look up information in the Globals, Data
 * Fields or File List.
 *
 * Also take a look at the \ref examples "Examples" and read the
 * \ref license "License".
 *
 * @section bugs Bugs, Patches, Wishes
 *
 * If you have found a bug in lucid that was not discovered by the test suite,
 * or if you have any suggestion, wishes or patches for the future of lucid,
 * please contact me at hollow[at]gentoo.org
 *
 * @{
 */
