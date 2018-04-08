#include "ref.h"

typedef struct memory_pool {
	char *current;
	char *tail;
}pool;

pool *init (size_t size){
	pool *ptr = (pool*)calloc( 1 , sizeof(pool));
	ptr->current =(char*) malloc(size);
	ptr->tail = ptr->current + size;

	/*pool *ptr = (pool*)calloc( 1 , size);
	printf("current place %p value %p\n",&ptr->current , ptr->current );
	printf("tail place %p value %p\n",  &ptr->tail , ptr ->tail);
	ptr->current = (char*)ptr + sizeof(pool);
	ptr->tail = (char*)ptr + size;
	getchar();*/
	/*printf("cur = %p tai = %p\n",ptr->current,ptr->tail );
	printf("cur place = %p value = %p\n",&(*ptr).current,ptr->current );
	printf("tail place = %p value = %p\n",&(*ptr).tail,ptr->tail );*/
	return ptr;
}

char *mpalloc(pool **ptr , size_t size) {
	//printf("get %p\n", ptr->current);
	if(((*ptr) -> tail - (*ptr) -> current) > size){
		char *temp = (*ptr)->current;
		//printf("before size = %ld temp is in %p current is in %p tail is in%p\n",size, temp, (*ptr)->current,(*ptr)->tail);
		(*ptr)->current += size;
		//printf("after size = %ld temp is in %p current is in %p tail is in%p\n",size, temp, (*ptr)->current,(*ptr)->tail);
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