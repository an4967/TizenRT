#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tinyara/kmalloc.h>

int mem_test_main(int argc, char *argv[])
{
	// int ret;

	printf("MEM TEST\n");

	int random = 0;
	int i;
	void *ptr;

	srand(time(NULL));

	for (i = 0; i < 10000; i++) {
		random = (rand() % 1024) + 1;
		ptr = malloc(random);
		free(ptr);
	}

	printf("MEM DONE\n");

	return 0;
}
