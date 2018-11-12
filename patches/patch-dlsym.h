$NetBSD$

Provide alternate dlsym()-backed implementation.

--- dlsym.h.orig	2018-11-12 21:36:50.000000000 +0000
+++ dlsym.h
@@ -0,0 +1,13 @@
+extern int (*real_open)();
+extern int (*real_close)();
+extern int (*real_link)();
+extern int (*real_unlink)();
+extern int (*real_rename)();
+extern int (*real_fsync)();
+
+#define SYS_OPEN(FILE,FLAG,MODE) real_open(FILE, FLAG, MODE)
+#define SYS_CLOSE(FD) real_close(FD)
+#define SYS_LINK(OLD,NEW) real_link(OLD, NEW)
+#define SYS_UNLINK(PATH) real_unlink(PATH)
+#define SYS_RENAME(OLD,NEW) real_rename(OLD, NEW)
+#define SYS_FSYNC(FD) real_fsync(FD)
