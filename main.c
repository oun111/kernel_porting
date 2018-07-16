#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "list.h"
#include "rbtree.h"
#include "radix-tree.h"


typedef struct test_list_t {
  int val;
  struct list_head node ;
} tl ;

typedef struct test_rbtree_t {
  int val;
  struct rb_node node;
} rt ;


int test_list()
{
  LIST_HEAD(head) ;
  tl p1 = {.val=10,},
     p2 = {.val=11,},
     p3 = {.val=12,},
     p4 = {.val=13,},
     p5 = {.val=14,};
  tl *p;

  /* add */
  list_add(&p1.node,&head);
  list_add(&p2.node,&head);
  list_add(&p3.node,&head);
  list_add(&p4.node,&head);
  list_add(&p5.node,&head);

  printf("before: \n");

  /* iterate */
  list_for_each_entry(p,&head,node) {
    printf("p->val: %d\n",p->val);
  }

  /* delete */
  list_del(&p2.node);

  printf("after: \n");

  /* iterate again */
  list_for_each_entry(p,&head,node) {
    printf("p->val: %d\n",p->val);
  }

  return 0;
}

int my_rb_tree_insert(struct rb_root *root, rt *item)
{
  struct rb_node **p = &root->rb_node, *parent = 0 ;
  rt *prt ;

  while (*p) {
    parent  = *p ;
    prt = rb_entry(parent,rt,node);
    if (prt->val < item->val) {
      p = &parent->rb_left ;
    } else {
      p = &parent->rb_right ;
    }
  }

  /* insert new node */
  item->node.__rb_parent_color = (unsigned long)parent; // FIXME: why ??
  item->node.rb_left = item->node.rb_right = NULL ;
  *p = &item->node ;

  /* change color */
  rb_insert_color(&item->node,root);

  return 0;
}

int my_rb_tree_find(struct rb_root *root, int val, rt **item)
{
  struct rb_node **p = &root->rb_node, *parent = 0 ;
  rt *prt ;

  while (*p) {
    parent  = *p ;
    prt = rb_entry(parent,rt,node);
    if (prt->val < val) {
      p = &parent->rb_left ;
    } else if (prt->val > val) {
      p = &parent->rb_right ;
    } else {
      break ;
    }
  }

  /* not found */
  if (!*p) {
    return -1;
  }

  if (item) {
    *item = prt ;
  }

  return 0;
}

int test_rbtree()
{
  struct rb_root root = RB_ROOT;
  rt rn1 = {.val=1,},
     rn2 = {.val=12,},
     rn3 = {.val=3,},
     rn4 = {.val=300,},
     rn5 = {.val=13,},
     rn6 = {.val=0,};
  int ret = 0;

  /**
   * add 
   */

  /* insert tree node */
  my_rb_tree_insert(&root,&rn1);

  my_rb_tree_insert(&root,&rn2);

  my_rb_tree_insert(&root,&rn3);

  my_rb_tree_insert(&root,&rn4);

  my_rb_tree_insert(&root,&rn5);

  my_rb_tree_insert(&root,&rn6);

  /**
   * iterate
   */
  rt *pos, *n ;

  rbtree_postorder_for_each_entry_safe(pos,n,&root,node) {
    printf("node %p, val %d\n",pos,pos->val);
  }

  /**
   * search
   */
  ret = my_rb_tree_find(&root,10,&pos);
  if (ret) {
    printf("not found\n");
  } else {
    printf("found %p, val %d\n",pos,pos->val);
  }

  /**
   * erase
   */
  rb_erase(&rn3.node,&root);

  /**
   * iterate
   */
  rbtree_postorder_for_each_entry_safe(pos,n,&root,node) {
    printf("node %p, val %d\n",pos,pos->val);
  }

  return 0;
}

int main()
{
  test_list();

  test_rbtree();

  return 0;
}

