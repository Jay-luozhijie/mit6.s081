#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  uint64 va;
  int pageNum;
  uint64 bufAddr;
  pte_t *pte;

  argaddr(0, &va);
  argint(1, &pageNum);
  argaddr(2, &bufAddr);


  if (pageNum > 64) return -1;
  char *bitmap = kalloc();
  memset(bitmap, 0, PGSIZE);
  char *bitmapPtr = bitmap;
  int offset = 0;
  int mask  = 1;


  uint64 a = PGROUNDDOWN(va);
  for (int i = 0; i < pageNum; i++){
    if ((pte = walk(myproc()->pagetable, a, 0)) != 0){
      if (*pte & PTE_A){
        *bitmapPtr |= (mask<<offset);
        *pte &= ~PTE_A;
      }
    }
    bitmapPtr += (offset + 1) / 8;
    offset = (offset + 1) % 8;
    a+= PGSIZE;
  }
  
  copyout(myproc()->pagetable, bufAddr, bitmap, sizeof(bitmap));
  return 0;
}
#endif



uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}