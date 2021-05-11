/* 
 * This file is part of the Nautilus AeroKernel developed
 * by the Hobbes and V3VEE Projects with funding from the 
 * United States National  Science Foundation and the Department of Energy.  
 *
 * The V3VEE Project is a joint project between Northwestern University
 * and the University of New Mexico.  The Hobbes Project is a collaboration
 * led by Sandia National Laboratories that includes several national 
 * laboratories and universities. You can find out more at:
 * http://www.v3vee.org  and
 * http://xtack.sandia.gov/hobbes
 *
 * Copyright (c) 2018, Peter Dinda <pdinda@northwestern.edu>
 *                     Mike Leonard <MichaelLeonard2018@u.northwestern.edu>
 *                     John Albers  <JohnAlbers2018@u.northwestern.edu>
 * Copyright (c) 2018, The V3VEE Project  <http://www.v3vee.org> 
 *                     The Hobbes Project <http://xstack.sandia.gov/hobbes>
 * All rights reserved.
 *
 * Authors: Peter Dinda <pdinda@northwestern.edu>
 *          Mike Leonard <MichaelLeonard2018@u.northwestern.edu>
 *          John Albers  <JohnAlbers2018@u.northwestern.edu>
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "LICENSE.txt".
 */


// Assume Gem5, full system simulation, x86, with kernel pointed at nautilus.bin

// In this startup mode, we are already in long mode, and we have no multiboot support
	
#include <asm/lowlevel.h>

.section .boot

GLOBAL(nautilus_entry)

	la sp, stack0
	li a0, 1024 * 4
	csrr a1, mhartid
	addi a1, a1, 1
	mul a0, a0, a1
	add sp, sp, a0

	call main
