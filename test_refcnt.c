/*
 * test_refcnt.c - Unit tests for refcnt.c
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
#include "refcnt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include "refcnt.h"

void test_refcnt_new(void) {
  assert(refcnt_new(0) == NULL);

  void * v = refcnt_new((size_t) -1);
  assert(errno == ENOMEM);

  v = refcnt_new(1);
  assert(*((unsigned *) ((void *) ((char *)(v) - (4)))) == 0);
  refcnt_unref(v);
}

void test_refcnt_ref() {
  void * v = refcnt_new(0);
  int ret = refcnt_ref(v);
  assert(errno == EINVAL);
  assert(ret == -1);

  refcnt_unref(v);
  
  v = refcnt_new(UINT16_MAX);
  ((unsigned *)v)[-1] = UINT32_MAX;
  ret = refcnt_ref(v);
  assert(errno == ETOOMANYREFS);
  assert(ret == -1);
  ((unsigned *)v)[-1] = 0;
  refcnt_unref(v);
}

void test_refcnt_unref() {
  assert(refcnt_unref(NULL) == -1 && errno == EINVAL);
  
  void * v = refcnt_new(256);
  for (unsigned i = 0; i < 500; i++) {
    assert(refcnt_ref(v) == 0);
  }
  
  for (unsigned j = 0; j < 500; j++) {
    assert(refcnt_unref(v) == 0);
  }

  assert(refcnt_unref(v) == 0);
  return;
}


int main()
{

  test_refcnt_new();

  test_refcnt_ref();

  test_refcnt_unref();
  return 0;
}
