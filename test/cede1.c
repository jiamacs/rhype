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
 * "Smoke" test of an OS yielding when there is no other OS to yield to.
 *
 */
#include <test.h>
#include <lpar.h>
#include <hypervisor.h>
#include <hcall.h>

#if 0
uval
test_os(void)
{
	reg_t mylpid;
	reg_t regsave[32];
	int i=0;
	(void)hcall_get_lpid(&mylpid);

	ctxt_count = ((uval64)mylpid) << 32;
	hputs("You should see repeated 'SUCCESS' messages.\n"
	      "This test will run until manually halted.\n");
	if (mfpir() == 0)
	    hcall_thread_control(regsave, HTC_START, 1, 0);
	while (1) {
	    hprintf("multi: %d:%d\n", mfpir(), i);
	    ++i;
	}

	return 0;
}
#endif


struct partition_info pinfo[2] = {{
	.sfw_tlb = 1,
	.large_page_size1 = LARGE_PAGE_SIZE64K,
	.large_page_size2 = LARGE_PAGE_SIZE16M
},};


const char str1[] = "Thread 0.\n";
const char str2[] = "Thread 1.\n";

uval
test_os(uval argc __attribute__ ((unused)),
	uval argv[] __attribute__ ((unused)))
{
	int i = 0;
	const char *string;
	int thread = 0;

	if (mfpir() == 0)
	{
	    string = str1;
	    hcall_put_term_string(0, 10, string);
	    hcall_thread_control(NULL, HTC_START, 1, 0x100);
	}
	else
	{
	    thread = 1;
	    string = str2;
	    hcall_put_term_string(0, 10, string);
	}

	while (1) {
	    ++i;
	    if ((i % 5000) == 0)
	    {
		hcall_put_term_string(0, 10, string);
		if(thread == 1) {
		    hputs("calling cede\n");
		    hcall_cede(0);
		    i = 4990;
		}
	    }
	}
	return 0;
}

