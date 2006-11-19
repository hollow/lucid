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

#include "addr.h"
#include "log.h"

static
int test_from_str(void)
{
	int i, ret, rc = 0;
	uint32_t ip, mask;
	
	struct test {
		char *str;
		int ret;
		uint32_t ip;
		uint32_t mask;
	} T[] = {
		{ "",                    -1, 0,          0 },
		{ "1.2.3.4",              0, 0x4030201,  0xffffff },
		{ "1.2.3.4/9",            0, 0x4030201,  0x80ff },
		{ "1.2.3.4/255.255.0.0",  0, 0x4030201,  0xffff },
		{ "/255.255.0.0",        -1, 0,          0 },
		{ "123/456",             -1, 0x7b000000, 0 },
		{ "random string",       -1, 0,          0 },
		{ "str.ing.wit.h/dots",  -1, 0,          0 },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		ip = mask = 0;
		
		ret = addr_from_str(T[i].str, &ip, &mask);
		
		if (ret  != T[i].ret ||
		    ip   != T[i].ip  ||
		    mask != T[i].mask)
			rc += log_error("[%s/%02d] E[%d,%#x,%#x] R[%d,%#x,%#x]",
			                __FUNCTION__, i,
			                T[i].ret, T[i].ip, T[i].mask,
			                ret,      ip,      mask);
	}
	
	return rc;
}

static
int test_to_str(void)
{
	int i, rc = 0;
	char *str;
	
	struct test {
		uint32_t ip;
		uint32_t mask;
		char *str;
	} T[] = {
		{ 0,          0,          "0.0.0.0" },
		{ 0xffffffff, 0,          "255.255.255.255" },
		{ 0,          0xffffffff, "0.0.0.0/255.255.255.255" },
		{ 0xffffffff, 0xffffffff, "255.255.255.255/255.255.255.255" },
		{ 0x4030201,  0x80ff,     "1.2.3.4/255.128.0.0" },
		{ 0x2000a8c0, 0xffffff,   "192.168.0.32/255.255.255.0" },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		str = addr_to_str(T[i].ip, T[i].mask);
		
		if (strcmp(str, T[i].str))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].str, str);
	}
	
	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;
	
	log_options_t log_options = {
		.ident  = "addr",
		.stderr = true,
		.time   = false,
	};
	
	log_init(&log_options);
	
	rc += test_from_str();
	rc += test_to_str();
	
	log_close();
	
	return rc;
}
