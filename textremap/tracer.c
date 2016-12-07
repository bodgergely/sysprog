#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/user.h>
#include <errno.h>

extern char** environ;

#define PAGESIZE 4096
#define WAIT_MILLISEC(n) (n * 1000)

int main(int argc, char** argv)
{
	pid_t pid;
	pid_t myPid;
	int status;

	int data = 0xDEADBEEF;
	struct user_regs_struct regs;

	if((pid = fork()) == -1)
	{
		printf("Could not fork!\n");
		exit(-1);
	}

	myPid = getpid();
	if(!pid)
	{
		if(ptrace(PTRACE_TRACEME, myPid, 0L, 0L) == -1)
		{
			printf("Ptraceme on child failed!\n");
			exit(-1);
		}
		else
		{
			printf("Child process...with pid %d before loading program: %s\n", myPid, argv[1]);
			execve(argv[1], argv+1, environ);
		}
	}
	else
	{
		while(1)
		{
			waitpid(pid, &status, 0);
			if(WIFSTOPPED(status))
			{
				printf("Child stopped!\n");
				if(WIFSTOPPED(status))
				{
					printf("Poking data at %p\n", (void*)0x6000f6);
					if(ptrace(PTRACE_POKETEXT, pid, 0x6000f6, &data) == -1)
					{
						printf("Poking failed!, errno: %d\n", errno);
					}
					printf("Got here\n");
				}
				if(ptrace(PTRACE_CONT, pid, 0, 0) == -1)
				{
					printf("Continuing failed!, errno: %d\n", errno);
				}
				usleep(WAIT_MILLISEC(5000));
				kill(pid, SIGINT);
			}
		}
	}

	return 0;
}
