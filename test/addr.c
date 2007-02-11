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
int addr_from_str_t(void)
{
	int i, ret, rc = 0;
	uint32_t ip, mask;

	struct test {
		char *str;
		int ret;
		uint32_t ip;
		uint32_t mask;
	} T[] = {
		{ "",                     0, 0,          0 },
		{ "123/456",              0, 0,          0 },
		{ "123.456.78.9",         1, 0x7bc84e09, 0 }, /* overflow */
		{ "random string",        0, 0,          0 },
		{ "str.ing.wit.h/dots",   0, 0,          0 },
		{ "1.2.3.4",              1, 0x01020304, 0 },
		{ "127.0.0.1",            1, 0x7f000001, 0 },
		{ "/24",                  2, 0,          0xffffff00 },
		{ "/255.255.0.0",         2, 0,          0xffff0000 },
		{ "1.2.3.4/9",            3, 0x01020304, 0xff800000 },
		{ "1.2.3.4/255.255.0.0",  3, 0x01020304, 0xffff0000 },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		ip = mask = 0;
		
		ret = addr_from_str(T[i].str, &ip, &mask);
		
		if (ret  != T[i].ret ||
		    ip   != addr_ntoh(T[i].ip)  ||
		    mask != addr_ntoh(T[i].mask))
			rc += log_error("[%s/%02d] E[%d,%#.8x,%#.8x] R[%d,%#.8x,%#.8x]",
			                __FUNCTION__, i,
			                T[i].ret, addr_ntoh(T[i].ip), addr_ntoh(T[i].mask),
			                ret, ip, mask);
	}

	return rc;
}

static
int addr_to_str_t(void)
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
		{ 0x01020304, 0xff800000, "1.2.3.4/255.128.0.0" },
		{ 0xc0a80020, 0xffffff00, "192.168.0.32/255.255.255.0" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		str = addr_to_str(addr_hton(T[i].ip), addr_hton(T[i].mask));
		
		if (!str || strcmp(str, T[i].str))
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
		.log_ident = "addr",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += addr_from_str_t();
	rc += addr_to_str_t();

	log_close();

	return rc;
}
