#ifndef REF_H
#define REF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memory_pool pool;

pool * init (size_t size);

char * mpalloc (pool *p, size_t size);

void pool_free(pool *p) ;

#endif