/* durian.cc
 * Copyright 2016 Mac Radigan
 * All Rights Reserved
 */

#include "packages/common/status.h"

#include <getopt.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite()/utimensat() */
#define _XOPEN_SOURCE 700
#endif

  #define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

//static void *durian_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
//{
//  (void) conn;
//  cfg->use_ino = 1;
//  cfg->entry_timeout = 0;
//  cfg->attr_timeout = 0;
//  cfg->negative_timeout = 0;
//  return NULL;
//}
//
//static int durian_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
//{
//  (void) fi;
//  int res;
//  res = lstat(path, stbuf);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_access(const char *path, int mask)
//{
//  int res;
//  res = access(path, mask);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_readlink(const char *path, char *buf, size_t size)
//{
//  int res;
//  res = readlink(path, buf, size - 1);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  buf[res] = '\0';
//  return 0;
//}
//
//static int durian_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
//{
//  DIR *dp;
//  struct dirent *de;
//
//  (void) offset;
//  (void) fi;
//  (void) flags;
//
//  dp = opendir(path);
//  if (dp == NULL)
//  {
//    return -errno;
//  }
//  while ((de = readdir(dp)) != NULL) 
//  {
//    struct stat st;
//    memset(&st, 0, sizeof(st));
//    st.st_ino = de->d_ino;
//    st.st_mode = de->d_type << 12;
//    if (filler(buf, de->d_name, &st, 0, 0))
//    {
//      break;
//    }
//  }
//  closedir(dp);
//  return 0;
//}
//
//static int durian_mknod(const char *path, mode_t mode, dev_t rdev)
//{
//  int res;
//  /* On Linux this could just be 'mknod(path, mode, rdev)' but this is more portable */
//  if (S_ISREG(mode))
//  {
//    res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
//    if (res >= 0)
//    {
//      res = close(res);
//    } 
//  } 
//  else if (S_ISFIFO(mode))
//  {
//    res = mkfifo(path, mode);
//  } 
//  else
//  {
//    res = mknod(path, mode, rdev);
//  } 
//  if (res == -1)
//  {
//    return -errno;
//  } 
//  return 0;
//}
//
//static int durian_mkdir(const char *path, mode_t mode)
//{
//  int res;
//  res = mkdir(path, mode);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_unlink(const char *path)
//{
//  int res;
//  res = unlink(path);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_rmdir(const char *path)
//{
//  int res;
//  res = rmdir(path);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_symlink(const char *from, const char *to)
//{
//  int res;
//  res = symlink(from, to);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_rename(const char *from, const char *to, unsigned int flags)
//{
//  int res;
//  if (flags)
//  {
//    return -EINVAL;
//  }
//  res = rename(from, to);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_link(const char *from, const char *to)
//{
//  int res;
//  res = link(from, to);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
//{
//  (void) fi;
//  int res;
//  res = chmod(path, mode);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
//{
//  (void) fi;
//  int res;
//  res = lchown(path, uid, gid);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_truncate(const char *path, off_t size, struct fuse_file_info *fi)
//{
//  int res;
//  if (fi != NULL)
//  {
//    res = ftruncate(fi->fh, size);
//  }
//  else
//  {
//    res = truncate(path, size);
//  }
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//#ifdef HAVE_UTIMENSAT
//static int durian_utimens(const char *path, const struct timespec ts[2],
//           struct fuse_file_info *fi)
//{
//  (void) fi;
//  int res;
//  /* don't use utime/utimes since they follow symlinks */
//  res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//#endif
//
//static int durian_create(const char *path, mode_t mode, struct fuse_file_info *fi)
//{
//  int res;
//  res = open(path, fi->flags, mode);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  fi->fh = res;
//  return 0;
//}
//
//static int durian_open(const char *path, struct fuse_file_info *fi)
//{
//  int res;
//  res = open(path, fi->flags);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  fi->fh = res;
//  return 0;
//}
//
//static int durian_read(const char *path, char *buf, size_t size, off_t offset,
//        struct fuse_file_info *fi)
//{
//  int fd;
//  int res;
//  if(fi == NULL)
//  {
//    fd = open(path, O_RDONLY);
//  }
//  else
//  {
//    fd = fi->fh;
//  }
//  if (fd == -1)
//  {
//    return -errno;
//  }
//  res = pread(fd, buf, size, offset);
//  if (res == -1)
//  {
//    res = -errno;
//  }
//  if(fi == NULL)
//  {
//    close(fd);
//  }
//  return res;
//}
//
//static int durian_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
//{
//  int fd;
//  int res;
//  (void) fi;
//  if(fi == NULL)
//  {
//    fd = open(path, O_WRONLY);
//  }
//  else
//  {
//    fd = fi->fh;
//  }
//  if (fd == -1)
//  {
//    return -errno;
//  }
//  res = pwrite(fd, buf, size, offset);
//  if (res == -1)
//  {
//    res = -errno;
//  }
//  if(fi == NULL)
//  {
//    close(fd);
//  }
//  return res;
//}
//
//static int durian_statfs(const char *path, struct statvfs *stbuf)
//{
//  int res;
//  res = statvfs(path, stbuf);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_release(const char *path, struct fuse_file_info *fi)
//{
//  (void) path;
//  close(fi->fh);
//  return 0;
//}
//
//static int durian_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
//{
//  /* Just a stub.  This method is optional and can safely be left unimplemented */
//  (void) path;
//  (void) isdatasync;
//  (void) fi;
//  return 0;
//}
//
//#ifdef HAVE_POSIX_FALLOCATE
//static int durian_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi)
//{
//  int fd;
//  int res;
//  (void) fi;
//  if (mode)
//  {
//    return -EOPNOTSUPP;
//  if(fi == NULL)
//  {
//    fd = open(path, O_WRONLY);
//  }
//  else
//  {
//    fd = fi->fh;
//  }
//  
//  if (fd == -1)
//  {
//    return -errno;
//  }
//  res = -posix_fallocate(fd, offset, length);
//  if(fi == NULL)
//  {
//    close(fd);
//  }
//  return res;
//}
//#endif
//
//#ifdef HAVE_SETXATTR
///* xattr operations are optional and can safely be left unimplemented */
//static int durian_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
//{
//  int res = lsetxattr(path, name, value, size, flags);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//
//static int durian_getxattr(const char *path, const char *name, char *value, size_t size)
//{
//  int res = lgetxattr(path, name, value, size);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return res;
//}
//
//static int durian_listxattr(const char *path, char *list, size_t size)
//{
//  int res = llistxattr(path, list, size);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return res;
//}
//
//static int durian_removexattr(const char *path, const char *name)
//{
//  int res = lremovexattr(path, name);
//  if (res == -1)
//  {
//    return -errno;
//  }
//  return 0;
//}
//#endif /* HAVE_SETXATTR */
//
//static struct fuse_operations durian_ops = 
//{
//  .init         = durian_init,
//  .getattr      = durian_getattr,
//  .access       = durian_access,
//  .readlink     = durian_readlink,
//  .readdir      = durian_readdir,
//  .mknod        = durian_mknod,
//  .mkdir        = durian_mkdir,
//  .symlink      = durian_symlink,
//  .unlink       = durian_unlink,
//  .rmdir        = durian_rmdir,
//  .rename       = durian_rename,
//  .link         = durian_link,
//  .chmod        = durian_chmod,
//  .chown        = durian_chown,
//  .truncate     = durian_truncate,
//#ifdef HAVE_UTIMENSAT
//  .utimens      = durian_utimens,
//#endif
//  .open         = durian_open,
//  .create       = durian_create,
//  .read         = durian_read,
//  .write        = durian_write,
//  .statfs       = durian_statfs,
//  .release      = durian_release,
//  .fsync        = durian_fsync,
//#ifdef HAVE_POSIX_FALLOCATE
//  .fallocate    = durian_fallocate,
//#endif
//#ifdef HAVE_SETXATTR
//  .setxattr     = durian_setxattr,
//  .getxattr     = durian_getxattr,
//  .listxattr    = durian_listxattr,
//  .removexattr  = durian_removexattr,
//#endif
//};

  int usage(int argc, char *argv[])
  {
    std::cout << argv[0] << " - " << "durian" << std::endl;
    std::cout << "    -d,--debug           debug" << std::endl;
    std::cout << "    -v,--verbose         verbose" << std::endl;
    std::cout << "    -h,--help            print this help message" << std::endl;
    exit(1);
  }

  int main(int argc, char *argv[])
  {
    status_t status;
    bool debug;
    bool verbose;
    while(true)
    {
      int this_option_optind = optind ? optind : 1;
      int option_index = 0;
      static struct option long_options[] = {
        {"debug", 0, 0, 0},
        {"verbose", 0, 0, 0},
        {"help", 0, 0, 0},
        {0, 0, 0, 0}
      };
      int c = getopt_long(argc, argv, "dhv", long_options, &option_index);
      if(-1 == c) break;
      switch(c)
      {
        case 'd':
          debug = true;
          break;
        case 'v':
          verbose = true;
          break;
        case 'h':
          usage(argc, argv);
          break;
      }
    }
    if(argc < 2) usage(argc, argv);

    umask(0);
    //return fuse_main(argc, argv, &durian_ops, NULL);

    return status;
  }

/* *EOF* */
