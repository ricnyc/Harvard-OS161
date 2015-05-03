#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>
#include <uio.h>
#include <vnode.h>


int allocate_physical_page (struct coremap_entry** page_address) {
  struct coremap_entry* coremap_ent;
  for(coremap_ent = coremap.coremap_str; coremap_ent < (coremap.coremap_str + coremap.numofpage); coremap_ent++) {
    if ((coremap_ent < (coremap.coremap_str + coremap.numofpage)) && coremap_ent->allocated == 0) {
      coremap_ent->allocated = 1;
      *page_address = coremap_ent;
      return 0;
    }
  }
  return -1;
}

//find a page in stack and set up a mapping to page in PM
int incStack (struct addrspace* as, vaddr_t faultaddress, paddr_t* _paddr) {

  struct coremap_entry* page_address = NULL;
  int paddr;
  //get one physical page
  if (allocate_physical_page(&page_address)) {
    return -1; 
  }

  //map one VA to a coremap entry in PM///////////////////////////////////
  u_int32_t L1Tindex = (unsigned)faultaddress>>22;
  u_int32_t L2Tindex = ((unsigned)faultaddress>>12)&page_frame_mask;
  struct LV_2_PT** L1T = as->lvone;
  struct LV_2_PT* L2T = L1T[L1Tindex];

  //allocate one master page linked to one secondary page table with 1024 entries
  //no master page table to allocate, then create a new one
  if (L2T == NULL) {
    L2T = kmalloc(sizeof(struct LV_2_PT));
    if (L2T == NULL) return ENOMEM;
    bzero(L2T, sizeof(struct LV_2_PT));
    struct page_entry* pe_Ptr = L2T->lvtwo;
    int i;
    //all entry are invaild with no pages attached
    for (i = 0; i < LEVEL2_SIZE; i++) {
      pe_Ptr[i].valid = 0;
      pe_Ptr[i].coremap_Ptr = NULL;
    }
    //link master table to as
    L1T[L1Tindex] = L2T;
  }
  //linked a page entry to master pagetable
  struct page_entry* pe_Ptr = &(L2T->lvtwo[L2Tindex]);
  pe_Ptr->valid = 1;
  //add the physical addr
  pe_Ptr->page_address = coremap.free_pages_str + (page_address - coremap.coremap_str)*PAGE_SIZE;
  pe_Ptr->coremap_Ptr = page_address; 
  page_address->as = as; 
  page_address->pe_Ptr = pe_Ptr;
  page_address->continuous = 1;
  //map one VA to a coremap entry in PM///////////////////////////////////

  //find a physical addr in PM
  paddr = coremap.free_pages_str + (page_address - coremap.coremap_str)*4096;
  bzero((void*)PADDR_TO_KVADDR(paddr), PAGE_SIZE);

  as->EOStack = as->EOStack - PAGE_SIZE;

  *_paddr = paddr;
  return 0;
}


int incHeap (struct addrspace* as, vaddr_t faultaddress, paddr_t* _paddr) {

  struct coremap_entry* page_address = NULL;
  int paddr;
  if (allocate_physical_page(&page_address)) {
    return -1; 
  }

  //map one VA to a coremap entry in PM///////////////////////////////////
  u_int32_t L1Tindex = (unsigned)faultaddress>>22;
  u_int32_t L2Tindex = ((unsigned)faultaddress>>12)&page_frame_mask;
  struct LV_2_PT** L1T = as->lvone;
  struct LV_2_PT* L2T = L1T[L1Tindex];

  //allocate one master page linked to one secondary page table with 1024 entries
  //no master page table to allocate, then create a new one
  if (L2T == NULL) {
    L2T = kmalloc(sizeof(struct LV_2_PT));
    if (L2T == NULL) return ENOMEM;
    bzero(L2T, sizeof(struct LV_2_PT));
    struct page_entry* pe_Ptr = L2T->lvtwo;
    int i;
    //all entry are invaild with no pages attached
    for (i = 0; i < LEVEL2_SIZE; i++) {
      pe_Ptr[i].valid = 0;
      pe_Ptr[i].coremap_Ptr = NULL;
    }
    //link master table to as
    L1T[L1Tindex] = L2T;
  }
  //linked a page entry to master pagetable
  struct page_entry* pe_Ptr = &(L2T->lvtwo[L2Tindex]);
  pe_Ptr->valid = 1;
  //add the physical addr
  pe_Ptr->page_address = coremap.free_pages_str + (page_address - coremap.coremap_str)*PAGE_SIZE;
  pe_Ptr->coremap_Ptr = page_address; 
  page_address->as = as; 
  page_address->pe_Ptr = pe_Ptr;
  page_address->continuous = 1;
  //map one VA to a coremap entry in PM///////////////////////////////////


  paddr = coremap.free_pages_str + (page_address - coremap.coremap_str)*4096;
  bzero((void*)PADDR_TO_KVADDR(paddr), PAGE_SIZE);
  *_paddr = paddr;
  return 0;
}
