#include "my-oj.h"

void trace(int child_pid) {
	int pid;
	int status;
	int enter_syscall = 1;
	int syscall_id;

	// wait the child process for tracing
	pid = waitpid(child_pid, &status, 0);

	// make system calls from child process easy to distinguish (SIGTRAP | 0x80)
	ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_TRACESYSGOOD);
	// trace system calls from child process
	ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
	while(1) {
		// wait for a system call from child process
		pid = waitpid(child_pid, &status, 0);
		if(WIFSTOPPED(status) && pid > 0) {
			int sig = WSTOPSIG(status);
			if(sig == (SIGTRAP|0x80)) {
				if (enter_syscall == 1) {	// issue a system call
					// get the system call ID
					syscall_id = ptrace(PTRACE_PEEKUSER, child_pid, ORIG_EAX * 4, 0);
					fprintf(stderr,"Child process issued a system call, syscall_ID = %d. ", syscall_id);
					switch(syscall_id) {
						case SYS_write:
							fprintf(stderr,"It's a 'write' system call.");
							void *str = (void *)ptrace(PTRACE_PEEKUSER, child_pid, ECX * 4, 0);
							int len = ptrace(PTRACE_PEEKUSER, child_pid, EDX * 4, 0);
							fprintf(stderr," The content is:\n");

							// set red color to hightlight the output
							fprintf(stderr,"\033[1;31m");
							fwrite(str, len, 1, stderr);
							// reset color
							fprintf(stderr,"\033[0m");
							break;
						case SYS_read:
							fprintf(stderr,"It's a 'read' system call.");
							break;
						case SYS_clone:
							fprintf(stderr,"It's a 'clone' system call.");
							fprintf(stderr,"\nkill child process!\n");
							ptrace(PTRACE_KILL, child_pid, 0, 0);
							break;
						case SYS_fork:
							fprintf(stderr,"It's a 'fork' system call.");
							fprintf(stderr,"\nkill child process!\n");
							ptrace(PTRACE_KILL, child_pid, 0, 0);
							break;
					}
					fprintf(stderr, "\n");
				}
				else if(enter_syscall == 0) {	// return from a system call
					// get the system call return value
					int ret_val = ptrace(PTRACE_PEEKUSER, child_pid, EAX * 4, 0);
					fprintf(stderr, "Return value from system call (ID = %d) is %08x.\n", syscall_id, ret_val);
				}
				else {
					// should not reach here
					assert(0);
				}
				// flip the flag
				enter_syscall = !enter_syscall;

				// trace again
				ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
			}

		}
		else if(WIFEXITED(status) || WIFSIGNALED(status) 
#ifdef WCOREDUMP
				|| WCOREDUMP(status)
#endif
			   ) {
			fprintf(stderr,"Child process exited.\n");
			return;
		}
		else {
			// should not reach here
			assert(0);
		}
	}
}
