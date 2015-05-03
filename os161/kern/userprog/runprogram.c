
/* 	 	
* Sample/test code for running a user program.  You can use this for 	 	
* reference when implementing the execv() system call. Remember though 	 	
* that execv() needs to do more than this function does. 	 		 
*/

#include <types.h>
#include <kern/unistd.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <thread.h>
#include <curthread.h>
#include <vm.h>
#include <vfs.h>
#include <test.h>
#include <vnode.h>
//#include <syscall.h>

/*
 * Load program "progname" and start running it in usermode.
 * Does not return except on error.
 *
 * Calls vfs_open on progname and thus may destroy it.
 */
int
runprogram(char *progname,char** argv,unsigned long argc)
{
	struct vnode *v;
	vaddr_t entrypoint, stackptr;
	int result;
	int length[argc];
	int arg_pointer[argc],buf_len,i;
	int offset=0;
	//passing the length of argv
	int j;
	//kprintf("arguments: %s,%s,%s\n",argv[0],argv[1],argv[2]);
	for(j = argc-1; j >= 0; j--) {
		length[j] = strlen(argv[j])+1;
	}
	
	/* Open the file. */

	result = vfs_open(progname, O_RDONLY, &v);//second execution of menu, cant pass this line
	if (result) {
		return result;
	}
	
	
	/* We should be a new thread. */
	assert(curthread->t_vmspace == NULL);

	/* Create a new address space. */
	curthread->t_vmspace = as_create();
	if (curthread->t_vmspace==NULL) {
		vfs_close(v);
		return ENOMEM;
	}

	/* Activate it. */
	as_activate(curthread->t_vmspace);

	/* Load the executable. */
	result = load_elf(v, &entrypoint);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		vfs_close(v);
		return result;
	}

	/* Done with the file now. */
	vfs_close(v);

	/* Define the user stack in the address space */
	result = as_define_stack(curthread->t_vmspace, &stackptr);
	if (result) {
		/* thread_exit destroys curthread->t_vmspace */
		return result;
	}


	
	for(i = argc-1; i >= 0; i--) {

		offset=(length[i] + (4-(length[i]%4)));
		stackptr = stackptr - offset;
		copyoutstr(argv[i], (userptr_t)stackptr, (size_t)length[i], &buf_len);
		arg_pointer[i] = stackptr;
	}


	arg_pointer[argc] = (int)NULL;
	i = argc;
	while(i>=0){

		stackptr = stackptr - 4;

		copyout(&arg_pointer[i] ,(userptr_t)stackptr, sizeof(arg_pointer[i]));
		i--;
	}

	


	/* Warp to user mode. */
	//md_usermode(0 /*argc*/, NULL /*userspace addr of argv*/,
	//	    stackptr, entrypoint);

	md_usermode(argc, (userptr_t)stackptr, stackptr, entrypoint);
	
	/* md_usermode does not return */
	panic("md_usermode returned\n");
	return EINVAL;
}
