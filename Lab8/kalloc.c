// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
int getcpu(){
//  push_off();
  int cpu=cpuid();
 // pop_off();
  return cpu;
}

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct kmem{
  struct spinlock lock;
  struct run *freelist;
};

struct kmem kmemm[3];
void
kinit()
{
  for(int i=0;i<3;i++)
  initlock(&kmemm[i].lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
int gcp = getcpu();
  acquire(&kmemm[gcp].lock);
  r->next = kmemm[gcp].freelist;
  kmemm[gcp].freelist = r;
  release(&kmemm[gcp].lock);
}


void * steal(int skip){

struct run * r2=0;
for(int i=0;i<3;i++){
//if(holding(&kmemm[i].lock)){continue;}


acquire(&kmemm[i].lock);
if(kmemm[i].freelist!=0){
r2 = kmemm[i].freelist;
kmemm[i].freelist = r2->next;
release(&kmemm[i].lock);
return (void *) r2;
}
release(&kmemm[i].lock);

}
return (void *)r2;
}
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;
int gcp = getcpu();
  acquire(&kmemm[gcp].lock);
  r = kmemm[gcp].freelist;
  if(r)
    kmemm[gcp].freelist = r->next;
  release(&kmemm[gcp].lock);
if(!r)
	r= steal(gcp);
  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;

}


