#include "my-oj.h"

extern void trace(int child_pid);

void trace_fun() {
	/* try some calls here */
	printf("Kill me if you can!  凸(-_-)凸\n");
}

int main() {
	int child_pid;

	// create a new process using fork()
	if( (child_pid = fork()) != 0) {
		// father process start from here

		trace(child_pid);
	}
	else {
		// child process start from here

		// notify father process to trace "me"
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		raise(SIGSTOP);

		// test function, system calls in this function will be caught by father process
		trace_fun();
	}

	return 0;
}
