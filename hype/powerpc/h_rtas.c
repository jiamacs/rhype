
/*
 * Copyright (C) 2005 Jimi Xenidis <jimix@watson.ibm.com>, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 $Id$
 *
 */

#include <config.h>
#include <h_proto.h>

sval
h_rtas(struct cpu_thread *thread, uval argp, uval privp)
{
	/* no matter what happens we are returning these */
	/* At the moment we cannot fail, so a failuer is an assert */
	return_arg(thread, 1, argp);
	return_arg(thread, 2, privp);

	assert(0, "not implemeted yet");
	return (H_Success);
}
