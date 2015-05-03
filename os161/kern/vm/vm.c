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



/*
 * Dumb MIPS-only "VM system" that is intended to only be just barely
 * enough to struggle off the ground. You should replace all of this
 * code while doing the VM assignment. In fact, starting in that
 * assignment, this file is not included in your kernel!
 */





void
vm_bootstrap(void)
{

  u_int32_t start , end;
  u_int32_t coremap_str, lastpage , freepages , mapsize, coremap_pages;
  //To get the physical memory size from low to high
  ram_getsize(&start, &end);
  
  coremap_str=(start)&PAGE_FRAME; //mask out addr
  lastpage=(end)&PAGE_FRAME;
  //total number of pages available in the RAM
  freepages = (lastpage - coremap_str) >> 12; 

  //since each coremap entry may map to one or more pages, so the 
  //worst case is one to one relation

  //used RAM for the coremap
  //coremap_pages is number of pages that coremap used in the RAM
  mapsize=freepages*sizeof(struct coremap_entry); 
  coremap_pages=mapsize/4096+1; 

  //will be upated
  //start is the first PM addr
  //coremap.free_pages_str = coremap_str;
  coremap.numofpage = freepages;

  //storing the first coremap entry vaddr
  coremap.coremap_str = (struct coremap_entry*)PADDR_TO_KVADDR(start);
  
  //set up the starting point of available phyical address
  coremap.free_pages_str= coremap_str+(coremap_pages<<12); 
  
  //updating number of page available in physical memory
  coremap.numofpage=coremap.numofpage-coremap_pages;


}

//static
paddr_t getppages(unsigned long npages)
{
  ///////////////////////////A3
  int spl;
  int counter=0;
  paddr_t addr;

  spl = splhigh();
  //first enter to find a physical page
  if(coremap.coremap_str == NULL){
    addr = ram_stealmem(npages);
    splx(spl);
    return addr;
  }
  else{
    //the starting vaddr of coremap and increment by coremap entry
    struct coremap_entry* lcr=coremap.coremap_str;
    //This is the starting 
    struct coremap_entry* freepgstart;


    
    //check the free phyiscal page
      for(lcr = coremap.coremap_str; lcr<(coremap.coremap_str + coremap.numofpage) && lcr->allocated==1; lcr++)
      {}
      freepgstart=lcr; //Store the starting point of the free page
      for(;lcr<(coremap.coremap_str + coremap.numofpage) && (lcr->allocated==0);lcr++) 
      {

        counter++;
        if (counter>=npages){ 
        //find the physical memory addr(base + # of pages)
        //1 coremap maps 1 physical memory
        addr=(paddr_t)(coremap.free_pages_str + ((freepgstart) - coremap.coremap_str) * 4096);
        freepgstart->continuous = npages;
        
        //initailize the n coremap entries
        for(counter=0; counter<npages; counter++)
        {
            freepgstart[counter].allocated = 1;
        }
        //updating # of pages left in the PM  
 
        splx(spl);
        return addr;
        } 
    
      }
     
      

  }
  splx(spl);
  return 0;
}


vaddr_t 
alloc_kpages(int npages)
{
  paddr_t pa;
  pa = getppages(npages);
  if (pa==0) {
    return 0;
  }
  return PADDR_TO_KVADDR(pa);
}

void 
free_kpages(vaddr_t addr)
{
  /* nothing */
  ////////////////////////////////A3
  int spl;
  spl = splhigh();

  //convert into the paddr
  paddr_t paddr = KVADDR_TO_PADDR(addr);
  
  //get starting coremap entry (base + # of physical page blocks)
  struct coremap_entry* delete_page;
  delete_page =(coremap.coremap_str + (((u_int32_t)(paddr) - coremap.free_pages_str) >> 12)); 
  int lcr = 0; 
  //free the  coremap entries space starting from current page 
  for (lcr = 0; lcr < delete_page->continuous; lcr++) 
  {
    delete_page[lcr].allocated=0; 
  } 
  //updating page left in physical memory

  delete_page->continuous = 0;
  splx(spl);
  return;
}



int
vm_fault(int faulttype, vaddr_t faultaddress)
{
  vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
  paddr_t paddr;
  int i;
  u_int32_t ehi, elo;
  struct addrspace *as;
  int spl;

  spl = splhigh();

  faultaddress &= PAGE_FRAME;

  DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);

  switch (faulttype) {
      case VM_FAULT_READONLY:

    panic("dumbvm: got VM_FAULT_READONLY\n");
      case VM_FAULT_READ:
      case VM_FAULT_WRITE:
    break;
      default:
    splx(spl);
    return EINVAL;
  }

  as = curthread->t_vmspace;
  if (as == NULL) {

    return EFAULT;
  }


  int result = 1;
  //This is the first PageTable index(first 10 bits of 32 bits)
  u_int32_t L1Tindex = (unsigned)faultaddress>>22;

  //This is second 10 bits of 32 bit VA (for secondary pagetable)
  u_int32_t L2Tindex = ((unsigned)faultaddress>>12)&page_frame_mask;
  struct LV_2_PT** L1T = as->lvone;
  //lvone is an array of first level page table entries
  //LV_2_PT is a secondary page table pointer and point to 1024 secondary page table entrys
  struct LV_2_PT* L2T = L1T[L1Tindex];

  if (L2T == NULL) {
    result = 1;
  }
  //The level two page table pointing to one page entry
  //pe_Ptr storing one physical page
  else {
    struct page_entry* pe_Ptr = &(L2T->lvtwo[L2Tindex]);
    if (pe_Ptr->valid == 0){ 
      result = 1;
    }else{
      paddr = pe_Ptr->page_address;
      result = 0;
    }


  }



  ///////////////////////////A3
  //if we can find a translation, it will directly return a paddr, ret 0
  //other to will set a mapping and a free page, ret 0
  //if fails, ret -1

 
  if (result) {
      int position =10;
      if ((USERSTACK - as->EOStack) >= stack_size) {
        position = 0; 
      }
         
      if (faultaddress <= USERSTACK && faultaddress >= (as->EOStack - PAGE_SIZE)){
        position = 0;
      }
      if (faultaddress >= as->heap_start && faultaddress <= as->heap_end){
         position = 1;
      } 
      
      switch(position){
        case 0:
            result = incStack(as, faultaddress, &paddr);
          break;
        case 1:
            result = incHeap(as, faultaddress, &paddr);
          break;
        default:
          break;
      }

      if (result) {
            splx(spl);
          return EFAULT;
      }
    }






  /* make sure it's page-aligned */
  assert((paddr & PAGE_FRAME)==paddr);

  for (i=0; i<NUM_TLB; i++) {
    TLB_Read(&ehi, &elo, i);
    if (elo & TLBLO_VALID) {
      continue;
    }
    ehi = faultaddress;
    elo = paddr | TLBLO_DIRTY | TLBLO_VALID;

    TLB_Write(ehi, elo, i);
    splx(spl);
    return 0;
  }

  //flush random tlb entry 
  int number = ((unsigned int)random() % NUM_TLB);
    TLB_Write(TLBHI_INVALID(number), TLBLO_INVALID(), number);

  for (i=0; i<NUM_TLB; i++) {
    TLB_Read(&ehi, &elo, i);
    if (elo & TLBLO_VALID) {
      continue;
    }
    ehi = faultaddress;
    elo = paddr | TLBLO_DIRTY | TLBLO_VALID;
 
    TLB_Write(ehi, elo, i);
    splx(spl);
    return 0;
  }


  
  splx(spl);
  return EFAULT;
}

