
#ifndef __PORTING_H__
#define __PORTING_H__


#ifdef __KERNEL__
# error "should NOT include this file in kernel compiling"
#endif

#include <stdbool.h>
#include <asm-generic/int-ll64.h>
#include "compiler.h"

#undef __BIG_ENDIAN

/**
 * porting from include/linux/poison.h
 */

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

//



/**
 * porting from include/linux/mmzone.h
 */
enum migratetype {
  MIGRATE_UNMOVABLE,
  MIGRATE_MOVABLE,
  MIGRATE_RECLAIMABLE,
  MIGRATE_PCPTYPES, /* the number of types on the pcp lists */
  MIGRATE_HIGHATOMIC = MIGRATE_PCPTYPES,
#ifdef CONFIG_CMA
  /*
   * MIGRATE_CMA migration type is designed to mimic the way
   * ZONE_MOVABLE works.  Only movable pages can be allocated
   * from MIGRATE_CMA pageblocks and page allocator never
   * implicitly change migration type of MIGRATE_CMA pageblock.
   *
   * The way to use it is to change migratetype of a range of
   * pageblocks to MIGRATE_CMA which can be done by
   * __free_pageblock_cma() function.  What is important though
   * is that a range of pageblocks must be aligned to
   * MAX_ORDER_NR_PAGES should biggest page be bigger then
   * a single pageblock.
   */
  MIGRATE_CMA,
#endif
#ifdef CONFIG_MEMORY_ISOLATION
  MIGRATE_ISOLATE,  /* can't allocate from here */
#endif
  MIGRATE_TYPES
};

//



#ifndef rcu_assign_pointer
# define rcu_assign_pointer(p,v)  WRITE_ONCE((p),(v))
#endif

#undef EXPORT_SYMBOL
#define EXPORT_SYMBOL(x)

#undef EXPORT_SYMBOL_GPL
#define EXPORT_SYMBOL_GPL(x)

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

#ifndef VM_WARN_ON
# define VM_WARN_ON(cond) WARN_ON(cond)
#endif


/**
 * porting from include/linux/types.h
 */
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

#define DECLARE_BITMAP(name,bits) \
  unsigned long name[BITS_TO_LONGS(bits)]

//



#ifndef CONFIG_BASE_SMALL
# define CONFIG_BASE_SMALL  0
#endif

#ifndef CONFIG_64BIT
# define CONFIG_64BIT 1
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

typedef __u8 u8 ;
typedef __u16 u16 ;
typedef __u32 u32 ;
typedef __u64 u64 ;

#ifndef preempt_enable
# define preempt_enable() 
#endif

#ifndef preempt_disable
# define preempt_disable()
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

#ifndef __init
#define __init
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



/* FIXME: no rcu here */
#ifndef call_rcu
#define call_rcu(rcu_head,func)  radix_tree_node_rcu_free((rcu_head))
#endif



/**
 * porting from include/asm-generic/page.h
 */
#define PAGE_SHIFT  12
#ifdef __ASSEMBLY__
#define PAGE_SIZE (1 << PAGE_SHIFT)
#else
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#endif
#define PAGE_MASK (~(PAGE_SIZE-1))


/**
 * porting from include/linux/slab.h
 */
#define SLAB_PANIC    0x00040000UL  /* Panic if kmem_cache_create() fails */

#define SLAB_RECLAIM_ACCOUNT  0x00020000UL    /* Objects are reclaimable */

//



/* FIXME */
#ifndef cpuhp_setup_state_nocalls
# define cpuhp_setup_state_nocalls(state,name,startup,teardown) ({\
  (void)startup;\
  (void)teardown;\
  0;\
})
#endif

#ifndef local_irq_save
# define local_irq_save(f) ({\
  (void)f; \
})
#endif

#ifndef local_irq_restore
# define local_irq_restore(f) ({\
  (void)f;\
})
#endif

#ifndef raw_local_irq_save
# define raw_local_irq_save(f) ({\
  (void)f; \
})
#endif

#ifndef raw_local_irq_restore
# define raw_local_irq_restore(f) ({\
  (void)f;\
})
#endif

/**
 * porting from include/linux/err.h
 */
static inline void * __must_check ERR_PTR(long error)
{
  return (void *) error;
}

//


/**
 * porting from include/linux/build_bug.h
 */
#ifdef __CHECKER__
#define BUILD_BUG_ON_ZERO(e) (0)
#else
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:(-!!(e)); }))
#endif

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#define BUILD_BUG_ON_MSG(cond, msg) /*(0)*/

//


#ifndef raw_copy_to_user
# define raw_copy_to_user(dst, src, size) memcpy(dst,src,size)
#endif

#ifndef raw_copy_from_user
# define raw_copy_from_user(dst, src, size) memcpy(dst,src,size)
#endif


#endif /* __PORTING_H__*/

