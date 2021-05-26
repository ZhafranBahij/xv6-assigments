# Code Modification Report

<h1>Compilation Test</h1>
<h2>Makefile</h2>

```diff
diff --git a/Makefile b/Makefile
index 6483959..c5dc997 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
+CS333_PROJECT ?= 1
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
```

<h1>System Call Tracing</h1>
<h2>Makefile</h2>

```diff
diff --git a/Makefile b/Makefile
index 6483959..dc8d53b 100644
--- a/Makefile
+++ b/Makefile
@@ -1,7 +1,7 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
-PRINT_SYSCALLS ?= 0
+CS333_PROJECT ?= 1
+PRINT_SYSCALLS ?= 1
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
 CS333_UPROGS +=        _halt _uptime
 ```

<h2>syscall.c</h2>

```diff
diff --git a/syscall.c b/syscall.c
index 9105b52..899e6ab 100644
--- a/syscall.c
+++ b/syscall.c
@@ -172,6 +172,9 @@ syscall(void)
   num = curproc->tf->eax;
   if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
     curproc->tf->eax = syscalls[num]();
+    #ifdef PRINT_SYSCALLS
+      cprintf("%s -> %d\n", syscallnames[num], num);
+    #endif
   } else {
     cprintf("%d %s: unknown sys call %d\n",
             curproc->pid, curproc->name, num);
```

<h1>Condition Compilation CS333_PROJECT set to 1</h1>
<h2>Makefile</h2>

```diff
diff --git a/Makefile b/Makefile
index 6483959..c5dc997 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
+CS333_PROJECT ?= 1
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
```

<h1>Date</h1>
<h2>Makefile</h2>

```diff
diff --git a/Makefile b/Makefile
index 6483959..acba3d9 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
+CS333_PROJECT ?= 1
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
@@ -13,7 +13,7 @@ endif

 ifeq ($(CS333_PROJECT), 1)
 CS333_CFLAGS += -DCS333_P1
-CS333_UPROGS += #_date
+CS333_UPROGS += _date
 endif

```

<h2>user.h</h2>

```diff
diff --git a/user.h b/user.h
index 31d9134..46ee67f 100644
--- a/user.h
+++ b/user.h
@@ -43,3 +43,7 @@ int atoi(const char*);
 int atoo(const char*);
 int strncmp(const char*, const char*, uint);
 #endif // PDX_XV6
+
+#ifdef CS333_P1
+int date(struct rtcdate*);
+#endif // CS333_P1 Munculin date
```

<h2>usys.S</h2>

```diff
diff --git a/usys.S b/usys.S
index 0d4eaed..e28e4e5 100644
--- a/usys.S
+++ b/usys.S
@@ -30,3 +30,4 @@ SYSCALL(sbrk)
 SYSCALL(sleep)
 SYSCALL(uptime)
 SYSCALL(halt)
+SYSCALL(date)
\ No newline at end of file
```

<h2>syscall.h</h2>

```diff
diff --git a/syscall.h b/syscall.h
index 7fc8ce1..cf95dab 100644
--- a/syscall.h
+++ b/syscall.h
@@ -22,3 +22,4 @@
 #define SYS_close   SYS_mkdir+1
 #define SYS_halt    SYS_close+1
 // student system calls begin here. Follow the existing pattern.
+#define SYS_date SYS_halt + 1 //munculin date
\ No newline at end of file
```

<h2>syscall.c</h2>

```diff
diff --git a/syscall.c b/syscall.c
index 9105b52..ec9cd9d 100644
--- a/syscall.c
+++ b/syscall.c
@@ -106,6 +106,9 @@ extern int sys_uptime(void);
 #ifdef PDX_XV6
 extern int sys_halt(void);
 #endif // PDX_XV6
+#ifdef CS333_P1
+extern int sys_date(void);
+#endif //CS333_P1

 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -132,6 +135,9 @@ static int (*syscalls[])(void) = {
 #ifdef PDX_XV6
 [SYS_halt]    sys_halt,
 #endif // PDX_XV6
+#ifdef CS333_P1
+[SYS_date]    sys_date,
+#endif // CS333_P1
 };

 #ifdef PRINT_SYSCALLS
@@ -160,6 +166,9 @@ static char *syscallnames[] = {
 #ifdef PDX_XV6
   [SYS_halt]    "halt",
 #endif // PDX_XV6
+#ifdef CS333_P1
+  [SYS_date]    "date",
+#endif // CS333_P1
 };
 #endif // PRINT_SYSCALLS

@@ -172,6 +181,9 @@ syscall(void)
   num = curproc->tf->eax;
   if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
     curproc->tf->eax = syscalls[num]();
+    #ifdef PRINT_SYSCALLS
+      cprintf("%s -> %d\n", syscallnames[num], num);
+    #endif
   } else {
     cprintf("%d %s: unknown sys call %d\n",
             curproc->pid, curproc->name, num);
```

<h2>sysproc.c</h2>

```diff
diff --git a/sysproc.c b/sysproc.c
index 98563ea..bf3d7a5 100644
--- a/sysproc.c
+++ b/sysproc.c
@@ -97,3 +97,16 @@ sys_halt(void)
   return 0;
 }
 #endif // PDX_XV6
+
+#ifdef CS333_P1
+int sys_date(void)
+{
+  struct rtcdate *d;
+
+  if (argptr(0, (void *)&d, sizeof(struct rtcdate)) < 0)
+    return -1;
+
+  cmostime(d); //munculin date waktu UTC
+  return 0;
+}
+#endif // CS333_P1 //munculin date
```

<h1>Control -P</h1>
<h2>Makefile</h2>

```diff
diff --git a/Makefile b/Makefile
index 6483959..c5dc997 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
+CS333_PROJECT ?= 1
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
```

<h2>proc.h</h2>

```diff
diff --git a/proc.h b/proc.h
index 0a0b4c5..be792cd 100644
--- a/proc.h
+++ b/proc.h
@@ -49,6 +49,7 @@ struct proc {
   struct file *ofile[NOFILE];  // Open files
   struct inode *cwd;           // Current directory
   char name[16];               // Process name (debugging)
+  uint start_ticks;            //Buat control p
 };

 // Process memory is laid out contiguously, low addresses first:
```

<h2>proc.c</h2>

```diff
diff --git a/proc.c b/proc.c
index d030537..37fdd7f 100644
--- a/proc.c
+++ b/proc.c
@@ -149,6 +149,8 @@ allocproc(void)
   memset(p->context, 0, sizeof *p->context);
   p->context->eip = (uint)forkret;

+  p->start_ticks = ticks; //Control P
+
   return p;
 }

@@ -563,7 +565,12 @@ procdumpP2P3P4(struct proc *p, char *state_string)
 void
 procdumpP1(struct proc *p, char *state_string)
 {
-  cprintf("TODO for Project 1, delete this line and implement procdumpP1() in proc.c to print a row\n");
+  //empat baris bawah buat control p
+  uint now = ticks - p->start_ticks;
+  uint now_second = now/1000;
+  uint now_milisecond = now%1000;
+  cprintf("%d\t%s\t\t%d.%d\t%s\t%d\t \n", p->pid, p->name, now_second, now_milisecond, states[p->state], p->sz);
+  //cprintf("TODO for Project 1, delete this line and implement procdumpP1() in proc.c to print a row\n");
   return;
 }
 #endif
@@ -914,4 +921,3 @@ checkProcs(const char *file, const char *func, int line)
   }
 }
 #endif
```

<h2>Some File</h2>
Beberapa file yang ditambahkan 
<ul>
 <li>ps.c</li>
 <li>testtest2.c</li>
 <li>testsetuid.c</li>
</ul>
