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
#include <htab.h>
#include <vm.h>

sval
h_clear_mod(struct cpu_thread *thread, uval flags, uval ptex)
{
	union pte *cur_htab = (union pte *)GET_HTAB(thread->cpu->os);
	union pte *pte;

	if (flags != 0) {
		hprintf("WARNING: %s: "
			"flags are undefined and should be 0: 0x%lx\n",
			__func__, flags);
	}

	if (check_index(thread->cpu->os, ptex))
		return H_Parameter;

	pte = &cur_htab[ptex];

	thread->reg_gprs[4] = pte->words.rpnWord;

	if (pte->bits.c == 0)
		return H_Success;

	pte->bits.v = 0;
	ptesync();
	do_tlbie(pte, ptex);

	pte->bits.c = 0;
	PTE_MOD_GENERAL_END(pte);

	return H_Success;
}
