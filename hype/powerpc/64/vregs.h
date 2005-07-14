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

#ifndef FORCE_APPLE_MODE
#define __HYPE_VREGS_H
#endif

#ifndef __HYPE_VREGS_H
#define __HYPE_VREGS_H

#include_next <vregs.h>

#include <msr.h>
#define VSTATE_KERN_MODE	0x1
#define VSTATE_ACTIVE_DEC	0x2
#define VSTATE_PENDING_DEC	0x4
#define VSTATE_PENDING_EXT	0x8
#define VSTATE_FORCE_EXCEPTION	0x10
#define VSTATE_DISABLED		0x20
#define VSTATE_MER		0x40
#define VSTATE_PENDING		(VSTATE_PENDING_EXT | VSTATE_PENDING_DEC)


#define V_LPAR_MSR_ON	(MSR_PR | MSR_IR | MSR_DR | MSR_ME | MSR_EE)
#define V_LPAR_MSR_OFF (MSR_HV | MSR_AM | (MSR_SF>>2))

#ifndef __ASSEMBLY__
#include <config.h>
#include <types.h>

#if !defined(HAS_SWSLB) || SWSLB_SR_NUM != 64
#error Assuming 64 entry SW SLB for Apple Mode
#endif

/* lpt : class id : class member : page : page offset */
/*  5  :    20    :     12       : 28 ----------      */
struct vstate {
	uval	thread_mode; /* saved version of tca's vstate */
	uval	dec_target;
	struct vm_class* active_cls[NUM_MAP_CLASSES];
	struct  slb_cache kern_slb_cache;
};

extern sval vbase_config(struct cpu_thread* thr, uval vbase, uval size);

extern void vrfid_asm(struct vexc_save_regs* restore)
	__attribute__((noreturn));

extern void set_v_msr(struct cpu_thread* thr, uval val);

extern sval insert_exception(struct cpu_thread *thread, uval exnum);
extern sval insert_debug_exception(struct cpu_thread *thread, uval dbgflag);
extern uval32 fetch_instruction(struct cpu_thread *thread, uval addr);

extern sval deliver_async_exception(struct cpu_thread *thread);


#endif

#endif /* __HYPE_VREGS_H */
