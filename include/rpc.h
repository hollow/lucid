// Copyright (C) 2006-2007 Benedikt Böhm <hollow@gentoo.org>
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

/*!
 * @defgroup rpc Remote Procedure Call
 *
 * @{
 */

#ifndef _LUCID_RPC_H
#define _LUCID_RPC_H

#include <stdbool.h>
#include <netinet/in.h>

#ifdef _LUCID_BUILD
#include "rtti.h"
#else
#include <lucid/rtti.h>
#endif

struct rpc_env {
	void *handle;
	struct sockaddr_in *remote;
};

extern struct rpc_env __rpc_env;

#define RPC_TYPE_START(func) \
	struct __rpc_ ## func ## _signature { func ## _signature; }; \
	RTTI_FIELD_START(__rpc_ ## func ## _signature)

#define RPC_TYPE_MEMBER(func, name, type) \
	RTTI_STRUCT_MEMBER(__rpc_ ## func ## _signature, name, type)

#define RPC_TYPE_END(func, rtype) \
	RTTI_FIELD_END \
	const rtti_t __rpc_ ## func ## __atype = \
		RTTI_STRUCT_TYPE(__rpc_ ## func ## _signature); \
	const rtti_t *__rpc_ ## func ## _atype = \
		&(__rpc_ ## func ## __atype); \
	const rtti_t *__rpc_ ## func ## _rtype = rtype;

#define RPC_DISPATCHER(rtype, func, rdefault, ...) \
	struct __rpc_ ## func ## _signature __rpc_data = { __VA_ARGS__ }; \
	rtype __rpc_ret = rdefault; \
	rpc_send(#func, &__rpc_data, &__rpc_ret); \
	return __rpc_ret; \

#define RPC_CONTROLLER(rtype, func, rdefault, ...) \
	rtype __rpc_ ## func ## _controller(); \
	rtype func(__VA_ARGS__) func ## _signature; { \
		if (__rpc_env.remote) { \
			RPC_DISPATCHER(rtype, func, rdefault, __VA_ARGS__) \
		} else { \
			return __rpc_ ## func ## _controller(__VA_ARGS__); \
		} \
	} \
	rtype __rpc_ ## func ## _controller(__VA_ARGS__) func ## _signature;

char *rpc_receive(const char *name, const char *data);
void rpc_send(const char *name, void *data, void *ret);

#endif

/*! @} rpc */
