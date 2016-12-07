#include "remap.h"

int foo(int rep)
{
	for(int i=0;i<rep;i++)
	{
		printf("foo: %d\n", i);
	}
}

int foo2()
{
	for(int i=0;i<3;i++)
	{
		printf("foo2: %d\n", i);
	}
}



int main()
{
	printf("main is at: %p foo: %p foo2: %p\n", main, foo, foo2);
	foo2();
	foo(10);
}
