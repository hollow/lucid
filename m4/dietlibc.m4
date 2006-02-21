# Copyright 2005 The libvserver Developers
# See AUTHORS for details
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by  *
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

AC_DEFUN([AC_LUCID_DIETLIBC],
[
	AC_MSG_CHECKING([for dietlibc])
	
	AC_ARG_VAR(DIET,      [The 'diet' wrapper (default: diet)])
	AC_ARG_VAR(DIETFLAGS, [Flags passed to the 'diet' wrapper (default: -O)])
	
	: ${DIET:=diet}
	: ${DIETFLAGS=-Os}
	
	CC="${DIET} ${DIETFLAGS} ${CC}"
	
	AC_LANG_PUSH(C)
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __dietlibc__
#error
#endif
	]])],
	[ac_vu_have_dietlibc=yes],
	[ac_vu_have_dietlibc=no])
	
	if test "$ac_vu_have_dietlibc" = "no"; then
		AC_MSG_ERROR([dietlibc is missing! please install dietlibc and try again])
	else
		AC_MSG_RESULT([ok])
	fi
	
	AC_LANG_POP
])
