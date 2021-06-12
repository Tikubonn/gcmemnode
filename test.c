
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memnode.h"
#include "gcmemnode.h"

#define TEST(form)\
if (form){\
printf("success: %s at %s:%d\n", #form, __FILE__, __LINE__);\
}\
else {\
printf("error: %s at %s:%d\n", #form, __FILE__, __LINE__); abort();\
}

static void test_gcmemnode_gc (){
  // allocate many times then gc.
  {
    MAKE_LOCAL_GCMEMNODE(gmnode, 1024, NULL);
    char *data1 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    char *data2 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    char *data3 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    TEST(data1 != NULL);
    TEST(data2 != NULL);
    TEST(data3 != NULL);
    TEST(gcmemnode_gc_start(gmnode) == 0);
    TEST(gcmemnode_gc_mark(data1, 3, gmnode) == 0);
    TEST(gcmemnode_gc_mark(data2, 3, gmnode) == 0);
    TEST(gcmemnode_gc_mark(data3, 3, gmnode) == 0);
    TEST(gcmemnode_gc_unmark(data1, 3, gmnode) == 0);
    bool markp;
    TEST(gcmemnode_gc_markp(data1, 3, gmnode, &markp) == 0);
    TEST(markp == false);
    TEST(gcmemnode_gc_markp(data2, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_markp(data3, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_end(gmnode) == 0);
    char *data4 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    TEST(data4 != NULL);
    TEST(data4 == data1);
  }
  // allocate many times from linked nodes then gc.
  {
    MAKE_LOCAL_GCMEMNODE(gmnode, 1024, NULL);
    MAKE_LOCAL_GCMEMNODE(rootgmnode, 0, gmnode);
    char *data1 = memnode_allocate(3, gcmemnode_memnode(rootgmnode));
    char *data2 = memnode_allocate(3, gcmemnode_memnode(rootgmnode));
    char *data3 = memnode_allocate(3, gcmemnode_memnode(rootgmnode));
    TEST(data1 != NULL);
    TEST(data2 != NULL);
    TEST(data3 != NULL);
    TEST(gcmemnode_gc_start(rootgmnode) == 0);
    TEST(gcmemnode_gc_mark(data1, 3, rootgmnode) == 0);
    TEST(gcmemnode_gc_mark(data2, 3, rootgmnode) == 0);
    TEST(gcmemnode_gc_mark(data3, 3, rootgmnode) == 0);
    TEST(gcmemnode_gc_unmark(data1, 3, rootgmnode) == 0);
    bool markp;
    TEST(gcmemnode_gc_markp(data1, 3, gmnode, &markp) == 0);
    TEST(markp == false);
    TEST(gcmemnode_gc_markp(data2, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_markp(data3, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_end(rootgmnode) == 0);
    char *data4 = memnode_allocate(3, gcmemnode_memnode(rootgmnode));
    TEST(data4 != NULL);
    TEST(data4 == data1);
  }
}

static void test_make_gcmemnode (){
  {
    gcmemnode *gmnode = make_gcmemnode(1024, NULL);
    TEST(gmnode != NULL);
    char *data1 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    char *data2 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    char *data3 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    TEST(data1 != NULL);
    TEST(data2 != NULL);
    TEST(data3 != NULL);
    TEST(gcmemnode_gc_start(gmnode) == 0);
    TEST(gcmemnode_gc_mark(data1, 3, gmnode) == 0);
    TEST(gcmemnode_gc_mark(data2, 3, gmnode) == 0);
    TEST(gcmemnode_gc_mark(data3, 3, gmnode) == 0);
    TEST(gcmemnode_gc_unmark(data1, 3, gmnode) == 0);
    bool markp;
    TEST(gcmemnode_gc_markp(data1, 3, gmnode, &markp) == 0);
    TEST(markp == false);
    TEST(gcmemnode_gc_markp(data2, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_markp(data3, 3, gmnode, &markp) == 0);
    TEST(markp == true);
    TEST(gcmemnode_gc_end(gmnode) == 0);
    char *data4 = memnode_allocate(3, gcmemnode_memnode(gmnode));
    TEST(data4 != NULL);
    TEST(data4 == data1);
    free_gcmemnode_all(gmnode);
  }
}

int main (){
  test_gcmemnode_gc();
  test_make_gcmemnode();
  return 0;
}
