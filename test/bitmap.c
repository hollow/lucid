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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bitmap.h"
#include "log.h"

static
int test_i2v32(void)
{
	int i, rc = 0;
	uint32_t value;
	
	struct test {
		int      index;
		uint32_t value;
	} T[] = {
		{  0, 0x00000001 },
		{  1, 0x00000002 },
		{  2, 0x00000004 },
		{  7, 0x00000080 },
		{ 31, 0x80000000 },
		{ 32, 0x00000000 },
		{ 33, 0x00000000 },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		value = i2v32(T[i].index);
		
		if (value != T[i].value)
			rc += log_error("[%s/%02d] E[%#.8lx] R[%#.8lx]",
			                __FUNCTION__, i, T[i].value, value);
	}
	
	return rc;
}

static
int test_i2v64(void)
{
	int i, rc = 0;
	uint64_t value;
	
	struct test {
		int      index;
		uint64_t value;
	} T[] = {
		{  0, 0x0000000000000001 },
		{  1, 0x0000000000000002 },
		{  2, 0x0000000000000004 },
		{  7, 0x0000000000000080 },
		{ 63, 0x8000000000000000 },
		{ 64, 0x0000000000000000 },
		{ 65, 0x0000000000000000 },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		value = i2v64(T[i].index);
		
		if (value != T[i].value)
			rc += log_error("[%s/%02d] E[%#.16llx] R[%#.16llx]",
			                __FUNCTION__, i, T[i].value, value);
	}
	
	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;
	
	log_options_t log_options = {
		.ident  = "bitmap",
		.stderr = true,
		.time   = false,
	};
	
	log_init(&log_options);
	
	rc += test_i2v32();
	rc += test_i2v64();
	
	log_close();
	
	return rc;
}
