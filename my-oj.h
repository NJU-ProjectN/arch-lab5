#ifndef __MY_OJ_H__
#define __MY_OJ_H__

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/syscall.h>

extern void trace(int child_pid);
extern void trace_fun();

#endif
