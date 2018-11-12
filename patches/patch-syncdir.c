$NetBSD: patch-syncdir.c,v 1.1 2017/07/20 02:07:27 schmonz Exp $

Use externally provided implementation (either syscall or dlsym).

--- syncdir.c.orig	2018-11-12 21:36:38.000000000 +0000
+++ syncdir.c
@@ -25,16 +25,12 @@
 #undef open
 #include <unistd.h>
 #include <string.h>
-#include <syscall.h>
 #include <stdio.h>
 #include <errno.h>
 
-#define SYS_OPEN(FILE,FLAG,MODE) syscall(SYS_open, FILE, FLAG, MODE)
-#define SYS_CLOSE(FD) syscall(SYS_close, FD)
-#define SYS_LINK(OLD,NEW) syscall(SYS_link, OLD, NEW)
-#define SYS_UNLINK(PATH) syscall(SYS_unlink, PATH)
-#define SYS_RENAME(OLD,NEW) syscall(SYS_rename, OLD, NEW)
-#define SYS_FSYNC(FD) syscall(SYS_fsync, FD)
+#include "wrappers.h"
+
+int load_real_syscalls(void);
 
 static int fdirsync(const char* filename, unsigned length)
 {
@@ -43,6 +39,7 @@ static int fdirsync(const char* filename
    * char* dirname = alloca(length+1); */
   int dirfd;
   int retval;
+  load_real_syscalls();
   memcpy(dirname, filename, length);
   dirname[length] = 0;
   if((dirfd = SYS_OPEN(dirname,O_RDONLY,0)) == -1)
@@ -75,7 +72,9 @@ static int fdirsyncfn(const char *filena
 
 int open(const char *file,int oflag,mode_t mode)
 {
-  int fd = SYS_OPEN(file, oflag, mode);
+  int fd;
+  load_real_syscalls();
+  fd = SYS_OPEN(file, oflag, mode);
   if(fd == -1)
     return fd;
   if (oflag & (O_WRONLY | O_RDWR))
@@ -88,6 +87,7 @@ int open(const char *file,int oflag,mode
 
 int link(const char *oldpath,const char *newpath)
 {
+   load_real_syscalls();
    if(SYS_LINK(oldpath,newpath) == -1)
      return -1;
    return fdirsyncfn(newpath);
@@ -95,6 +95,7 @@ int link(const char *oldpath,const char 
 
 int unlink(const char *path)
 {
+   load_real_syscalls();
    if(SYS_UNLINK(path) == -1)
      return -1;
    return fdirsyncfn(path);
@@ -102,6 +103,7 @@ int unlink(const char *path)
 
 int rename(const char *oldpath,const char *newpath)
 {
+   load_real_syscalls();
    if (SYS_RENAME(oldpath,newpath) == -1)
      return -1;
    if (fdirsyncfn(newpath) == -1)
