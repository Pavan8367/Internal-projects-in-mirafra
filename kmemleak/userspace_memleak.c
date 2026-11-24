//unfreed allocated memory

#if 0
#include<stdio.h>
#include<stdlib.h>

int main()
{
	//Allocating 100 bytes
	int *ptr =(int*) malloc(100 * sizeof(int));
	if(!ptr)
	{
		printf("Memory is not alllocated\n");
		perror("malloc");
	}
	//forgot to free the memory
	//free(ptr);
	return 0;
}
#endif

#if 1
#include<stdio.h>
#include<stdlib.h>

int main()
{
	//Allocating 100 bytes
	int *ptr =(int*) malloc(100 * sizeof(int));
	if(!ptr)
	{
		printf("Memory is not alllocated\n");
		perror("malloc");
	}
	free(ptr);
	ptr = NULL;
	return 0;
}
#endif


//Example 2: Invalid Memory Access
#if 0
#include <stdlib.h>

int main()
{
    int *ptr = malloc(sizeof(int));

    free(ptr);

    *ptr = 10;

    return 0;
}
#endif


//Example 3: Uninitialized Variable
#if 0
#include <stdio.h>

int main()
{
    int x;

    printf("%d\n", x);

    return 0;
}
#endif
