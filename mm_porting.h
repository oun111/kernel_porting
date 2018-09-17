
#ifndef __MM_PORTING_H__
#define __MM_PORTING_H__

#include "porting.h"


#define SIZE_ALIGNED(sz) ({\
  const size_t __BOUNDARY =  \
    ((sz)<8 ? 8 : \
    (sz)<16 ? 16 : \
    (sz)<32 ? 32 : 64) - 1 ;\
  ((sz) + __BOUNDARY) & (~__BOUNDARY) ; \
})

/* FIXME: dummy */
struct kmem_cache {
  char name[64];
  size_t size ;
} ;

extern struct kmem_cache *
kmem_cache_create(const char *name, size_t size, size_t align,
      unsigned long flags, void (*ctor)(void *));

extern void kmem_cache_destroy(struct kmem_cache *s);

extern void* kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags);

extern void kmem_cache_free(struct kmem_cache *cachep, void *objp);

#if 1
extern void* kmalloc(size_t size, gfp_t flags);

extern void kfree(const void *objp);

extern void* krealloc(void *p, size_t newsize, gfp_t flags);
#else
#include <stdio.h>

extern void* __malloc__(size_t sz) ;
extern void __free__(void*) ;

#define kmalloc(sz,f) ({\
  printf("kmalloc caller %s\n",__func__);  \
  __malloc__(sz); \
})
#define kfree(p) ({\
  printf("kfree caller %s\n",__func__);  \
  __free__(p); \
})
#endif

#endif
