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
 * Hypervisor call implementation for interrupt support hcalls.
 *
 */

#include <config.h>
#include <h_proto.h>

sval
__h_ipoll(struct cpu_thread *thread, uval server_num)
{
	thread = thread;
	server_num = server_num;
	return (H_Function);
}
extern sval h_ipoll(struct cpu_thread *thread, uval server_num)
	__attribute__ ((weak, alias("__h_ipoll")));
