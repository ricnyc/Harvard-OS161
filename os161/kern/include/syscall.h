#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */



int sys_reboot(int code);
int sys_waitpid(pid_t, int*, int);
int sys_write(int filehandle, const void *buf, size_t size);
int sys_read(int filehandle, void *buf, size_t size);
void _exit(int code);
int getpid(void);
int sys_waitpid (pid_t pid, int* status, int options);
void remove_all_exited_children (pid_t);
int sys_fork(struct trapframe* tf, int* retval);

void remove_all_exited_children (pid_t pid);
struct process* get_process (pid_t pid);
#endif /* _SYSCALL_H_ */
