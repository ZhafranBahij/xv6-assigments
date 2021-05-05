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