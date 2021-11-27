/*
 * refcnt.h - A garbage collector for the average C programmer (header)
 * 
 *     Copyright (C) 2021, Roxanne MacKinnon
 * 
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *     <rdm3@williams.edu> Roxanne MacKinnon
 */
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
