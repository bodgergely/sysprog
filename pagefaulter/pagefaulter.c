#include <sys/mman.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PAGE_SIZE 4096

#define FLAGS_PRIVATE MAP_PRIVATE|MAP_ANONYMOUS
#define FLAGS_SHARED  MAP_SHARED|MAP_ANONYMOUS
#define FLAGS_ALL     MAP_SHARED|MAP_LOCKED|MAP_POPULATE|MAP_ANONYMOUS

void* mmapPages(int count)
{
	return mmap(NULL, count * PAGE_SIZE, PROT_READ|PROT_WRITE, FLAGS_SHARED, -1, 0);
}

void read(char* mem, int pageCount, char* out)
{
	for(int i=0;i<pageCount;i++)
	{
		out[i] = *mem;
		mem += PAGE_SIZE;
	}
}


void write(char* mem, int pageCount,  int val)
{
	for(int i=0;i<pageCount;i++)
	{
		*mem = val;
		mem += PAGE_SIZE;
	}
}

struct rusage prevUsage;

void printStats(const char* message)
{
	struct rusage usage;
	int a;
	if((a = getrusage(RUSAGE_SELF, &usage))!=0)
	{
		printf("ERROR in getrusage!: %d\n", a);
	}

	printf("%s : minor: %ld, major: %ld\n", message, usage.ru_minflt - prevUsage.ru_minflt, usage.ru_majflt-usage.ru_majflt);

	prevUsage = usage;
}

void init()
{
	memset(&prevUsage, 0, sizeof(prevUsage));
}

int main(int argc, char** argv)
{
	init();
	const int pageCount = 4;
	printStats();
	char* mem = (char*)mmapPages(pageCount);
	if((void*)mem == -1)
	{
		printf("Failed to acquite mem. Errno: %d\n", errno);
		exit(-1);
	}
	printf("Mem at: %p\n", mem);
	char buff[pageCount];
	printStats();
	read(mem, pageCount, buff);
	printStats();
	write(mem, pageCount, 'A');
	printStats();

	return 0;
}
