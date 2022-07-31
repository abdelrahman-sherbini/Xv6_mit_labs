// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
int pgreference[PHYSTOP/PGSIZE];

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE){
   // pgreference[p]=1;
    kfree(p);};
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
int pgincr(uint64 pa,char symb){
if(pa>=(uint64)end  &&pa <=PHYSTOP){
 //uint64 pa=PTE2PA(pgre);
 int refree =( pa - (uint64)end)/PGSIZE;
if (symb =='+'){
 pgreference[refree]++;}
 else if (symb =='-'){
	 if (pgreference[refree]==0)pgreference[refree]=pgreference[refree];
	 else
 pgreference[refree]--;
 }
else if (symb == '1') return pgreference[refree]=1;
else if (symb == 'v') return pgreference[refree];
return -1;
}
return 0;
}
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;
pgincr((uint64)pa,'-');
if(pgincr((uint64)pa,'v')!=0) return;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);
if (r ) 
	pgincr((uint64)r,'1');
  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
