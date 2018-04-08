#include "ref.h"

typedef struct memory_pool {
	char *current;
	char *tail;
}pool;

pool *init (size_t size){
	/*pool *ptr = (pool*)calloc( 1 , sizeof(pool));
	ptr->current =(char*) malloc(size);
	ptr->tail =(char*) ptr + size;*/

	pool *ptr = (pool*)calloc( 1 , size);
	ptr->current = (char*)ptr + sizeof(pool);
	ptr->tail =(char*) ptr + size;
	return ptr;
}

char *mpalloc(pool **ptr , size_t size) {
	if(((*ptr) -> tail - (*ptr) -> current) > size){
		char *temp = (*ptr)->current;
		(*ptr)->current += size;
		return temp;
	}
	else
		return NULL;
	
}
pool *mpfreeback(pool **ptr , size_t size) {

	(*ptr)->current -= size;
	memset((*ptr)->current,0,size);
	return (*ptr);
}

void pool_free(pool *p) {
    free(p);
}

int getLarge( pool *ptr){
	return (int)(ptr->tail - ptr ->current);
}