#ifndef _ADDRSPACE_H_
#define _ADDRSPACE_H_

#include <vm.h>
#include "opt-dumbvm.h"
#include <elf.h>


struct vnode;
//physical pages in the PM
struct page_entry{
	struct coremap_entry* coremap_Ptr;
 	paddr_t   page_address: 20;
 	u_int32_t valid: 1;
};

//secondary pagetable 
struct LV_2_PT {
	struct page_entry lvtwo[LEVEL2_SIZE];
};




struct addrspace {
  //this is abstract of the program files
  struct vnode* v;

  ///position of stack ending point
  int EOStack;

  //position of heap start and heap end
  u_int32_t heap_start;
  u_int32_t heap_end;
 
  //related info for two program header
  size_t as_npages2;
  size_t as_npages1;
  vaddr_t as_vbase1;
  vaddr_t as_vbase2;

  struct LV_2_PT* lvone[LEVEL1_SIZE];//master pagetable
};


//This is for each coremap entry
struct coremap_entry
{
  int allocated;
  int continuous;
  struct addrspace* as; 
  struct page_entry* pe_Ptr;

};

/*
 * Functions in addrspace.c:
 *
 *    as_create - create a new empty address space. You need to make 
 *                sure this gets called in all the right places. You
 *                may find you want to change the argument list. May
 *                return NULL on out-of-memory error.
 *
 *    as_copy   - create a new address space that is an exact copy of
 *                an old one. Probably calls as_create to get a new
 *                empty address space and fill it in, but that's up to
 *                you.
 *
 *    as_activate - make the specified address space the one currently
 *                "seen" by the processor. Argument might be NULL, 
 *		  meaning "no particular address space".
 *
 *    as_destroy - dispose of an address space. You may need to change
 *                the way this works if implementing user-level threads.
 *
 *    as_define_region - set up a region of memory within the address
 *                space.
 *
 *    as_prepare_load - this is called before actually loading from an
 *                executable into the address space.
 *
 *    as_complete_load - this is called when loading from an executable
 *                is complete.
 *
 *    as_define_stack - set up the stack region in the address space.
 *                (Normally called *after* as_complete_load().) Hands
 *                back the initial stack pointer for the new process.
 */



struct addrspace *as_create(void);
int               as_copy(struct addrspace *src, struct addrspace **ret);
void              as_activate(struct addrspace *);
void              as_destroy(struct addrspace *);

int as_define_region(struct addrspace *as, vaddr_t vaddr, size_t sz, int readable, 
				   int writeable,int executable,int interation);
int		  as_prepare_load(struct addrspace *as,struct vnode *v);
int		  as_complete_load(struct addrspace *as);
int     as_define_stack(struct addrspace *as, vaddr_t *stackptr);

int incStack (struct addrspace* as, vaddr_t faultaddress, paddr_t* _paddr);
int allocate_physical_page (struct coremap_entry** page_address);
int incHeap (struct addrspace* as, vaddr_t faultaddress, paddr_t* _paddr);

int load_elf(struct vnode *v, vaddr_t *entrypoint);


#endif /* _ADDRSPACE_H_ */
