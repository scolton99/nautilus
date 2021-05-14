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
 * http://xstack.sandia.gov/hobbes
 *
 * Copyright (c) 2015, Kyle C. Hale <khale@cs.iit.edu>
 * Copyright (c) 2015, The V3VEE Project  <http://www.v3vee.org>
 *                     The Hobbes Project <http://xstack.sandia.gov/hobbes>
 * All rights reserved.
 *
 * Author: Kyle C. Hale <khale@cs.iit.edu>
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "LICENSE.txt".
 */
#ifndef __CPU_H__
#define __CPU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nautilus/naut_types.h>
#include <arch/riscv/riscv.h>

ulong_t nk_detect_cpu_freq(uint32_t);

struct nk_regs {
  /*   0 */ ulong_t zero;
  /*   8 */ ulong_t ra;
  /*  16 */ ulong_t sp;
  /*  24 */ ulong_t gp;
  /*  32 */ ulong_t tp;
  /*  40 */ ulong_t t0;
  /*  48 */ ulong_t t1;
  /*  56 */ ulong_t t2;
  /*  64 */ ulong_t s0;
  /*  72 */ ulong_t s1;
  /*  80 */ ulong_t a0;
  /*  88 */ ulong_t a1;
  /*  96 */ ulong_t a2;
  /* 104 */ ulong_t a3;
  /* 112 */ ulong_t a4;
  /* 120 */ ulong_t a5;
  /* 128 */ ulong_t a6;
  /* 136 */ ulong_t a7;
  /* 144 */ ulong_t s2;
  /* 152 */ ulong_t s3;
  /* 160 */ ulong_t s4;
  /* 168 */ ulong_t s5;
  /* 176 */ ulong_t s6;
  /* 184 */ ulong_t s7;
  /* 192 */ ulong_t s8;
  /* 200 */ ulong_t s9;
  /* 208 */ ulong_t s10;
  /* 216 */ ulong_t s11;
  /* 224 */ ulong_t t3;
  /* 232 */ ulong_t t4;
  /* 240 */ ulong_t t5;
  /* 248 */ ulong_t t6;
};


#define PAUSE_WHILE(x) \
    while ((x)) { \
        asm volatile("nop"); \
    }

#define mbarrier()

#define BARRIER_WHILE(x) \
    while ((x)) { \
        mbarrier(); \
    }


static inline uint8_t
inb (uint64_t addr)
{
    uint8_t ret;
    asm volatile ("lb  %[_r], 0(%[_a])"
                  : [_r] "=r" (ret)
                  : [_a] "r" (addr));
    return ret;
}


static inline uint16_t
inw (uint64_t addr)
{
    uint16_t ret;
    asm volatile ("lh  %[_r], 0(%[_a])"
                  : [_r] "=r" (ret)
                  : [_a] "r" (addr));
    return ret;
}


static inline uint32_t
inl (uint64_t addr)
{
    uint32_t ret;
    asm volatile ("lw  %[_r], 0(%[_a])"
                  : [_r] "=r" (ret)
                  : [_a] "r" (addr));
    return ret;
}


static inline void
outb (uint8_t val, uint64_t addr)
{
    asm volatile ("sb  %[_v], 0(%[_a])"
                  :
                  : [_a] "r" (addr),
                    [_v] "r" (val));
}


static inline void
outw (uint16_t val, uint64_t addr)
{
    asm volatile ("sh  %[_v], 0(%[_a])"
                  :
                  : [_a] "r" (addr),
                    [_v] "r" (val));
}

static inline void
outl (uint32_t val, uint64_t addr)
{
    asm volatile ("sw  %[_v], 0(%[_a])"
                  :
                  : [_a] "r" (addr),
                    [_v] "r" (val));
}

static inline void
sti (void)
{
    intr_on();
}


static inline void
cli (void)
{
    intr_off();
}


static inline uint64_t __attribute__((always_inline))
rdtsc (void)
{
    return r_time();
}


static inline uint64_t
rdtscp (void)
{
    uint32_t lo, hi;
    asm volatile("rdtscp" : "=a"(lo), "=d"(hi));
    return lo | ((uint64_t)(hi) << 32);
}


static inline uint64_t
read_rflags (void)
{
    uint64_t ret;
    asm volatile ("pushfq; popq %0" : "=a"(ret));
    return ret;
}

static inline void
halt (void)
{
    asm volatile ("wfi");
}


static inline void
invlpg (unsigned long addr)
{
    asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}


static inline void
wbinvd (void)
{
    asm volatile("wbinvd" : : : "memory");
}


static inline void clflush(void *ptr)
{
    __asm__ __volatile__ ("clflush (%0); "
			  : : "r"(ptr) : "memory");

}

static inline void clflush_unaligned(void *ptr, int size)
{
    clflush(ptr);
    if ((addr_t)ptr % size) {
	// ptr is misaligned, so be paranoid since we
	// may be spanning a cache line
	clflush((void*)((addr_t)ptr+size-1));
    }
}

/**
 * Flush all non-global entries in the calling CPU's TLB.
 *
 * Flushing non-global entries is the common-case since user-space
 * does not use global pages (i.e., pages mapped at the same virtual
 * address in *all* processes).
 *
 */
static inline void
tlb_flush(void)
{
  // the zero, zero means flush all TLB entries.
  asm volatile("sfence.vm");
}


static inline void io_delay(void)
{
    const uint64_t DELAY_PORT = 0x80;
    outb(0, DELAY_PORT);
}


static void udelay(uint_t n) {
    while (n--){
        io_delay();
    }
}

#ifdef __cplusplus
}
#endif


#endif
