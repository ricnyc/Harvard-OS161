#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_getpid(void);
int sys_waitpid(pid_t, int*, int, int* retval);
int sys_fork(struct trapframe* tf, int* retval);
int sys_execv(char *program, char **args);
//int sys_fork(struct trapframe* tf);
int sys_read(int fd, /*userptr_t buf*/ char* buf, size_t size, int* retval);
int
sys_execv(char *progname, char **args);
int sys__exit (int);
int sys_write(int filehandle, const void *buf, size_t size);
int sys_sbrk(int byte);
//int sys_read(int filehandle, void *buf, size_t size);
struct process* get_process (pid_t pid);

#endif /* _SYSCALL_H_ */