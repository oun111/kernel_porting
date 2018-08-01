
#ifndef __MM_PORTING_H__
#define __MM_PORTING_H__

#include "porting.h"

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

extern void* kmalloc(size_t size, gfp_t flags);

extern void kfree(const void *objp);

#endif
