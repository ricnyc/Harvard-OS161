#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <machine/pcb.h>
#include <machine/spl.h>
#include <machine/trapframe.h>
#include <kern/callno.h>
#include <syscall.h>
#include <curthread.h>
#include <synch.h>
#include <thread.h>
#include <addrspace.h>
struct semaphore* sem_write;
struct semaphore* sem_exit;
int errno;
/*
 * System call handler.
 *
 * A pointer to the trapframe created during exception entry (in
 * exception.S) is passed in.
 *
 * The calling conventions for syscalls are as follows: Like ordinary
 * function calls, the first 4 32-bit arguments are passed in the 4
 * argument registers a0-a3. In addition, the system call number is
 * passed in the v0 register.
 *
 * On successful return, the return value is passed back in the v0
 * register, like an ordinary function call, and the a3 register is
 * also set to 0 to indicate success.
 *
 * On an error return, the error code is passed back in the v0
 * register, and the a3 register is set to 1 to indicate failure.
 * (Userlevel code takes care of storing the error code in errno and
 * returning the value -1 from the actual userlevel syscall function.
 * See src/lib/libc/syscalls.S and related files.)
 *
 * Upon syscall return the program counter stored in the trapframe
 * must be incremented by one instruction; otherwise the exception
 * return code will restart the "syscall" instruction and the system
 * call will repeat forever.
 *
 * Since none of the OS/161 system calls have more than 4 arguments,
 * there should be no need to fetch additional arguments from the
 * user-level stack.
 *
 * Watch out: if you make system calls that have 64-bit quantities as
 * arguments, they will get passed in pairs of registers, and not
 * necessarily in the way you expect. We recommend you don't do it.
 * (In fact, we recommend you don't use 64-bit quantities at all. See
 * arch/mips/include/types.h.)
 */
void remove_all_exited_children (pid_t pid) {
  lock_acquire(pid_list_lock);
  if (pid_list_head == NULL) {
    lock_release(pid_list_lock);
    return;
  }
  struct process* curr = pid_list_head;
  struct process* prev = NULL;
  struct process* to_delete;

  while (curr != NULL) {
    if (curr->ppid == pid && curr->status == 0) {
      to_delete = curr;
      if (prev == NULL) {
        curr = curr->next;
        pid_list_head = pid_list_head->next;
        cv_destroy(to_delete->wait_cv);
        kfree(to_delete);
      } else {
        curr = curr->next;
        prev->next = curr;
        cv_destroy(to_delete->wait_cv);
        kfree(to_delete);
      }
      pid_list_tail = pid_list_head;
      while (pid_list_tail->next != NULL) pid_list_tail = pid_list_tail->next;
    }
    if (curr == NULL) break;
    prev = curr;
    curr = curr->next;
  }
  lock_release(pid_list_lock);
}

struct process* get_process (pid_t pid) {
  if (pid_list_head == NULL) {
    kprintf("error: pid_list_head is NULL\n");
    return NULL;
  }
  struct process* traverser = pid_list_head;
  while (traverser != NULL) {
    if (traverser->pid == pid) {
      return traverser;
    }
    traverser = traverser->next;
  }
  return NULL;
}
int sys_waitpid (pid_t pid, int* status, int options) {
  struct process* child = NULL;
  child = get_process(pid);
  if (child == NULL) return EINVAL;
  if (options != 0) return EINVAL;
  if (status == NULL) return EINVAL;
  lock_acquire(pid_list_lock);
  if (pid_list_head == NULL) panic("sys_waitpid: head list does not exists\n");
  if (child->ppid != curthread->pid) {
    lock_release(pid_list_lock);
    return EINVAL;
  }
  if (child->status == 0) {
    *status = child->exitcode;

    //kprintf("This is *status: %d 0x%lx\n",*status, status);
    //if (*status != 0) kprintf("returning with a value greater than zero.\n");
  } else {
    cv_wait(child->wait_cv,pid_list_lock);
    *status = child->exitcode;
    //kprintf("This is *status: %d 0x%lx\n",*status, status);
    //if (*status != 0) kprintf("returning with a value greater than zero.\n");
  }
  lock_release(pid_list_lock);
  return 0;
}

int sys_write(int filehandle, const void *buf, size_t size){
	//P(sem_write);
	if(filehandle < 0){
		return EBADF;
	}
	if(buf == NULL){
		return EFAULT;
	}
	kprintf("%s", (char*)buf);
	//V(sem_write);
	return 0;
}

void _exit(int code){
	struct process* ptr = curthread->ptr;
  	ptr->status = 0;
  	ptr->exitcode = code;
  	thread_exit();
  	//kprintf("sleep: Dropping thread !!!!!!!!!!!!");
  	return;
}

int getpid(void){
	//kprintf("haha");
    return curthread->pid;
}


void mips_syscall(struct trapframe *tf)
{
	int callno;
	int32_t retval;
	int err;
	 sem_write = sem_create("sem_write", 1);
     sem_exit = sem_create("sem_exit", 1);

	assert(curspl==0);

	callno = tf->tf_v0;

	DEBUG(DB_SYSCALL, "The system call number is %d \n", callno);

	/*
	 * Initialize retval to 0. Many of the system calls don't
	 * really return a value, just 0 for success and -1 on
	 * error. Since retval is the value returned on success,
	 * initialize it to 0 by default; thus it's not necessary to
	 * deal with it except for calls that return other values, 
	 * like write.
	 */

	retval = 0;

	switch (callno) {
	    case SYS_reboot:
			err = sys_reboot(tf->tf_a0);
		break;
	    case SYS_write:
            	err = sys_write(tf->tf_a0, (void *)tf->tf_a1, (size_t) tf->tf_a2);
		break;
	    case SYS__exit:
		//kprintf("exit and I am out of memory\n");
           	 _exit(tf->tf_a0);     
            	err = 0;
		break;
	    /* Add stuff here */
            case SYS_getpid:
            	retval = getpid();
	    	//kprintf("pid value %d\n", retval);
            	err = 0;
        	break;
            case SYS_waitpid:
    		retval  = sys_waitpid(tf->tf_a0, (int *)tf->tf_a1, (int)tf->tf_a2);
		//kprintf("pid value %d\n", retval);
    		err = 0;
    		break;
            case SYS_fork:
   		retval = sys_fork(tf);
		//kprintf("forknum %d and %d\n", retval, ENOMEM);
		
    		if (retval >= 111) {
      			//kprintf("forking right now and I am out of memory\n");
      			err = EAGAIN;
    		}
    		if (retval == ENOMEM) {
      			//kprintf("forking right now and I am out of memory\n");
      			err = ENOMEM;
    		}
    		else err = 0;
    		break;
	    default:
		kprintf("Unknown syscall %d\n", callno);
		err = ENOSYS;
		break;
	}




	if (err) {
		/*
		 * Return the error code. This gets converted at
		 * userlevel to a return value of -1 and the error
		 * code in errno.
		 */
		tf->tf_v0 = err;
		tf->tf_a3 = 1;      /* signal an error */
	}
	else {
		/* Success. */
		tf->tf_v0 = retval;
		tf->tf_a3 = 0;      /* signal no error */
	}
	
	/*
	 * Now, advance the program counter, to avoid restarting
	 * the syscall over and over again.
	 */
	
	tf->tf_epc += 4;

	/* Make sure the syscall code didn't forget to lower spl */
	assert(curspl==0);
}

//void
//md_forkentry(struct trapframe *tf)
//{
	/*
	 * This function is provided as a reminder. You need to write
	 * both it and the code that calls it.
	 *
	 * Thus, you can trash it and do things another way if you prefer.
	 */

	//(void)tf;
//}


void
md_forkentry(struct trapframe *tf, unsigned long as_data)
{
	/*
	 * This function is provided as a reminder. You need to write
	 * both it and the code that calls it.
	 *
	 * Thus, you can trash it and do things another way if you prefer.
	 */

  struct trapframe tf_local;
  struct trapframe* newtf;
  struct addrspace* newas = (struct addrspace*) as_data;

  tf_local = *tf;
  newtf = &tf_local;

  newtf->tf_v0 = 0;
  newtf->tf_a3 = 0;
  newtf->tf_epc += 4;

  curthread->t_vmspace = newas;

  as_activate(curthread->t_vmspace);

  //kfree(tf);
  mips_usermode(&tf_local);
}

int sys_fork(struct trapframe* tf/*, int* retval*/) {
  //kprintf("fork is getting called\n");

//kprintf("eeee");
  struct thread* newthread;
  struct trapframe* newtf;
  struct addrspace* newas;
  //struct trapframe tf_local;
  int result;
	
  newtf = (struct trapframe*) kmalloc(sizeof(struct trapframe));

  if (newtf == NULL) {
     kfree(newtf);
    //*retval = ENOMEM;
	//kprintf("haha");
    return ENOMEM;
  }
  *newtf = *tf;

  int ret = as_copy(curthread->t_vmspace, &newas);
  if (newas == NULL || ret != 0) {
    kfree(newtf);
    //*retval = ENOMEM;
	//kprintf("haha");
    return ENOMEM;
  }

  as_activate(curthread->t_vmspace);

  result = thread_fork(curthread->t_name, newtf,(unsigned long)newas,(void (*)(void *, unsigned long)) md_forkentry,&newthread);

  if (result) {
    kfree(newtf);
	//kprintf("haha");
    return ENOMEM;
  }
  
  //newthread->ppid = curthread->pid;
  //newthread->pid = add_pid();

  tf->tf_v0 = newthread->pid;

  return newthread->pid;

}
