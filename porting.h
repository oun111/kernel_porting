
#ifndef __PORTING_H__
#define __PORTING_H__


#ifdef __KERNEL__
#error "should NOT include this file in kernel compiling"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "mem.h"

/********** include/linux/list.h **********/

/*
 * Architectures might want to move the poison pointer offset
 * into some well-recognized area such as 0xdead000000000000,
 * that is also not mappable by user-space exploits:
 */
#ifdef CONFIG_ILLEGAL_POINTER_VALUE
# define POISON_POINTER_DELTA _AC(CONFIG_ILLEGAL_POINTER_VALUE, UL)
#else
# define POISON_POINTER_DELTA 0
#endif
  
/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define LIST_POISON1  ((void *) 0x100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *) 0x200 + POISON_POINTER_DELTA)

/* porting from include/linux/gfp.h */
/*
 * In case of changes, please don't forget to update
 * include/trace/events/mmflags.h and tools/perf/builtin-kmem.c
 */

/* Plain integer GFP bitmasks. Do not use this directly. */
#define ___GFP_DMA    0x01u
#define ___GFP_HIGHMEM    0x02u
#define ___GFP_DMA32    0x04u
#define ___GFP_MOVABLE    0x08u
#define ___GFP_RECLAIMABLE  0x10u
#define ___GFP_HIGH   0x20u
#define ___GFP_IO   0x40u
#define ___GFP_FS   0x80u
#define ___GFP_COLD   0x100u
#define ___GFP_NOWARN   0x200u
#define ___GFP_RETRY_MAYFAIL  0x400u
#define ___GFP_NOFAIL   0x800u
#define ___GFP_NORETRY    0x1000u
#define ___GFP_MEMALLOC   0x2000u
#define ___GFP_COMP   0x4000u
#define ___GFP_ZERO   0x8000u
#define ___GFP_NOMEMALLOC 0x10000u
#define ___GFP_HARDWALL   0x20000u
#define ___GFP_THISNODE   0x40000u
#define ___GFP_ATOMIC   0x80000u
#define ___GFP_ACCOUNT    0x100000u
#define ___GFP_DIRECT_RECLAIM 0x400000u
#define ___GFP_WRITE    0x800000u
#define ___GFP_KSWAPD_RECLAIM 0x1000000u
#ifdef CONFIG_LOCKDEP
#define ___GFP_NOLOCKDEP  0x2000000u
#else
#define ___GFP_NOLOCKDEP  0
#endif

#define __GFP_IO  ((__force gfp_t)___GFP_IO)
#define __GFP_FS  ((__force gfp_t)___GFP_FS)
#define __GFP_DIRECT_RECLAIM  ((__force gfp_t)___GFP_DIRECT_RECLAIM) /* Caller can reclaim */
#define __GFP_KSWAPD_RECLAIM  ((__force gfp_t)___GFP_KSWAPD_RECLAIM) /* kswapd can wake */
#define __GFP_RECLAIM ((__force gfp_t)(___GFP_DIRECT_RECLAIM|___GFP_KSWAPD_RECLAIM))
#define __GFP_RETRY_MAYFAIL ((__force gfp_t)___GFP_RETRY_MAYFAIL)
#define __GFP_NOFAIL  ((__force gfp_t)___GFP_NOFAIL)
#define __GFP_NORETRY ((__force gfp_t)___GFP_NORETRY)

#define GFP_KERNEL  (__GFP_RECLAIM | __GFP_IO | __GFP_FS)

#ifndef WRITE_ONCE
# define WRITE_ONCE(x,val) (x)=(val)
#endif

#ifndef READ_ONCE
# define READ_ONCE(x) (x)
#endif

#ifndef rcu_assign_pointer
# define rcu_assign_pointer(p,v)  WRITE_ONCE((p),(v))
#endif

#ifndef __always_inline
# define __always_inline   inline __attribute__((always_inline))
#endif

#ifndef likely
# define likely(x)  __builtin_expect(!!(x),1)
#endif

#ifndef unlikely
# define unlikely(x)  __builtin_expect(!!(x),0)
#endif

#undef EXPORT_SYMBOL
#define EXPORT_SYMBOL(x)

#ifndef WARN_ON_ONCE
#define WARN_ON_ONCE(x)  ({\
  !!(x)?1:0; \
})
#endif

#ifndef WARN_ON
#define WARN_ON(x)  ({\
  !!(x)?1:0; \
})
#endif

/**
 * struct callback_head - callback structure for use with RCU and task_work
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 *
 * The struct is aligned to size of pointer. On most architectures it happens
 * naturally due ABI requirements, but some architectures (like CRIS) have
 * weird ABI and we need to ask it explicitly.
 *
 * The alignment is required to guarantee that bit 0 of @next will be
 * clear under normal conditions -- as long as we use call_rcu(),
 * call_rcu_bh(), call_rcu_sched(), or call_srcu() to queue callback.
 *
 * This guarantee is important for few reasons:
 *  - future call_rcu_lazy() will make use of lower bits in the pointer;
 *  - the structure shares storage spacer in struct page with @compound_head,
 *    which encode PageTail() in bit 0. The guarantee is needed to avoid
 *    false-positive PageTail().
 */
struct callback_head {
  struct callback_head *next;
  void (*func)(struct callback_head *head);
} __attribute__((aligned(sizeof(void *))));
#define rcu_head callback_head

#ifndef __rcu
# define __rcu
#endif

#ifndef CONFIG_BASE_SMALL
# define CONFIG_BASE_SMALL  0
#endif

#ifndef BITS_PER_LONG
#define BITS_PER_LONG 64
#endif

#ifndef BIT_WORD
#define BIT_WORD(nr)    ((nr) / BITS_PER_LONG) 
#endif

#ifndef BIT_MASK
#define BIT_MASK(nr)    (1UL << ((nr) % BITS_PER_LONG))
#endif

#ifndef __bitwise
# define __bitwise
#endif

#ifndef rcu_dereference
#define rcu_dereference(p)  READ_ONCE(p)
#endif

#ifndef rcu_dereference_raw
#define rcu_dereference_raw(p)  READ_ONCE(p)
#endif

typedef unsigned __bitwise gfp_t;

typedef uint32_t u32 ;
typedef uint64_t u64 ;

typedef u32 __u32 ;
typedef u64 __u64 ;

#ifndef preempt_enable
# define preempt_enable() 
#endif

#ifndef preempt_disable
# define preempt_disable()
#endif

#ifndef INT_MAX
# define INT_MAX   ((int)(~0U>>1))
#endif

#ifndef __must_check
# define __must_check
#endif

#ifndef DIV_ROUND_UP
# define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif

#ifndef __read_mostly
# define __read_mostly __attribute__((__section__(".data..read_mostly")))
#endif

/* FIXME */
#ifndef DEFINE_PER_CPU
# define DEFINE_PER_CPU(type,name)  __typeof__(type) name
#endif

/* FIXME */
#ifndef DECLARE_PER_CPU
# define DECLARE_PER_CPU(type,name)  extern __typeof__(type) name
#endif

/* FIXME */
#ifndef this_cpu_read
# define this_cpu_read(pcp)  pcp
#endif

/* FIXME */
#ifndef this_cpu_cmpxchg
# define this_cpu_cmpxchg(pcp,oval,nval)  __sync_bool_compare_and_swap(&pcp,oval,nval)
#endif

/* FIXME */
#ifndef per_cpu
# define per_cpu(var,cpu)  var
#endif

#ifndef IS_ENABLED
#define IS_ENABLED(option)  0
#endif

#ifndef __force
#define __force
#endif

#ifndef __init
#define __init
#endif

#define BITS_PER_BYTE   8
#define BITS_TO_LONGS(nr) DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))

#define small_const_nbits(nbits) \
  (__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

/* Room for N __GFP_FOO bits */
#define __GFP_BITS_SHIFT (25 + IS_ENABLED(CONFIG_LOCKDEP))
#define __GFP_BITS_MASK ((__force gfp_t)((1 << __GFP_BITS_SHIFT) - 1))

/* FIXME */
#ifndef gfpflags_allow_blocking
# define gfpflags_allow_blocking(f)   0
#endif

#ifndef in_interrupt
# define in_interrupt()  0
#endif

#ifndef kmemleak_update_trace
#define kmemleak_update_trace(p)
#endif

#ifndef this_cpu_ptr
#define this_cpu_ptr(p)  (p)
#endif

#ifndef BUG_ON
#define BUG_ON(c)
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)
#endif

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:  the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({        \
  void *__mptr = (void *)(ptr);         \
  ((type *)(__mptr - offsetof(type, member))); })

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/* FIXME: no rcu here */
#ifndef call_rcu
#define call_rcu(rcu_head,func)  radix_tree_node_rcu_free((rcu_head))
#endif

/*
 * The IDR API does not expose the tagging functionality of the radix tree
 * to users.  Use tag 0 to track whether a node has free space below it.
 */
#define IDR_FREE  0

/*
 * A trick to suppress uninitialized variable warning without generating any
 * code
 */
#define uninitialized_var(x) x = x

#ifndef max
# define max(m0,m1)  (m0)>(m1)?(m0):(m1)
#endif

#ifndef min
# define min(m0,m1)  (m0)<(m1)?(m0):(m1)
#endif

#define SLAB_PANIC    0x00040000UL  /* Panic if kmem_cache_create() fails */

#define SLAB_RECLAIM_ACCOUNT  0x00020000UL    /* Objects are reclaimable */

/* FIXME */
#ifndef cpuhp_setup_state_nocalls
# define cpuhp_setup_state_nocalls(state,name,startup,teardown) ({\
  (void)startup;\
  (void)teardown;\
  0;\
})
#endif

struct kmem_cache {
  char name[64];
  size_t size ;
} ;

/**
 * __ffs - find first set bit in word
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static __always_inline unsigned long __ffs(unsigned long word)
{
  asm("rep; bsf %1,%0"
    : "=r" (word)
    : "rm" (word));
  return word;
}

/*
 * __fls: find last set bit in word
 * @word: The word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
static __always_inline unsigned long __fls(unsigned long word)
{
  asm("bsr %1,%0"
      : "=r" (word)
      : "rm" (word));
  return word;
}

/**
 * fls - find last set bit in word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffs, but returns the position of the most significant set bit.
 *
 * fls(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 32.
 */
static __always_inline int fls(int x)
{
  int r;

#ifdef CONFIG_X86_64
  /*
   * AMD64 says BSRL won't clobber the dest reg if x==0; Intel64 says the
   * dest reg is undefined if x==0, but their CPU architect says its
   * value is written to set it to the same as before, except that the
   * top 32 bits will be cleared.
   *
   * We cannot do this on 32 bits because at the very least some
   * 486 CPUs did not behave this way.
   */
  asm("bsrl %1,%0"
      : "=r" (r)
      : "rm" (x), "0" (-1));
#elif defined(CONFIG_X86_CMOV)
  asm("bsrl %1,%0\n\t"
      "cmovzl %2,%0"
      : "=&r" (r) : "rm" (x), "rm" (-1));
#else
  asm("bsrl %1,%0\n\t"
      "jnz 1f\n\t"
      "movl $-1,%0\n"
      "1:" : "=r" (r) : "rm" (x));
#endif
  return r + 1;
}

/**
 * fls64 - find last set bit in a 64-bit word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffsll, but returns the position of the most significant set bit.
 *
 * fls64(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 64.
 */
#if BITS_PER_LONG == 32
static __always_inline int fls64(__u64 x)
{
  __u32 h = x >> 32; 
  if (h) 
    return fls(h) + 32; 
  return fls(x);
}
#elif BITS_PER_LONG == 64
static __always_inline int fls64(__u64 x)
{
  if (x == 0)
    return 0;
  return __fls(x) + 1;
}
#else
#error BITS_PER_LONG not 32 or 64
#endif

static inline unsigned fls_long(unsigned long l)
{
  if (sizeof(l) == 4)
    return fls(l);
  return fls64(l);
}

/**
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __set_bit(int nr, volatile unsigned long *addr)
{
  unsigned long mask = BIT_MASK(nr);
  unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

  *p  |= mask;
}

static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
  unsigned long mask = BIT_MASK(nr);
  unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

  *p &= ~mask;
}

/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
  return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

static inline void bitmap_fill(unsigned long *dst, unsigned int nbits)
{
  unsigned int nlongs = BITS_TO_LONGS(nbits);
  if (!small_const_nbits(nbits)) {
    unsigned int len = (nlongs - 1) * sizeof(unsigned long);
    memset(dst, 0xff,  len);
  }
  dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}

static inline void * __must_check ERR_PTR(long error)
{
  return (void *) error;
}

static struct kmem_cache *
kmem_cache_create(const char *name, size_t size, size_t align,
      unsigned long flags, void (*ctor)(void *))
{
  struct kmem_cache *p = __malloc(sizeof(struct kmem_cache));

  strcpy(p->name,name);
  p->size = size ;

  return p;
}

static void* kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
  if (!cachep) {
    return NULL ;
  }

  return __malloc(cachep->size) ;
}

static void kmem_cache_free(struct kmem_cache *cachep, void *objp)
{
  if (cachep) {
    __free(objp) ;
  }
}

static void* kmalloc(size_t size, gfp_t flags)
{
  return __malloc(size);
}

static void kfree(const void *objp)
{
  __free((void*)objp);
}

#endif /* __PORTING_H__*/

