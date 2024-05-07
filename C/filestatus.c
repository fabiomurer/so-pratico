/*
 Two useful functions exist to inquire about the files current status. You can
find out how large the file is (st_size) when it was created (st_ctime) etc.
(see stat structure definition below. The two functions are prototyped in
<sys/stat.h>

int stat(char *path, struct stat *buf),
int fstat(int fd, struct
stat *buf)


stat() obtains information about the file named by path. Read, write or execute
permission of the named file is not required, but all directories listed in the
path name leading to the file must be searchable.

fstat() obtains the same information about an open file referenced by the
argument descriptor, such as would be obtained by an open call (Low level I/O).

stat(), and fstat() return 0 on success, -1 on failure and sets errno to
indicate the error. Errors are again defined in #include <sys/stat.h>

buf is a pointer to a stat structure into which information is placed concerning
the file. A stat structure is define in #include <sys/types.h>, as follows

struct stat {
          mode_t   st_mode;      File mode (type, perms)
          ino_t    st_ino;       Inode number
          dev_t    st_dev;       ID of device containing
                                 a directory entry for this file
          dev_t    st_rdev;      ID of device
                                 This entry is defined only for
                                 char special or block special files
          nlink_t  st_nlink;     Number of links
          uid_t    st_uid;       User ID of the file's owner
          gid_t    st_gid;       Group ID of the file's group
          off_t    st_size;      File size in bytes
          time_t   st_atime;     Time of last access
          time_t   st_mtime;     Time of last data modification
          time_t   st_ctime;     Time of last file status change
                                 Times measured in seconds since
                                 00:00:00 UTC, Jan. 1, 1970
          long     st_blksize;   Preferred I/O block size
          blkcnt_t st_blocks;    Number of 512 byte blocks allocated
}

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[]) {
  struct stat sb;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  printf("File type:                ");

  switch (sb.st_mode & S_IFMT) {
  case S_IFBLK:
    printf("block device\n");
    break;
  case S_IFCHR:
    printf("character device\n");
    break;
  case S_IFDIR:
    printf("directory\n");
    break;
  case S_IFIFO:
    printf("FIFO/pipe\n");
    break;
  case S_IFLNK:
    printf("symlink\n");
    break;
  case S_IFREG:
    printf("regular file\n");
    break;
  case S_IFSOCK:
    printf("socket\n");
    break;
  default:
    printf("unknown?\n");
    break;
  }

  printf("I-node number:            %ld\n", (long)sb.st_ino);

  printf("Mode:                     %lo (octal)\n", (unsigned long)sb.st_mode);

  printf("Link count:               %ld\n", (long)sb.st_nlink);
  printf("Ownership:                UID=%ld   GID=%ld\n", (long)sb.st_uid,
         (long)sb.st_gid);

  printf("Preferred I/O block size: %ld bytes\n", (long)sb.st_blksize);
  printf("File size:                %lld bytes\n", (long long)sb.st_size);
  printf("Blocks allocated:         %lld\n", (long long)sb.st_blocks);

  printf("Last status change:       %s", ctime(&sb.st_ctime));
  printf("Last file access:         %s", ctime(&sb.st_atime));
  printf("Last file modification:   %s", ctime(&sb.st_mtime));


  switch (sb.st_mode & S_IXUSR) {
    case S_IXUSR:
        printf("is executable\n");
        break;
    default:
        printf("not executable\n");
  }
  exit(EXIT_SUCCESS);
}