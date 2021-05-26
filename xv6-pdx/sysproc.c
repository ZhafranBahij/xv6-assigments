#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#ifdef PDX_XV6
#include "pdx-kernel.h"
#endif // PDX_XV6
#ifdef CS333_P2
#include "uproc.h"
#endif  // CS333_P2

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      return -1;
    }
    sleep(&ticks, (struct spinlock *)0);
  }
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  xticks = ticks;
  return xticks;
}

#ifdef PDX_XV6
// shutdown QEMU
int
sys_halt(void)
{
  do_shutdown();  // never returns
  return 0;
}
#endif // PDX_XV6

#ifdef CS333_P1

int
sys_date(void){
  struct rtcdate *d;

  if(argptr(0, (void*)&d, sizeof(struct rtcdate)) < 0)
    return -1;

  cmostime(d); //munculin date waktu UTC
  return 0;
}

#endif // CS333_P1 //munculin date

#ifdef CS333_P2
int
sys_getuid(void)
{
  return myproc()->uid;
}
int
sys_getgid(void)
{
  return myproc()->gid;
}
int
sys_getppid(void)
{
  if(myproc()->parent == NULL)
    return myproc()->pid;
  else
    return myproc()->parent->pid;
}
int
sys_setuid(void)
{
  int test;
  if(argint(0, &test)<0)
    return -1;
  if(test < 0 || test >32767)
    return -1;
  else{
    myproc()->uid = test;
    return 0;
  }
}
int
sys_setgid(void)
{
  int test;
  if(argint(0, &test)<0)
    return -1;
  if(test < 0 || test >32767)
    return -1;
  else{
    myproc()->gid = test;
    return 0;
  }
}
int
sys_getprocs(void)
{
  struct uproc *p;
  int max;

  if(argint(0,&max)<0){
    return -1;
  }
  if(argptr(1, (void*)&p, max*sizeof(struct uproc)<0)){return -1;}

  return getprocs(max, p);
}
#endif
