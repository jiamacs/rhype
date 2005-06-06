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

#ifndef __POWERPC_MMU_H_
#define __POWERPC_MMU_H_

#include <config.h>
#include <asm.h>
#include <htab.h>
#include <types.h>
#include <partition.h>
#include <util.h>
#include <mmu_defs.h>


enum { lpg0, lpg1, lpg2 };

union slb_entry {
	struct
	{
		uval64	vsid:52;
		uval64	ks:1;
		uval64	kp:1;
		uval64	n:1;
		uval64	l:1;
		uval64	c:1;
		uval64	_zeroes1:7; /* is this lp selection? */
		uval64	esid:36;
		uval64	v:1;
		uval64	_zeroes2:15;
		uval64	index:12;
	} bits;
	struct
	{
		uval64 vsid;
		uval64 esid;
	} words;
#define slb_vsid	words.vsid
#define slb_esid	words.esid
};


extern sval hprintf(const char *fmt, ...)
	__attribute__ ((format(printf, 1, 2), no_instrument_function));

static inline void
inval_slb_entry(uval index, union slb_entry *cache)
{
	if (!cache[index].bits.v) return;

	/* slbie arguments is esid in upper-most 36 bits, followed
	 * by class bit. */
	uval64 esid_c = cache[index].words.esid;
	esid_c &= (~0ULL << 28);
	if (cache[index].bits.c)
		esid_c |= 1 << 28;

	__asm__ __volatile__ ("isync\n\t"
			      "slbie %[esid_c]\n\t"
			      "isync\n\t"::
			      [esid_c] "r" (esid_c));
	cache[index].bits.v = 0;
	cache[index].words.esid = 0;
	cache[index].words.vsid = 0;

}


static inline void
set_slb_entry(uval index, union slb_entry *cache, union slb_entry *new)
{
	inval_slb_entry(index, cache);

	cache[index] = *new;
	cache[index].bits.index = index;
	/* *INDENT-OFF* */
	__asm__ __volatile__(
		/* FIXME: do we have to isync for every one? */
		"isync			\n\t"
		"slbmte		%0,%1	\n\t"
		"isync			\n\t"
		:
		: "r" (cache[index].words.vsid), "r" (cache[index].words.esid)
		: "memory");
	/* *INDENT-ON* */
}

static inline void
load_slb_entry(uval index, union slb_entry *cache)
{
	if (cache[index].bits.v == 0) return;

	/* *INDENT-OFF* */
	__asm__ __volatile__(
		/* FIXME: do we have to isync for every one? */
		"isync			\n\t"
		"slbmte		%0,%1	\n\t"
		"isync			\n\t"
		:
		: "r" (cache[index].words.vsid), "r" (cache[index].words.esid)
		: "memory");
	/* *INDENT-ON* */
}

static inline void
get_slb_entry(uval index, union slb_entry *cache)
{

	/* *INDENT-OFF* */
	__asm__ __volatile__("\n\t"
			     "isync		\n\t"
			     "slbmfee  %0,%2	\n\t"
			     "slbmfev  %1,%2	\n\t"
			     "isync		\n\t"
			     : "=r" (cache[index].words.esid),
			       "=r" (cache[index].words.vsid)
			     : "r" (index)
			     : "memory");
	/* *INDENT-ON* */
}



void hwMapRange(uval eaddr, uval64 raddr, uval size, /* unused */ int bolted);

/* If vsid == 0 slb insert will make up it's own value */
extern int slb_insert(uval ea, uval8 is_lp, uval8 lp_selection, uval kp,
		      uval vsid, union slb_entry* slbe_cache);

extern int slb_insert_to_slot(uval slot, uval ea, uval8 is_lp,
			      uval8 lp_selection,
			      uval kp, uval vsid, union slb_entry* slbe_cache);

extern uval get_esid(uval ea);
extern uval get_vsid(uval ea);
extern uval get_pteg(uval vaddr, uval vsid, uval pg_size, uval log_htab_bytes);

extern uval pteg_idx(uval vaddr, uval vsid);
extern int pte_find_free(struct partition_info *pi, uval eaddr,
			 uval8 l, uval8 ls);
extern int pte_find_entry(struct partition_info *pi, uval eaddr,
			  uval8 l, uval8 ls);
extern int pte_enter(struct partition_info *pi, uval eaddr, uval raddr,
		     uval flags, int bolted, uval8 l, uval8 ls);
extern int pte_remove(int ptex);
extern void *iomap(struct partition_info *pi,
		   uval raddr, uval size, int bolted, uval8 l, uval8 ls);
extern void *iomap_ptx(struct partition_info *pi,
		       uval raddr, uval size, int bolted,
		       uval8 l, uval8 ls, int ptx_num, int *ptx_list);
extern void iounmap_ptx(int ptx_num, int *ptx_list);

extern int tlb_enter(struct partition_info *pi,
		     uval eaddr, uval raddr, uval flags, uval8 l, uval8 ls);

static __inline__ void
tlbie(uval64 ea)
{
	__asm__ __volatile__(
			"tlbie %0"
			: /* no outputs */
			: "r"(ea)
			: "memory"
			);
}

static __inline__ void
tlbie_large(uval64 ea)
{
	__asm__ __volatile__(
			"tlbie %0,1"
			: /* no outputs */
			: "r"(ea)
			: "memory"
			);
}

static __inline__ void
eieio(void)
{
	__asm__ __volatile__("eieio");
}

/* syncs */
static __inline__ void
sync(void)
{
	__asm__ __volatile__("sync");
}

static __inline__ void
isync(void)
{
	__asm__ __volatile__("isync");
}

/* cache */

static __inline__ void
dcbst(uval line)
{
	__asm__ __volatile__("dcbst 0, %0"	:
				: "r" (line)
				: "memory");
}

static __inline__ void
dcbz(uval line)
{
	__asm__ __volatile__("dcbz 0, %0" :
				: "r" (line)
				: "memory");
}

/*
 * XXX Are the address calculations from the RPN here correct?
 */

static __inline__ void
clear_page(uval rpn, int pgshift)
{
	uval k;
	uval page_size = 1 << pgshift;
	uval addr = rpn << LOG_PGSIZE;

	for (k = 0; k < page_size; k += CACHE_LINE_SIZE)
		dcbz(addr + k);
}

static __inline__ void
icache_invalidate_page(uval rpn, int pgshift)
{
	uval k;
	uval page_size = 1 << pgshift;
	uval addr = rpn << LOG_PGSIZE;

	/*
	 * The instr. sequence for icache invalidation given in
	 * book II, sec. 3.2.1, p17, iterated over all blocks within
	 * the page.
	 */
	for (k = 0; k < page_size; k += CACHE_LINE_SIZE) {
		dcbst(addr + k);
		sync();
		icbi(addr + k);
		sync();
		isync();
	}
}

static __inline__ void
icache_synchronize_page(uval rpn, int pgshift)
{
	uval k;
	uval page_size = 1 << pgshift;
	uval addr = rpn << LOG_PGSIZE;

	/*
	 * The instr. sequence for icache invalidation given in
	 * book II, sec. 3.2.1, p17, iterated over all blocks within
	 * the page, except omitting the dcbst(); sync(); portion.
	 * XXX Is this the correct distinction?
	 */
	for (k = 0; k < page_size; k += CACHE_LINE_SIZE) {
		icbi(addr + k);
		sync();
		isync();
	}
}

/*
 * initialize the specified PTE
 *
 *	pte - pointer to PTE to initialize
 *	eaddr - effective address being mapped
 *	raddr - real address being mapped
 *	vsid - vsid from the approprite STE
 *	bolted - should this PTE be bolted?
 * 	lp - Large Page (1)
 */
static __inline__ void
pte_init(union pte *pte, uval eaddr, uval raddr, uval vsid, int bolted,
	 uval8 is_large, uval8 lp_selection)
{
	pte->words.vsidWord = 0;
	pte->words.rpnWord = 0;
	pte->bits.avpn = vsid << 5;	/* shift past API */
	pte->bits.avpn |= VADDR_TO_API(eaddr);
	pte->bits.bolted = bolted ? 1 : 0;
	pte->bits.l = is_large ? 1 : 0;
	pte->bits.h = 0;
	pte->bits.v = 1;
	pte->bits.rpn = (raddr >> RPN_SHIFT);
	if (is_large) {
		uval8 lpbits = 0;

		while (lp_selection--)
			lpbits = ((lpbits << 1) | 1);
		pte->bits.rpn |= lpbits;
	}
	pte->bits.r = 0;	/* referenced */
	pte->bits.c = 0;	/* changed */
	pte->bits.pp0 = PP_RWRW & 0x1UL;
	pte->bits.pp1 = (PP_RWRW >> 1) & 0x3UL;
}

/* large page bits */
static inline int
get_large_page_shift(uval large_page_size)
{
	if (large_page_size == LARGE_PAGE_SIZE64K) {
		return 16;
	} else if (large_page_size == LARGE_PAGE_SIZE1M) {
		return 20;
	} else if (large_page_size == LARGE_PAGE_SIZE16M) {
		return 24;
	}

	return 0;
}

/* large page bits */
static inline int
get_large_page_hid_bits(uval large_page_size)
{
	if (large_page_size == LARGE_PAGE_SIZE64K) {
		return 2;
	} else if (large_page_size == LARGE_PAGE_SIZE1M) {
		return 1;
	} else if (large_page_size == LARGE_PAGE_SIZE16M) {
		return 0;
	}

	return 0;
}

static inline uval
get_log_pgsize(struct partition_info *pi, uval8 l, uval8 ls)
{
	uval lp = 12;

	if (l) {
		if (ls == 0) {
			lp = get_large_page_shift(pi->large_page_size1);
		} else {
			lp = get_large_page_shift(pi->large_page_size2);
		}
	}
	return lp;
}

#endif /* ! __POWERPC_MMU_H_ */
