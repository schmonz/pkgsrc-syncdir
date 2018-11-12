$NetBSD$

Provide alternate dlsym()-backed implementation.

--- trysyscall.c.orig	2018-11-12 21:36:50.000000000 +0000
+++ trysyscall.c
@@ -0,0 +1,5 @@
+#include <unistd.h>
+
+int main(void) {
+  syscall(5);
+}
