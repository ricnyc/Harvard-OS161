#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <addrspace.h>
#include <curthread.h>
#include <vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <vnode.h>


#define DUMBVM_STACKPAGES    12

struct addrspace *
as_create(void)
{
	struct addrspace *as = kmalloc(sizeof(struct addrspace));
	if (as==NULL) {
		return NULL;
	}


	as->as_vbase1 = 0;
	as->as_npages1 = 0;
	as->as_vbase2 = 0;
	as->as_npages2 = 0;
	as->heap_start = 0;

  /* some extra signals */
  as->v = NULL;
  as->EOStack = 0;

	return as;
}





//this is used to flush tlb for the new addr space
void
as_activate(struct addrspace *as)
{
	int i, spl;

	(void)as;

	spl = splhigh();

	for (i=0; i<NUM_TLB; i++) {
		TLB_Write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
	}
	splx(spl);
}

int
as_define_region(struct addrspace *as, vaddr_t vaddr, size_t sz,
		 int readable, int writeable, int executable, int counter)
{

	size_t npages; 


	sz += vaddr & ~(vaddr_t)PAGE_FRAME;
	vaddr &= PAGE_FRAME;

	//This is size of file to be loaded in VM
	sz = (sz + PAGE_SIZE - 1) & PAGE_FRAME;

	//Number of pages used in VM for that file
	npages = sz / PAGE_SIZE;

	(void)readable;
	(void)writeable;
	(void)executable;
	if(counter == 0){
		if (as->as_vbase1 == 0) {
		as->as_vbase1 = vaddr;
		as->as_npages1 = npages;
		//updating the heap starting and ending point
		//vaddr & PAGE_FRAME + sz ----> base VA + Size of VM == end of SEG1 == Start of heap
		if (as->heap_start < ((vaddr & PAGE_FRAME) + sz)) {
	      	as->heap_start = (vaddr & PAGE_FRAME) + sz;
	      	as->heap_end = as->heap_start;
  		}
		return 0;
	}

}
	if(counter == 1){
	if (as->as_vbase2 == 0) {
		as->as_vbase2 = vaddr;
		as->as_npages2 = npages;
		//updating the heap starting and ending point
		if (as->heap_start < ((vaddr & PAGE_FRAME) + sz)) {
	      	as->heap_start = (vaddr & PAGE_FRAME) + sz;
	      	as->heap_end = as->heap_start;
  		}
		return 0;
	}
}

	
	return EUNIMP;
}


int
as_complete_load(struct addrspace *as)
{
	(void)as;
	return 0;
}


int
as_prepare_load(struct addrspace *as,struct vnode *v )
{

	  assert(as->v == NULL);
	  as->v = v;
	  VOP_INCREF(v);

	return 0;
}


int
as_define_stack(struct addrspace *as, vaddr_t *stackptr)
{

	//updating the EOStack
	//This should be the vaddr
	as->EOStack = USERSTACK;

	*stackptr = USERSTACK;
	return 0;
}


int
as_copy(struct addrspace *old, struct addrspace **ret)
{
	struct addrspace *new;

	new = as_create();
	if (new==NULL) {
		return ENOMEM;
	}

	new->as_vbase1 = old->as_vbase1;
	new->as_npages1 = old->as_npages1;
	new->as_vbase2 = old->as_vbase2;
	new->as_npages2 = old->as_npages2;


	
	*ret = new;
	return 0;
}



void
as_destroy(struct addrspace *as)
{
	///////////////////////////A2
	//kfree(as);

  int spl = splhigh();
  //first pagetable
  struct LV_2_PT** L1T = as->lvone;
  //second pagetable
  struct LV_2_PT* L2T = NULL;
  int i, j;


  for (i = 0; i < LEVEL1_SIZE; i++) {
    L2T = L1T[i];
    //secondary pagetable is not empty
    if (L2T != NULL) {
      for (j = 0; j < LEVEL2_SIZE; j++) {
      	//if the coremap is not NULL, delete coremap
        if (L2T->lvtwo[j].coremap_Ptr != NULL) {
          //free the pages allocated by each coremap entries
          //input is the coremap entry vaddr
          L2T->lvtwo[j].coremap_Ptr->as = NULL;
 		  L2T->lvtwo[j].coremap_Ptr->pe_Ptr = NULL;
  		  L2T->lvtwo[j].coremap_Ptr->allocated = 0;
  		  L2T->lvtwo[j].coremap_Ptr->continuous = 0;
          L2T->lvtwo[j].page_address = 0;
          L2T->lvtwo[j].valid  = 0;
          L2T->lvtwo[j].coremap_Ptr = NULL;
        }
      }
    }
  }
  //delete the node for the address_space
  if (as->v) VOP_DECREF(as->v);
  kfree(as);
  splx(spl);
}
