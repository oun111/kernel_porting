
#ifndef __MYRBTREE_H__
#define __MYRBTREE_H__


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

#endif /* __MYRBTREE_H__*/
