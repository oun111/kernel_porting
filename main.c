#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "rbtree.h"
#include "radix-tree.h"
#include "bitmap.h"
#include "llist.h"
#include "hashtable.h"


typedef struct test_list_t {
  int val;
  struct list_head node ;
} tl ;

typedef struct test_rbtree_t {
  int val;
  struct rb_node node;
} rt ;

typedef struct test_radixtree_t {
  unsigned long val ;
} xt ;

typedef struct test_llist_t {
  int val;
  struct llist_node node ;
} tll ;

typedef struct test_hashtable_t {
  int val;
  struct hlist_node node ;
} ht ;

int g_caseCnt = 1;


int test_list()
{
  LIST_HEAD(head) ;
  tl px[] = {
    {.val=10,},
    {.val=11,},
    {.val=12,},
    {.val=13,},
    {.val=14,},
  } ;
  tl *p;

  printf("\n%d: test link list tree\n\n",g_caseCnt++);

  /** 
   * add 
   */
  for (int i=0;i<ARRAY_SIZE(px);i++) {
    list_add(&px[i].node,&head);
  }

  printf("before: \n");

  /**
   * iterate 
   */
  list_for_each_entry(p,&head,node) {
    printf("p->val: %d\n",p->val);
  }

  /**
   * delete 
   */
  list_del(&px[2].node);

  printf("after: \n");

  /**
   * iterate again 
   */
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
  rt rnx[] = {
    {.val=1,},
    {.val=12,},
    {.val=3,},
    {.val=300,},
    {.val=13,},
    {.val=0,},
  } ;
  int ret = 0;

  printf("\n%d: test rb tree\n\n",g_caseCnt++);

  /**
   * add 
   */

  /* insert tree node */
  for (int i=0;i<ARRAY_SIZE(rnx);i++) {
    my_rb_tree_insert(&root,&rnx[i]);
  }

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
  ret = my_rb_tree_find(&root,300,&pos);
  if (ret) {
    printf("%s: not found\n",__func__);
  } else {
    printf("%s: found %p -> %d\n",__func__,pos,pos->val);
  }

  /**
   * erase
   */
  rb_erase(&rnx[3].node,&root);

  /**
   * iterate
   */
  rbtree_postorder_for_each_entry_safe(pos,n,&root,node) {
    printf("node %p, val %d\n",pos,pos->val);
  }

  return 0;
}

int test_radixtree()
{
  RADIX_TREE(root, GFP_KERNEL);
  xt ndx[] = {
     {.val = 12,},
     {.val = 1,},
     {.val = 100,},
     {.val = 50,},
  };

  printf("\n%d: test radix tree\n\n",g_caseCnt++);

  radix_tree_init();

  /**
   * insert
   */
  for (int i=0;i<ARRAY_SIZE(ndx);i++) {
    radix_tree_insert(&root,ndx[i].val,&ndx[i]);
    printf("inserting %ld %p\n",ndx[i].val,&ndx[i]);
  }

  /**
   * lookup
   */
  unsigned long search = ndx[2].val;
  xt *pxt = radix_tree_lookup(&root,search);
  if (!pxt) {
    printf("%s: not found\n",__func__);
  } else {
    printf("%s: found: %p -> %ld\n",__func__,pxt,pxt->val);
  }

  /**
   * iteration
   */
  void **slot ;
  struct radix_tree_iter itr ;
  radix_tree_for_each_slot(slot,&root,&itr,0) {
    printf("idx: %ld, %p\n",itr.index,*slot);
  }

  return 0;
}

int test_bitmap()
{
  DECLARE_BITMAP(bmap1, 32);

  printf("\n%d: test bitmap\n\n",g_caseCnt++);

  bitmap_zero(bmap1, 32);

  printf("zero bits: 0x%lx\n",bmap1[0]);

  __set_bit(8,bmap1);

  printf("set 1 bit: 0x%lx\n",bmap1[0]);

  bitmap_set(bmap1, 4, 2);

  printf("set bits: 0x%lx\n",bmap1[0]);

  __clear_bit(12,bmap1);

  printf("clear 1 bit: 0x%lx\n",bmap1[0]);

  printf("test 1 bit: %d\n",test_bit(13,bmap1));

  bitmap_clear(bmap1, 10, 1);

  printf("clear bits: 0x%lx\n",bmap1[0]);

  bitmap_fill(bmap1, 3);

  printf("fill bits: 0x%lx\n",bmap1[0]);

  return 0;
}

int test_llist()
{
  LLIST_HEAD(head) ;
  tll px[] = {
    {.val=10,},
    {.val=11,},
    {.val=12,},
    {.val=13,},
    {.val=14,},
  } ;
  tll *p;
#define is_head(e,h) ({\
  struct llist_node *llp = &(e)->node ;\
  struct llist_head *llh = container_of(llp,struct llist_head,first);\
  (bool)(llh==(h));\
})

  printf("\n%d: test lockless link list\n\n",g_caseCnt++);

  /** 
   * add 
   */
  for (int i=0;i<ARRAY_SIZE(px);i++) {
    llist_add(&px[i].node,&head);
  }

  printf("before: \n");

  /**
   * iterate 
   */
  llist_for_each_entry(p,&head,node) {

    if (is_head(p,&head)) 
      continue ;

    printf("p->val: %d\n",p->val);
  }

  /**
   * can only delete 1st entry
   */
  llist_del_first(&head);
  llist_del_first(&head);

  printf("after: \n");

  /**
   * iterate again 
   */
  llist_for_each_entry(p,&head,node) {

    if (is_head(p,&head)) 
      continue ;

    printf("p->val: %d\n",p->val);
  }

  /**
   * delete all
   */

  printf("clear list: \n");

  llist_del_all(&head);

  /**
   * iterate again 
   */
  llist_for_each_entry(p,&head,node) {

    if (is_head(p,&head)) 
      continue ;

    printf("p->val: %d\n",p->val);
  }


  return 0;
}

int test_hashtable()
{
#define hash_get(tbl,key)   ((tbl)[hash_min((key), HASH_BITS((tbl)))])

  DEFINE_HASHTABLE(htable,16);
  ht tx[] = {
    {.val=10,},
    {.val=11,},
    {.val=12,},
    {.val=13,},
    {.val=14,},
  } ;
  int i;
  ht *pt ;
  struct hlist_node *n ;

  printf("\n%d: test hash table\n\n",g_caseCnt++);

  /**
   * add
   */
  for (int i=0;i<ARRAY_SIZE(tx);i++) {
    hash_add(htable,&tx[i].node,tx[i].val);
    printf("adding %p -> key %d\n",tx+i,tx[i].val);
  }

  /**
   * iterate
   */
  printf("hash contents: \n");

  hash_for_each_safe(htable,i,n,pt,node) {
    printf("h->val: %d\n",pt->val);
  }

  /**
   * fetch
   */
  int key = 11 ;
  struct hlist_head *pv = &hash_get(htable,key) ;

  if (!pv || hlist_empty(pv)) {
    printf("found nothing by key %d\n",key);
  }

  hlist_for_each_entry_safe(pt,n,pv,node) {
    printf("fetching ptr %p -> key %d\n",pt,key);
  }

  return 0;
}

int main()
{
  test_list();

  test_rbtree();

  test_radixtree();

  test_bitmap();

  test_llist();

  test_hashtable();

  return 0;
}

