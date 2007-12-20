// Copyright (C) 2007 Benedikt Böhm <hollow@gentoo.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA

#include <dlfcn.h>
#include <ffi.h>
#include <sys/socket.h>

#include "error.h"
#include "printf.h"
#include "rtti.h"
#include "rpc.h"
#include "str.h"
#include "uio.h"

struct rpc_env __rpc_env = { RTLD_DEFAULT, NULL };

void rpc_init(const char *lib)
{
	if (!lib)
		return;

	__rpc_env.handle = dlopen(lib, RTLD_NOW);
	if (!__rpc_env.handle) {
		error_set(errno, "failed to load RPC library: %s", dlerror());
		return;
	}
}

void rpc_remote_enable(struct sockaddr_in *addr)
{
	__rpc_env.remote = addr;
}

void rpc_remote_disable(void)
{
	__rpc_env.remote = NULL;
}

struct __rpc_call_symbols {
	char *controller;
	char *atype;
	char *rtype;
};

struct __rpc_call {
	void (*entry)();
	size_t size;
	const rtti_t *atype;
	const rtti_t *rtype;
};

static
void __rpc_call_symbols_init(const char *name, struct __rpc_call_symbols *sym)
{
	asprintf(&sym->controller, "__rpc_%s_controller", name);
	asprintf(&sym->atype, "__rpc_%s_atype", name);
	asprintf(&sym->rtype, "__rpc_%s_rtype", name);
}

static
void __rpc_call_init(const char *name, struct __rpc_call *call)
{
	char *ebuf;

	/* find rpc type data */
	struct __rpc_call_symbols sym;
	__rpc_call_symbols_init(name, &sym);

	/* find method */
	dlerror();
	*(void **)(&call->entry) = dlsym(__rpc_env.handle, sym.controller);
	if ((ebuf = dlerror()) != NULL) {
		error_set(errno, "undefined method (%s): %s", name, ebuf);
		return;
	}

	dlerror();
	const rtti_t **atypep = dlsym(__rpc_env.handle, sym.atype);
	if ((ebuf = dlerror()) != NULL) {
		error_set(errno, "unknown method type: %s", ebuf);
		return;
	}
	call->atype = *atypep;

	dlerror();
	const rtti_t **rtypep = dlsym(__rpc_env.handle, sym.rtype);
	if ((ebuf = dlerror()) != NULL) {
		error_set(errno, "unknown method type: %s", ebuf);
		return;
	}
	call->rtype = *rtypep;
}

#define MEMBP(F, D) ((void *)((char *)(D) + (F).offset))

char *rpc_receive(const char *name, const char *buf)
{
	/* initialize rpc call symbols */
	struct __rpc_call call;
	__rpc_call_init(name, &call);
	error_do return NULL;

	/* decode function arguments */
	void *data = mem_alloc(call.atype->size);
	rtti_decode(call.atype, &buf, data);
	error_do return NULL;

	/* calculate number of arguments */
	int argc = rtti_length(call.atype, data);

	/* init ffi data */
	ffi_cif cif;
	ffi_type *argt[argc];
	void *argv[argc];

	size_t i;
	for (i = 0; i < argc; i++) {
		const rtti_field_t *field = call.atype->args[0].v;
		argv[i] = MEMBP(field[i], data);
		argt[i] = (field[i].type)->ftype;
	}

	if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, argc,
				(call.rtype)->ftype, argt) != FFI_OK) {
		error_set(errno, "failed to initialize ffi data (%s)", name);
		return NULL;
	}

	void *ret = NULL;
	ffi_call(&cif, call.entry, &ret, argv);

	char *retstr = rtti_encode(call.rtype, &ret);
	return retstr;
}

static
char *__rpc_send(const char *name, const char *buf)
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		error_set(errno, "failed to create client socket");
		return NULL;
	}

	if (connect(sfd, __rpc_env.remote, sizeof(*(__rpc_env.remote))) < 0) {
		error_set(errno, "failed to connect");
		return NULL;
	}

	char *req = NULL;
	asprintf(&req, "%s:%s", name, buf);

	if (uio_write_netstring(sfd, req) < 0) {
		error_set(errno, "failed to write to socket");
		return NULL;
	}

	char *res = NULL;
	if (uio_read_netstring(sfd, &res) < 0) {
		error_set(errno, "failed to read from socket");
		return NULL;
	}

	return res;
}

void rpc_send(const char *name, void *data, void *ret)
{
	/* initialize rpc call symbols */
	struct __rpc_call call;
	__rpc_call_init(name, &call);
	error_do return;

	/* encode function arguments */
	char *buf = rtti_encode(call.atype, data);
	error_do return;

	/* dispatch call */
	const char *retstr;
	if (__rpc_env.remote)
		retstr = __rpc_send(name, buf);
	else
		retstr = rpc_receive(name, buf);
	error_do return;

	rtti_decode(call.rtype, &retstr, ret);
	error_do return;
}
