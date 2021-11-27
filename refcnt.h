#ifndef __REFCNT_H__
#define __REFCNT_H__

#include <stddef.h>
#include <stdlib.h>

/**
 * Returns a reference-counted block of memory, with 0 references.
 * size: the requested size of the block, in bytes.
 * ENOMEM if malloc fails
 * note: refcnt does not currently track references, just counts.
 */
void * refcnt_new(size_t size);


/**
 * Adds a reference to a block of memory.
 * Returns 0 if successful, -1 otherwise. 
 * ETOOMANYREFS if ptr has too many references.
 * EINVAL if ptr is null
 * ptr: a block of memory allocated by refcnt_new.
 */
int refcnt_ref(void * ptr);

/**
 * Removes a reference from a block of memory.
 * Frees memory when the reference count reaches 0
 * ptr: a block of memory allocated by refcnt_new
 * EINVAL if ptr is NULL
 * Returns -1 on error, 0 otherwise
 */
int refcnt_unref(void * ptr);


#endif
