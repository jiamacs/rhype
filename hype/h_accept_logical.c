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
 * H_GRANT_LOGICAL implementation.
 *
 */

#include <config.h>
#include <hype.h>
#include <types.h>
#include <lib.h>
#include <hcall.h>
#include <hash.h>
#include <resource.h>
#include <lpar.h>
#include <os.h>
#include <h_proto.h>

sval
h_accept_logical(struct cpu_thread *thread, uval cookie)
{
	struct sys_resource *sr;
	uval retval;

	sval err = get_sys_resource(thread->cpu->os, cookie, &sr);

	if (err != H_Success) {
		return err;
	}

	lock_acquire(&sr->sr_lock);

	err = accept_locked_resource(sr, &retval);

	if (err == H_Success) {
		return_arg(thread, 1, retval);
	}

	lock_release(&sr->sr_lock);
	return err;
}
