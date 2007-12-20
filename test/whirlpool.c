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

#include "log.h"
#include "whirlpool.h"

static
int whirlpool_digest_t(void)
{
	int i, rc = 0;
	char *digest = NULL;

	char astring[1000001];

	bzero(astring, 1000001);
	memset(astring, 'a', 1000000);

	struct test {
		const char *str;
		const char *digest;
	} T[] = {
		{ "",
		  "19FA61D75522A466"
		  "9B44E39C1D2E1726"
		  "C530232130D407F8"
		  "9AFEE0964997F7A7"
		  "3E83BE698B288FEB"
		  "CF88E3E03C4F0757"
		  "EA8964E59B63D937"
		  "08B138CC42A66EB3" },
		{ "a",
		  "8ACA2602792AEC6F"
		  "11A67206531FB7D7"
		  "F0DFF59413145E69"
		  "73C45001D0087B42"
		  "D11BC645413AEFF6"
		  "3A42391A39145A59"
		  "1A92200D560195E5"
		  "3B478584FDAE231A" },
		{ "abc",
		  "4E2448A4C6F486BB"
		  "16B6562C73B4020B"
		  "F3043E3A731BCE72"
		  "1AE1B303D97E6D4C"
		  "7181EEBDB6C57E27"
		  "7D0E34957114CBD6"
		  "C797FC9D95D8B582"
		  "D225292076D4EEF5" },
		{ "message digest",
		  "378C84A4126E2DC6"
		  "E56DCC7458377AAC"
		  "838D00032230F53C"
		  "E1F5700C0FFB4D3B"
		  "8421557659EF55C1"
		  "06B4B52AC5A4AAA6"
		  "92ED920052838F33"
		  "62E86DBD37A8903E" },
		{ "abcdefghijklmnopqrstuvwxyz",
		  "F1D754662636FFE9"
		  "2C82EBB9212A484A"
		  "8D38631EAD4238F5"
		  "442EE13B8054E41B"
		  "08BF2A9251C30B6A"
		  "0B8AAE86177AB4A6"
		  "F68F673E7207865D"
		  "5D9819A3DBA4EB3B" },
		{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		  "DC37E008CF9EE69B"
		  "F11F00ED9ABA2690"
		  "1DD7C28CDEC066CC"
		  "6AF42E40F82F3A1E"
		  "08EBA26629129D8F"
		  "B7CB57211B9281A6"
		  "5517CC879D7B9621"
		  "42C65F5A7AF01467" },
		{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		  "466EF18BABB0154D"
		  "25B9D38A6414F5C0"
		  "8784372BCCB204D6"
		  "549C4AFADB601429"
		  "4D5BD8DF2A6C44E5"
		  "38CD047B2681A51A"
		  "2C60481E88C5A20B"
		  "2C2A80CF3A9A083B" },
		{ "abcdbcdecdefdefgefghfghighijhijk",
		  "2A987EA40F917061"
		  "F5D6F0A0E4644F48"
		  "8A7A5A52DEEE6562"
		  "07C562F988E95C69"
		  "16BDC8031BC5BE1B"
		  "7B947639FE050B56"
		  "939BAAA0ADFF9AE6"
		  "745B7B181C3BE3FD" },
		{ astring,
		  "0C99005BEB57EFF5"
		  "0A7CF005560DDF5D"
		  "29057FD86B20BFD6"
		  "2DECA0F1CCEA4AF5"
		  "1FC15490EDDC47AF"
		  "32BB2B66C34FF9AD"
		  "8C6008AD677F7712"
		  "6953B226E4ED8B01" },
	};

	int TS = sizeof(T) / sizeof(T[0]);

	for (i = 0; i < TS; i++) {
		if (digest)
			free(digest);

		digest = whirlpool_digest(T[i].str);

		if (strcmp(digest, T[i].digest))
			rc += log_error("[%s/%02d] E[%s] R[%s]",
			                __FUNCTION__, i,
			                T[i].digest, digest);
	}

	return rc;
}

int main(int argc, char *argv[])
{
	int rc = EXIT_SUCCESS;

	log_options_t log_options = {
		.log_ident  = "whirlpool",
		.log_dest  = LOGD_STDERR,
		.log_opts  = LOGO_PRIO|LOGO_IDENT,
	};

	log_init(&log_options);

	rc += whirlpool_digest_t();

	log_close();

	return rc;
}
