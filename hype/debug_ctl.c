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
 *
 * $Id$
 */

#include <config.h>
#include <types.h>
#include <hype.h>
#include <debug.h>

uval debug_level = (1 << DBG_MEMMGR);

struct debug_info debugs[32] = {
	/* *INDENT-OFF* */
	[DBG_INTRPT] = { .prefix = "intrpt", .output_fn = &hprintf },
	[DBG_MEMMGR] = { .prefix = "memmgr", .output_fn = &hprintf },
	[DBG_LPAR]   = { .prefix = "lpar",   .output_fn = &hprintf },
	[DBG_TCA]    = { .prefix = "tca",    .output_fn = &hprintf },
	[DBG_INIT]   = { .prefix = "init",   .output_fn = &hprintf },
	[DBG_VM_MAP] = { .prefix = "vmmap",  .output_fn = &hprintf },
	[DBG_EE]     = { .prefix = "ee",     .output_fn = &hprintf },
	[DBG_LLAN]   = { .prefix = "llan",     .output_fn = &hprintf },
	[DBG_VTERM]  = { .prefix = "vterm",     .output_fn = &hprintf },
	[DBG_CRQ]    = { .prefix = "crq",     .output_fn = &hprintf },
	/* *INDENT-ON* */
};

