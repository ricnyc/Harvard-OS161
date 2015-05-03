#include <types.h>
#include <lib.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <array.h>
#include <machine/spl.h>
#include <machine/pcb.h>
#include <thread.h>
#include <curthread.h>
#include <scheduler.h>
#include <addrspace.h>
#include <vnode.h>
#include "opt-synchprobs.h"
#include <synch.h>


struct thread * init_process(struct thread *thread){
    if (process_lock == NULL) {
      process_lock = lock_create("process_lock");
      if (process_lock == NULL) {
        kfree(thread->t_name);
        kfree(thread);
        thread = NULL;
        return thread;
      }
    }

    //int array_preallocate(struct array *a, int MAXPID)
    
    struct process* temp = kmalloc(sizeof(struct process));
    if (temp == NULL) {
      kfree(thread->t_name);
      kfree(thread);
      thread = NULL;
      return thread;
      panic("thread_bootstrap: Out of memory\n");
    }
    temp->array_cv = cv_create("array_cv");
    if (temp->array_cv == NULL) {
      kfree(thread->t_name);
      kfree(thread);
      kfree(temp);
      thread = NULL;
      temp = NULL;
      return thread;
      panic("thread_bootstrap: create_cv failed\n");
    }
      temp->pid = MINPID;
      temp->ppid = MINPID-1;
      temp->status = PROCESS_RUNNING;
      temp->exitcode = 0; 
      temp->process_ptr = thread;
      thread->array_ptr = temp;
      thread->pid = MINPID;
      array_table[temp->pid] = temp;
      return thread;
}


struct thread * child_process(struct thread *thread){
      struct process* temp = kmalloc(sizeof(struct process));
      if (temp == NULL) {
        kfree(thread->t_name);
        kfree(thread);
        thread = NULL;
        return thread;
      }
      temp->array_cv = cv_create("array_cv");
      if (temp->array_cv == NULL) {
        kfree(thread->t_name);
        kfree(thread);
        kfree(temp);
        thread = NULL;
        temp = NULL;
        return thread;
        //return ENOMEM;
      }
      lock_acquire(process_lock);
      temp->pid = next_pid();
      if(temp->pid == -1){
        cv_destroy(temp->array_cv);
        kfree(thread->t_name);
        kfree(thread);
        kfree(temp);
        thread = NULL;
        temp = NULL;
        return thread;
        //return ENOMEM;
      }
      temp->ppid = curthread->pid;
      temp->status = PROCESS_RUNNING;
      temp->exitcode = 0;
      temp->process_ptr = thread;
      thread->array_ptr = temp;
      thread->pid = temp->pid;
      array_table[temp->pid] = temp;
      lock_release(process_lock);
      return thread;
}