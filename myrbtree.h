
#ifndef __MYRBTREE_H__
#define __MYRBTREE_H__


#define MY_RB_TREE_INSERT(root, item, k, node) ({  \
  struct rb_node **p = &(root)->rb_node, *parent = 0 ;  \
  typeof(item) prt ;  \
  while (*p) {  \
    parent  = *p ;  \
    prt = rb_entry(parent,typeof((*(item))),node);  \
    if (prt->k < (item)->k) {  \
      p = &parent->rb_left ;  \
    } else {   \
      p = &parent->rb_right ;  \
    }  \
  }   \
  /* insert new node */   \
  (item)->node.__rb_parent_color = (unsigned long)parent; /* FIXME: why ?? */  \
  (item)->node.rb_left = (item)->node.rb_right = NULL ;   \
  *p = &(item)->node ;   \
  /* change color */    \
  rb_insert_color(&(item)->node,root);  \
  0;  \
})


#define MY_RB_TREE_FIND(root, key, res, k, node) ({  \
  struct rb_node **p = &(root)->rb_node, *parent = 0 ;  \
  typeof(res) prt ;   \
  int rc = 0;   \
  while (*p) {   \
    parent  = *p ;   \
    prt = rb_entry(parent,typeof((*(res))),node);  \
    if (prt->k < key)  p = &parent->rb_left ;    \
    else if (prt->k > key)  p = &parent->rb_right ;   \
    else  break ;   \
  }   \
  /* not found */  \
  if (!(*p)) rc = -1;   \
  else  res = prt ; \
  rc;   \
})

#endif /* __MYRBTREE_H__*/
