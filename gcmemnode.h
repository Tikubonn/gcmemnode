
#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memnode.h"
#include "bitarray.h"

typedef struct gcmemnode {
  memnode memnode;
  bitarray *gcmarks;
} gcmemnode;

#define MAKE_LOCAL_GCMEMNODE(varname, size, nextnode)\
MAKE_LOCAL_BITARRAY(__ ## varname ## _reservemask, size);\
MAKE_LOCAL_BITARRAY(__ ## varname ## _gcmarks, size);\
uint8_t __ ## varname ## _sequence[(size)];\
gcmemnode __ ## varname = { { __ ## varname ## _reservemask, __ ## varname ## _sequence, (size), (memnode*)(nextnode) }, __ ## varname ## _gcmarks };\
gcmemnode *varname = &__ ## varname;

extern void gcmemnode_init (void*, size_t, bitarray*, bitarray*, gcmemnode*, gcmemnode*);
extern gcmemnode *make_gcmemnode (size_t, gcmemnode*);
extern void free_gcmemnode (gcmemnode*);
extern void free_gcmemnode_all (gcmemnode*);
extern memnode *gcmemnode_memnode (gcmemnode*);
extern gcmemnode *gcmemnode_next (gcmemnode*);
extern int gcmemnode_gc_start (gcmemnode*);
extern int gcmemnode_gc_mark (void*, size_t, gcmemnode*);
extern int gcmemnode_gc_unmark (void*, size_t, gcmemnode*);
extern int gcmemnode_gc_markp (void*, size_t, gcmemnode*, bool*);
extern int gcmemnode_gc_end (gcmemnode*);
