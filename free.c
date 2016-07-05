#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

int main(void) {
	void * p = malloc(8);
	printf("malloc p is %x.\n", p);
	free(p);
	printf("free p is %x.\n", p);
	p = alloca(5);
	printf("alloca p is %x.\n", p);	
	return 0;
}
