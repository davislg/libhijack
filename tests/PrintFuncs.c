#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hijack.h>
#include <hijack_func.h>

void usage(const char *name)
{
	fprintf(stderr, "USAGE: %s <pid>\n", name);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	HIJACK *hijack;
	FUNC *func;
	unsigned long addr;
	
	if (argc != 2)
		usage(argv[0]);
	
	hijack = InitHijack();
	AssignPid(hijack, atoi(argv[1]));
	
	if (Attach(hijack) != ERROR_NONE)
	{
		fprintf(stderr, "[-] Couldn't attach!\n");
		exit(EXIT_FAILURE);
	}
	
	if (LocateAllFunctions(hijack) != ERROR_NONE)
	{
		fprintf(stderr, "[-] Couldn't locate all functions!\n");
		exit(EXIT_FAILURE);
	}

	for (func = hijack->funcs; func != NULL; func = func->next)
	{
		if (!(func->name))
			continue;
		
		addr = FindFunctionInGot(hijack, func->vaddr);
		
		printf("[+] %s\t%s @ 0x%08lx (%u)", func->libname, func->name, func->vaddr, func->sz);
		if (addr > 0)
			printf(" -> 0x%08lx", addr);
		
		printf("\n");
	}
	
	Detach(hijack);
	
	return 0;
}
