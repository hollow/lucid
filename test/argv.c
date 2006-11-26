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

#include "argv.h"
#include "log.h"

static
int argv_cmp(char ** const argv1, char ** const argv2, int argc)
{
	int i, err = 0;
	
	for (i = 0; i < argc; i++) {
		if (argv1[i] == NULL && argv2[i] == NULL)
			continue;
		else if (argv1[i] == NULL && argv2[i] != NULL)
			err++;
		else if (argv1[i] != NULL && argv2[i] == NULL)
			err++;
		else if (strcmp(argv1[i], argv2[i]))
			err++;
		else
			continue;
	}
	
	return err;
}

static
int argv_from_str_t(void)
{
	int i, ret, rc = 0;
	char *str, *argv[4];
	
	struct test {
		char *str;
		int ret;
		char *argv[4];
	} T[] = {
		{ NULL,                  0, { NULL, NULL, NULL, NULL } },
		{ "",                    0, { NULL, NULL, NULL, NULL } },
		{ "arg1",                1, { "arg1", NULL, NULL, NULL } },
		{ "arg1 arg2",           2, { "arg1", "arg2", NULL, NULL } },
		/* max_argc = 4 */
		{ "arg1 arg2 arg3 arg4", 3, { "arg1", "arg2", "arg3", NULL } },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		str = NULL;
		
		if (T[i].str)
			str = strdup(T[i].str);
		
		ret = argv_from_str(str, argv, 4);
		
		if (ret  != T[i].ret || argv_cmp(argv, T[i].argv, 4))
			rc += log_error("[%s/%02d] E[%d,%s,%s,%s,%s] R[%d,%s,%s,%s,%s]",
			                __FUNCTION__, i,
			                T[i].ret,
			                T[i].argv[0], T[i].argv[1], T[i].argv[2], T[i].argv[3],
			                ret, argv[0], argv[1], argv[2], argv[3]);
		
		if (str)
			free(str);
	}
	
	return rc;
}

static
int argv_to_str_t(void)
{
	int i, rc = 0;
	char *str;
	
	struct test {
		const char *argv[4];
		int argc;
		char *str;
	} T[] = {
		{ { NULL, NULL, NULL, NULL },       0, "" },
		{ { "arg1", NULL, NULL, NULL },     1, "arg1" },
		{ { "arg1", "arg2", NULL, NULL },   2, "arg1 arg2" },
		/* max_argc = 2 */
		{ { "arg1", "arg2", "arg3", NULL }, 2, "arg1 arg2" },
	};
	
	int TS = sizeof(T) / sizeof(T[0]);
	
	for (i = 0; i < TS; i++) {
		str = argv_to_str(T[i].argc, T[i].argv);
		
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
		.ident  = "argv",
		.stderr = true,
		.time   = false,
	};
	
	log_init(&log_options);
	
	rc += argv_from_str_t();
	rc += argv_to_str_t();
	
	log_close();
	
	return rc;
}
