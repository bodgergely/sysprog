#include <sys/mman.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define PAGE_SIZE 4096


void* mmapPages(int count, int fd, int flags)
{
	return mmap(NULL, count * PAGE_SIZE, PROT_READ|PROT_WRITE, flags, fd, 0);
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

#define FLAGS_ALL MAP_SHARED|MAP_LOCKED|MAP_POPULATE|MAP_ANONYMOUS

int main(int argc, char** argv)
{
	init();
	const int pageCount = 4;
	char buff[pageCount];
	buff[1] = 1;
	int fd = open("memmapfile", O_RDWR);
	if(fd == -1)
	{
		printf("Failed to open file. errno: %d\n", errno);
	}
	printStats("Upon start.");
	char* mem = (char*)mmapPages(pageCount, fd, MAP_SHARED|MAP_POPULATE);
	if((void*)mem == -1)
	{
		printf("Failed to acquite mem. Errno: %d\n", errno);
		exit(-1);
	}
	printf("Mem at: %p\n", mem);

	printStats("After mmap");
	read(mem, pageCount, buff);
	printStats("After read");
	write(mem, pageCount, 'A');
	printStats("After write");
	msync(mem, pageCount*PAGE_SIZE, MS_SYNC);
	write(mem, pageCount, 'A');
	printStats("After write 2");

	return 0;
}
