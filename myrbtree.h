
#ifndef __MYRBTREE_H__
#define __MYRBTREE_H__


#if 0
#define MY_RB_TREE_INSERT(__root__, __i__, __k__, __nd__) ({  \
  struct rb_node **__p__ = &(__root__)->rb_node, *__parent__ = 0 ;  \
  typeof(__i__) __prt__ ;  \
  while (*__p__) {  \
    __parent__  = *__p__ ;  \
    __prt__ = rb_entry(__parent__,typeof((*(__i__))),__nd__);  \
    if (__prt__->__k__ < (__i__)->__k__) {  \
      __p__ = &__parent__->rb_left ;  \
    } else {   \
      __p__ = &__parent__->rb_right ;  \
    }  \
  }   \
  /* insert new __nd__ */   \
  (__i__)->__nd__.__rb_parent_color = (unsigned long)__parent__; /* FIXME: why ?? */  \
  (__i__)->__nd__.rb_left = (__i__)->__nd__.rb_right = NULL ;   \
  *__p__ = &(__i__)->__nd__ ;   \
  /* change color */    \
  rb_insert_color(&(__i__)->__nd__,__root__);  \
  0;  \
})


#define MY_RB_TREE_FIND(__root__, __key__, __res__, __k__, __nd__) ({  \
  struct rb_node **__p__ = &(__root__)->rb_node, *__parent__ = 0 ;  \
  typeof(__res__) __prt__ ;   \
  int __rc__ = 0;   \
  while (*__p__) {   \
    __parent__  = *__p__ ;   \
    __prt__ = rb_entry(__parent__,typeof((*(__res__))),__nd__);  \
    if (__prt__->__k__ < __key__)  __p__ = &__parent__->rb_left ;    \
    else if (__prt__->__k__ > __key__)  __p__ = &__parent__->rb_right ;   \
    else  break ;   \
  }   \
  /* not found */  \
  if (!(*__p__)) __rc__ = -1;   \
  else  __res__ = __prt__ ; \
  __rc__;   \
})
#else

#define MY_RB_TREE_INSERT(__root__, __i__, __k__, __nd__,cmp) ({  \
  struct rb_node **__p__ = &(__root__)->rb_node, *__parent__ = 0 ;  \
  typeof(__i__) __prt__ ;  \
  while (*__p__) {  \
    __parent__  = *__p__ ;  \
    __prt__ = rb_entry(__parent__,typeof((*(__i__))),__nd__);  \
    if (cmp(__prt__->__k__, (__i__)->__k__)<0) {  \
    /*if (__prt__->__k__< (__i__)->__k__) {*/  \
      __p__ = &__parent__->rb_left ;  \
    } else {   \
      __p__ = &__parent__->rb_right ;  \
    }  \
  }   \
  /* insert new __nd__ */   \
  rb_link_node(&(__i__)->__nd__,__parent__,__p__); \
  /* change color */    \
  rb_insert_color(&(__i__)->__nd__,__root__);  \
  0;  \
})

#define MY_RB_TREE_FIND(__root__, __key__, __res__, __k__, __nd__,cmp) ({  \
  struct rb_node **__p__ = &(__root__)->rb_node, *__parent__ = 0 ;  \
  typeof(__res__) __prt__ ;   \
  int __rc__ = 0, __r__ = 0;   \
  while (*__p__) {   \
    __parent__  = *__p__ ;   \
    __prt__ = rb_entry(__parent__,typeof((*(__res__))),__nd__);  \
    __r__ = cmp(__prt__->__k__,__key__); \
    if (__r__<0)  __p__ = &__parent__->rb_left ;    \
    else if (__r__>0)  __p__ = &__parent__->rb_right ;   \
    else  break ;   \
  }   \
  /* not found */  \
  if (!(*__p__)) __rc__ = -1;   \
  else  __res__ = __prt__ ; \
  __rc__;   \
})
#endif

#define MY_RBTREE_PREORDER_FOR_EACH_ENTRY_SAFE(pos, n, root, field) \
	for (pos = rb_entry_safe(rb_first(root), typeof(*pos), field); \
	     pos && ({ n = rb_entry_safe(rb_next(&pos->field), \
			typeof(*pos), field); 1; }); \
	     pos = n)

#define MY_RBTREE_SORTORDER_FOR_EACH_ENTRY_SAFE(pos, n, root, field) \
	for (pos = rb_entry_safe(rb_last(root), typeof(*pos), field); \
	     pos && ({ n = rb_entry_safe(rb_prev(&pos->field), \
			typeof(*pos), field); 1; }); \
	     pos = n)


#endif /* __MYRBTREE_H__*/
