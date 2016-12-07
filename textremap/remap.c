#include "remap.h"
#include <sys/mman.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define HUGE_PAGE_SIZE (1 << 21)

static void MemCpy(void* dest_, const void* src_, size_t n_)
{
	size_t i = 0;
	while(i < n_)
	{
		((char*)dest_)[i] = ((const char*)src_)[i];
		i++;
	}
}

void remap_text_section(const uint8_t* start_text, const uint8_t* end_text)
{
	int len = end_text - start_text;
	printf("remap_text_section addr: %p\n", remap_text_section);
	printf("start_text: %p end_text: %p size: %d which is %d pages.\n", start_text, end_text, len, len/HUGE_PAGE_SIZE);
	printf("Data at start: %x\n", *((uint*)start_text));

	void* placeholder = mmap(NULL, len, PROT_READ|PROT_EXEC, MAP_ANONYMOUS|MAP_PRIVATE|MAP_NORESERVE, -1, 0);

	if(placeholder == MAP_FAILED)
	{
		printf("placeholder map failed!\n");
		exit(-1);
	}

	void* temp_text_address = mremap( (uint8_t*)start_text, len, len, MREMAP_MAYMOVE | MREMAP_FIXED, placeholder );
	printf("remapped text to: %p\n", temp_text_address);

	if(temp_text_address == MAP_FAILED)
	{
		printf("remap failed!, errno: %d\n", errno);
		exit(-1);
	}

	void* original_text_address = mmap((uint8_t*)start_text, len, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_NORESERVE|MAP_POPULATE,-1,0);
	if(original_text_address == MAP_FAILED)
	{
		printf("original_text_address map failed!, errno: %d\n", errno);
		exit(-1);
	}

	printf("Data at original_text before copy: %x\n", *((uint*)original_text_address));
	MemCpy(original_text_address, temp_text_address, len);
	printf("Data at original_text after copy: %x\n", *((uint*)original_text_address));

	printf("Will set protection at: %p with size: %d\n", original_text_address, len);

	if(mprotect(original_text_address, len, PROT_READ|PROT_EXEC)!=0)
	{
		printf("mprotect failed!, errno: %d\n", errno);
		exit(-1);
	}


	if(munmap(temp_text_address, len)!=0)
	{
		printf("unmap failed!, errno: %d\n", errno);
		exit(-1);
	}

	printf("Remapping succeeded!\n");

}
