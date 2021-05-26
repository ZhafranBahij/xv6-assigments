<h1>Code Modification</h1>

<h2>defs.h</h2>

``` diff
diff --git a/defs.h b/defs.h
index f85557d..7bb91a2 100644
--- a/defs.h
+++ b/defs.h
@@ -124,6 +124,9 @@ void            userinit(void);
 int             wait(void);
 void            wakeup(void*);
 void            yield(void);
+#ifdef CS333_P2
+int getprocs(uint max, struct uproc *table);
+#endif
 #ifdef CS333_P3
 void            printFreeList(void);
 void            printList(int);
```

<h2>Makefile</h2>

``` diff
diff --git a/Makefile b/Makefile
index 6d6fa03..0cac351 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 1
+CS333_PROJECT ?= 2
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
```

<h2>pdx.h</h2>

```diff
diff --git a/pdx.h b/pdx.h
index a1b55ca..1fbfa02 100644
--- a/pdx.h
+++ b/pdx.h
@@ -19,7 +19,15 @@
 #define TPS 1000   // ticks-per-second
 #define SCHED_INTERVAL (TPS/100)  // see trap.c

+#define UID 0
+#define GID 0
+#define P2TICKS 0
+
 #ifdef CS333_P2
+// for ps.c and getprocs()
+#define MAX 72
+#define STRSIZE 11
+
 #define DEFAULT_UID 0
 #define DEFAULT_GID 0
 #endif // CS333_P2
 ```

<h2>proc.c</h2>

 ``` diff
diff --git a/proc.c b/proc.c
index 37fdd7f..a32acef 100644
--- a/proc.c
+++ b/proc.c
@@ -6,6 +6,9 @@
 #include "x86.h"
 #include "proc.h"
 #include "spinlock.h"
+#ifdef CS333_P2
+#include "uproc.h"  // Tugas 2
+#endif  //CS333_P2

 static char *states[] = {
   [UNUSED]    "unused",
@@ -126,6 +129,11 @@ allocproc(void)
   }
   p->state = EMBRYO;
   p->pid = nextpid++;
+
+  #ifdef CS333_P2
+    p->gid = GID;
+    p->uid = UID;
+  #endif //CS333_P2
   release(&ptable.lock);

   // Allocate kernel stack.
@@ -150,7 +158,10 @@ allocproc(void)
   p->context->eip = (uint)forkret;

   p->start_ticks = ticks; //Control P
-
+  #ifdef CS333_P2
+    p->cpu_ticks_in = P2TICKS;
+    p->cpu_ticks_total = P2TICKS;
+  #endif //CS333_P2
   return p;
 }

@@ -177,7 +188,10 @@ userinit(void)
   p->tf->eflags = FL_IF;
   p->tf->esp = PGSIZE;
   p->tf->eip = 0;  // beginning of initcode.S
-
+  #ifdef CS333_P2
+    p->uid = UID;
+    p->gid = GID;
+  #endif //CS333_P2
   safestrcpy(p->name, "initcode", sizeof(p->name));
   p->cwd = namei("/");

@@ -249,7 +263,11 @@ fork(void)
   safestrcpy(np->name, curproc->name, sizeof(curproc->name));

   pid = np->pid;
-
+  #ifdef CS333_P2
+    // setting new process gid and uid to current process gid and uid it forked from
+    np->gid = curproc->gid;
+    np->uid = curproc->uid;
+  #endif //CS333_P2
   acquire(&ptable.lock);
   np->state = RUNNABLE;
   release(&ptable.lock);
@@ -364,6 +382,9 @@ scheduler(void)
 {
   struct proc *p;
   struct cpu *c = mycpu();
+#ifdef CS333_P2
+  uint ticks_out = 0;
+#endif //CS333_P2
   c->proc = 0;
 #ifdef PDX_XV6
   int idle;  // for checking if processor is idle
@@ -388,6 +409,9 @@ scheduler(void)
 #ifdef PDX_XV6
       idle = 0;  // not idle this timeslice
 #endif // PDX_XV6
+#ifdef CS333_P2
+      p->cpu_ticks_in = ticks;
+#endif //CS333_P2
       c->proc = p;
       switchuvm(p);
       p->state = RUNNING;
@@ -396,6 +420,11 @@ scheduler(void)

       // Process is done running for now.
       // It should have changed its p->state before coming back.
+      #ifdef CS333_P2
+            ticks_out = ticks;
+            p->cpu_ticks_total += ticks_out - p->cpu_ticks_in;
+            ticks_out = 0;
+      #endif //CS333_P2
       c->proc = 0;
     }
     release(&ptable.lock);
@@ -555,10 +584,18 @@ kill(int pid)
 // No lock to avoid wedging a stuck machine further.

 #if defined(CS333_P2)
-void
-procdumpP2P3P4(struct proc *p, char *state_string)
+void procdumpP2P3P4(struct proc *p, char *state_string)
 {
-  cprintf("TODO for Project 2, delete this line and implement procdumpP2P3P4() in proc.c to print a row\n");
+  uint x;
+  if (p->parent == NULL)
+  {
+    x = p->pid;
+  }
+  else
+    x = p->parent->pid;
+
+  cprintf("%d\t%s\t%d\t%d\t%d\t%d.%d\t%d.%d\t%s\t%d\t", p->pid, p->name, p->uid, p->gid, x, (ticks - p->start_ticks) / 1000, (ticks - p->start_ticks) % 1000, (p->cpu_ticks_total) / 1000, (p->cpu_ticks_total) % 1000, state_string, p->sz);
+
   return;
 }
 #elif defined(CS333_P1)
@@ -624,6 +661,37 @@ procdump(void)
   cprintf("$ ");  // simulate shell prompt
 #endif // CS333_P1
 }
+#ifdef CS333_P2
+int getprocs(uint max, struct uproc *table)
+{
+  struct proc *p;
+  int counter = 0;
+
+  acquire(&ptable.lock);
+  for (p = ptable.proc; p < &ptable.proc[NPROC] && counter < max; ++p)
+  {
+    if (p->state == UNUSED || p->state == EMBRYO)
+      continue;
+    table->pid = p->pid;
+    table->uid = p->uid;
+    table->gid = p->gid;
+    if (p->pid == 1)
+      table->ppid = 1;
+    else
+      table->ppid = p->parent->pid;
+
+    table->elapsed_ticks = ticks - p->start_ticks;
+    table->CPU_total_ticks = p->cpu_ticks_total;
+    table->size = p->sz;
+    safestrcpy(table->state, states[p->state], sizeof(table->state) / sizeof(char));
+    safestrcpy(table->name, p->name, sizeof(table->name) / sizeof(char));
+    table++;
+    counter++;
+  }
+  release(&ptable.lock);
+  return counter;
+}
+#endif //CS333_P2

 #if defined(CS333_P3)
 // list management helper functions
```

<h2>proc.h</h2>

```diff
diff --git a/proc.h b/proc.h
index 4b5386b..95a3a35 100644
--- a/proc.h
+++ b/proc.h
@@ -36,6 +36,12 @@ enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

 // Per-process state
 struct proc {
+  #ifdef CS333_P2 //Tambahan Tugas 2
+    uint uid;
+    uint gid;
+    uint cpu_ticks_total;
+    uint cpu_ticks_in;
+  #endif                         //CS333_P2
   uint sz;                     // Size of process memory (bytes)
   pde_t* pgdir;                // Page table
   char *kstack;                // Bottom of kernel stack for this process
```

<h2>runoff.list</h2>

```diff
diff --git a/runoff.list b/runoff.list
index 81930d9..5e9f7e6 100644
--- a/runoff.list
+++ b/runoff.list
@@ -92,3 +92,12 @@ p2-test.c
 p3-test.c
 p4-test.c
 testsetprio.c
+
+#additions for P2
+testsetuid.c
+testuidgid.c
+p2-test.c
+uproc.h
+ps.c
+time.c
+testtest2.c
\ No newline at end of file
```

<h2>syscall.c</h2>

```diff
diff --git a/syscall.c b/syscall.c
index 07e83a7..11ac94a 100644
--- a/syscall.c
+++ b/syscall.c
@@ -109,6 +109,15 @@ extern int sys_halt(void);
 #ifdef CS333_P1
 extern int sys_date(void);
 #endif //CS333_P1
+//Tambahan dari Tugas 02
+#ifdef CS333_P2
+extern int sys_getuid(void);
+extern int sys_getgid(void);
+extern int sys_getppid(void);
+extern int sys_setuid(void);
+extern int sys_setgid(void);
+extern int sys_getprocs(void);
+#endif //CS333_P2

 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -138,6 +147,14 @@ static int (*syscalls[])(void) = {
 #ifdef CS333_P1
 [SYS_date]    sys_date,
 #endif // CS333_P1
+#ifdef CS333_P2
+[SYS_getuid]  sys_getuid,
+[SYS_getgid]  sys_getgid,
+[SYS_getppid] sys_getppid,
+[SYS_setuid]  sys_setuid,
+[SYS_setgid]  sys_setgid,
+[SYS_getprocs]  sys_getprocs,
+#endif  // CS333_P2

 };

@@ -170,6 +187,15 @@ static char *syscallnames[] = {
 #ifdef CS333_P1
   [SYS_date]    "date",
 #endif // CS333_P1
+#ifdef CS333_P2
+  [SYS_getuid]  "getuid",
+  [SYS_getgid]  "getgid",
+  [SYS_getppid] "getppid",
+  [SYS_setuid]  "setuid",
+  [SYS_setgid]  "setgid",
+  [SYS_getprocs] "getprocs"
+
+#endif // CS333_P2
 };
 #endif // PRINT_SYSCALLS
```

<h2>syscall.h</h2>

```diff
diff --git a/syscall.h b/syscall.h
index 57fb797..99fd107 100644
--- a/syscall.h
+++ b/syscall.h
@@ -23,3 +23,9 @@
 #define SYS_halt    SYS_close+1
 // student system calls begin here. Follow the existing pattern.
 #define SYS_date    SYS_halt+1 //munculin date
+#define SYS_getuid  SYS_date+1 //Tambahan tugas 2
+#define SYS_getgid  SYS_getuid+1
+#define SYS_getppid  SYS_getgid+1
+#define SYS_setuid  SYS_getppid+1
+#define SYS_setgid  SYS_setuid+1
+#define SYS_getprocs SYS_setgid+1
\ No newline at end of file
```

<h2>sysproc.c</h2>

```diff
diff --git a/sysproc.c b/sysproc.c
index 8292584..e562cfe 100644
--- a/sysproc.c
+++ b/sysproc.c
@@ -9,6 +9,9 @@
 #ifdef PDX_XV6
 #include "pdx-kernel.h"
 #endif // PDX_XV6
+#ifdef CS333_P2
+#include "uproc.h"
+#endif // CS333_P2

 int
 sys_fork(void)
@@ -112,3 +115,62 @@ sys_date(void){
 }

 #endif // CS333_P1 //munculin date
+#ifdef CS333_P2
+int sys_getuid(void)
+{
+  return myproc()->uid;
+}
+int sys_getgid(void)
+{
+  return myproc()->gid;
+}
+int sys_getppid(void)
+{
+  if (myproc()->parent == NULL)
+    return myproc()->pid;
+  else
+    return myproc()->parent->pid;
+}
+int sys_setuid(void)
+{
+  int test;
+  if (argint(0, &test) < 0)
+    return -1;
+  if (test < 0 || test > 32767)
+    return -1;
+  else
+  {
+    myproc()->uid = test;
+    return 0;
+  }
+}
+int sys_setgid(void)
+{
+  int test;
+  if (argint(0, &test) < 0)
+    return -1;
+  if (test < 0 || test > 32767)
+    return -1;
+  else
+  {
+    myproc()->gid = test;
+    return 0;
+  }
+}
+int sys_getprocs(void)
+{
+  struct uproc *p;
+  int max;
+
+  if (argint(0, &max) < 0)
+  {
+    return -1;
+  }
+  if (argptr(1, (void *)&p, max * sizeof(struct uproc) < 0))
+  {
+    return -1;
+  }
+
+  return getprocs(max, p);
+}
+#endif
\ No newline at end of file
```

<h2>uproc.h</h2>

```diff
diff --git a/uproc.h b/uproc.h
index 997ec25..fc8b6e9 100644
--- a/uproc.h
+++ b/uproc.h
@@ -1,6 +1,5 @@
-#ifndef UPROC_H
-#define UPROC_H
 #define STRMAX 32
+#include "types.h"

 struct uproc {
   uint pid;
```

<h2>user.h</h2>

```diff
diff --git a/user.h b/user.h
index 094e533..f7d4c9a 100644
--- a/user.h
+++ b/user.h
@@ -46,4 +46,15 @@ int strncmp(const char*, const char*, uint);

 #ifdef CS333_P1
 int date(struct rtcdate*);
-#endif // CS333_P1 Munculin date
\ No newline at end of file
+#endif // CS333_P1 Munculin date
+
+#ifdef CS333_P2
+uint getuid(void); //UID of the current process
+uint getgid(void); //GID of the current process
+uint getppid(void); //proccess ID of the parent Process
+
+int setuid(uint); //set UID
+int setgid(uint); //set GID
+
+int getprocs(uint max, struct uproc *table); //Get proc
+#endif // CS333_P2
```

<h2>usys.S</h2>

```diff
diff --git a/usys.S b/usys.S
index 84bd80b..6ee47b8 100644
--- a/usys.S
+++ b/usys.S
@@ -31,3 +31,9 @@ SYSCALL(sleep)
 SYSCALL(uptime)
 SYSCALL(halt)
 SYSCALL(date)
+SYSCALL(getuid)
+SYSCALL(getgid)
+SYSCALL(getppid)
+SYSCALL(setuid)
+SYSCALL(setgid)
+SYSCALL(getprocs)
```

<h2>Some File</h2>
Beberapa file yang ditambahkan 
<ul>
 <li>ps.c</li>
 <li>testtest2.c</li>
 <li>testsetuid.c</li>
</ul>
