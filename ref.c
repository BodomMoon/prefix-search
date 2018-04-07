#include "ref.h"

typedef struct memory_pool {
	char *current;
	char *tail;
}pool;

pool* init (size_t size){
	pool *ptr = (pool*)calloc( 0 , size);
	ptr->current = (char*) ptr;
	ptr->tail = ptr->current + size;
	printf("cur = %p tai = %p\n",ptr->current,ptr->tail );
	return ptr;
}

char* mpalloc(pool* ptr , size_t size) {
	//printf("get %p\n", ptr->current);
	if((ptr -> tail - ptr -> current) > size){
		char *temp = ptr->current;
		ptr->current += size;
		//printf("size = %ld temp is in %p current is in %p\n",size, temp, ptr->current);
		return temp;
	}
	else
		return NULL;
	
}

void pool_free(pool *p) 
{
    free(p);
}