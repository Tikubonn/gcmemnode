
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memnode.h"
#include "bitarray.h"
#include "gcmemnode.h"

void gcmemnode_init (void *sequence, size_t size, bitarray *reservemask, bitarray *gcmarks, gcmemnode *nextnode, gcmemnode *gmnode){
  memnode_init(sequence, size, reservemask, (memnode*)nextnode, &(gmnode->memnode));
  gmnode->gcmarks = gcmarks;
}

gcmemnode *make_gcmemnode (size_t size, gcmemnode *nextnode){
  gcmemnode *gmnode = malloc(sizeof(gcmemnode));
  if (gmnode == NULL){
  }
  uint8_t *gmnodeseq = malloc(sizeof(uint8_t) * size);
  if (gmnodeseq == NULL){
  }
  bitarray *reservemask = make_bitarray(size);
  if (reservemask == NULL){
  }
  bitarray *gcmarks = make_bitarray(size);
  if (gcmarks == NULL){
  }
  gcmemnode_init(gmnodeseq, size, reservemask, gcmarks, nextnode, gmnode);
  return gmnode;
}

void free_gcmemnode (gcmemnode *gmnode){
  free_bitarray(gmnode->memnode.reservemask);
  free_bitarray(gmnode->gcmarks);
  free(gmnode->memnode.sequence);
  free(gmnode);
}

void free_gcmemnode_all (gcmemnode *gmnode){
  gcmemnode *gmnd = gmnode;
  while (gmnd != NULL){
    gcmemnode *nextnode = gcmemnode_next(gmnd);
    free_gcmemnode(gmnd);
    gmnd = nextnode;
  }
}

memnode *gcmemnode_memnode (gcmemnode *gmnode){
  return &(gmnode->memnode);
}

gcmemnode *gcmemnode_next (gcmemnode *gmnode){
  return (gcmemnode*)gcmemnode_memnode(gmnode)->nextnode;
}

int gcmemnode_gc_start (gcmemnode *gmnode){
  for (gcmemnode *nd = gmnode; nd != NULL; nd = gcmemnode_next(nd)){
    if (bitarray_fill(0, 0, bitarray_length(nd->gcmarks), nd->gcmarks) != 0){
      return 1;
    }
  }
  return 0;
}

int gcmemnode_gc_mark (void *address, size_t size, gcmemnode *gmnode){
  memnode *foundmemnode = memnode_find(address, gcmemnode_memnode(gmnode));
  if (foundmemnode == NULL){
    return 1;
  }
  size_t offset = address - foundmemnode->sequence;
  gcmemnode *foundgmnode = (gcmemnode*)foundmemnode;
  if (bitarray_fill(1, offset, offset + size, foundgmnode->gcmarks) != 0){
    return 1;
  }
  return 0;
}

int gcmemnode_gc_unmark (void *address, size_t size, gcmemnode *gmnode){
  memnode *foundmemnode = memnode_find(address, gcmemnode_memnode(gmnode));
  if (foundmemnode == NULL){
    return 1;
  }
  size_t offset = address - foundmemnode->sequence;
  gcmemnode *foundgmnode = (gcmemnode*)foundmemnode;
  if (bitarray_fill(0, offset, offset + size, foundgmnode->gcmarks) != 0){
    return 1;
  }
  return 0;
}

int gcmemnode_gc_markp (void *address, size_t size, gcmemnode *gmnode, bool *markp){
  memnode *foundmemnode = memnode_find(address, gcmemnode_memnode(gmnode));
  if (foundmemnode == NULL){
    return 1;
  }
  size_t offset = address - foundmemnode->sequence;
  gcmemnode *foundgmnode = (gcmemnode*)foundmemnode;
  if (bitarray_fillp(1, offset, offset + size, foundgmnode->gcmarks, markp) != 0){
    return 1;
  }
  return 0;
}

int gcmemnode_gc_end (gcmemnode *gmnode){
  for (gcmemnode *nd = gmnode; nd != NULL; nd = gcmemnode_next(nd)){
    if (bitarray_copy(nd->gcmarks, gcmemnode_memnode(nd)->reservemask) != 0){
      return 1;
    }
  }
  return 0;
}
