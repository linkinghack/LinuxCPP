#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>


#define STACK_SIZE (1024 * 1024)

static char container_stack[STACK_SIZE];

char* const container_args[] = {
    "/bin/bash",
    NULL,
};

int container_func(void* args) {
    printf("In container_func:: PID=%d\n", getpid());
    execv(container_args[0], container_args);
    printf("Child process exited.\n");
    return 0;
}

int main() {
    int pid = clone(container_func, container_stack + STACK_SIZE, CLONE_NEWPID | SIGCHLD, NULL);
    printf("Parent view: PID=%d\n", pid);
    waitpid(pid, NULL, 0);
    printf("Contianer stopped.\n");
    return 0;
}