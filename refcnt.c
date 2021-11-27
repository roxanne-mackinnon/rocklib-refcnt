#include "refcnt.h"
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define UNSCALED_POINTER_ADD(p,x) ((void *) ((char *)(p) + (x)))
#define UNSCALED_POINTER_SUB(p,x) ((void *) ((char *)(p) - (x)))

#define MAX_REFS_ALLOWED UINT32_MAX

/**
 * Represents the information header for a block of memory.
 */
struct RefHeader {
  unsigned ref_count;
};


/**
 * Represents an entire reference-counted block of memory, including header.
 */
struct RefBlock {
  struct RefHeader head;
  void * data;
};

/**
 * Returns a reference-counted block of memory, with 0 references.
 * size: the requested size of the block, in bytes.
 * ENOMEM if malloc fails.
 * ---
 * note: refcnt does not currently track references, just counts.
 */
void *
refcnt_new
(size_t size)
{
  // if the client requests 0 bytes, we should simply return NULL,
  // without causing error.
  if (size == 0) {
    errno = 0;
    return NULL;
  }

  if (size + sizeof(struct RefHeader) < size) {
    errno = ENOMEM;
    return NULL;
  }
  
  // allocate space for memory and refcount information
  struct RefBlock * result = malloc(size + sizeof (struct RefHeader));

  // return NULL if malloc fails.
  // duplicate whatever errno malloc returned (by not changing it).
  if (result == NULL) {
    return NULL;
  }

  // set out initial reference count to zero (not one!).
  result->head.ref_count = 0;
  
  // be sure to return the DATA, not the header.
  return UNSCALED_POINTER_ADD(result, sizeof(struct RefHeader));
}


int
refcnt_ref
(void * ptr)
{
  if (ptr == NULL) {
    errno = EINVAL;
    return -1;
  }
  
  // retreive info header from the given block of memory.
  struct RefBlock * block = (struct RefBlock *) UNSCALED_POINTER_SUB(ptr, sizeof(struct RefHeader));

  // cause an error if the object has too many references. 
  if (block->head.ref_count == MAX_REFS_ALLOWED) { 
    errno = ETOOMANYREFS;
    return -1; 
  } 

  // increment refcount and return with a success.
  block->head.ref_count++;
  return 0;
}

int
refcnt_unref
(void * ptr)
{
  if (ptr == NULL) {
    errno = EINVAL;
    return -1;
  }
  
  // retreive info header from given block of memory.
  struct RefBlock * block = (struct RefBlock *) UNSCALED_POINTER_SUB(ptr, sizeof(struct RefHeader));

  // this is a race condition!
  // if refcount is zero, or one, destroy the info header and free the memory.
  if (block->head.ref_count == 0) {
    memset(block, 0, sizeof(struct RefHeader));
    free(block);
    return 0;
  }

  block->head.ref_count--;
  return 0;
}
