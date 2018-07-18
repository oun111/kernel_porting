
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "mm_porting.h"

void* __malloc__(size_t sz)
{
  /* FIXME */
  return malloc(sz);
}

void __free__(void *p)
{
  /* FIXME */
  free(p);
}

struct kmem_cache *
kmem_cache_create(const char *name, size_t size, size_t align,
      unsigned long flags, void (*ctor)(void *))
{
  struct kmem_cache *p = __malloc__(sizeof(struct kmem_cache));

  strcpy(p->name,name);
  p->size = size ;

  return p;
}

void* kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
{
  if (!cachep) {
    return NULL ;
  }

  return __malloc__(cachep->size) ;
}

void kmem_cache_free(struct kmem_cache *cachep, void *objp)
{
  if (cachep) {
    __free__(objp) ;
  }
}

void* kmalloc(size_t size, gfp_t flags)
{
  return __malloc__(size);
}

void kfree(const void *objp)
{
  __free__((void*)objp);
}
