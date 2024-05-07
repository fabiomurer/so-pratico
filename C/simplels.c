/*
Example - a simple C version of UNIX ls utility
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>

#define FALSE 0
#define TRUE !FALSE

extern int
alphasort(); /*alphasort(struct direct **d1, **d2) -- alphasort() is a built in
                routine which will sort the array alphabetically. */

char pathname[MAXPATHLEN];

/*
scandir(char *dirname, struct direct **namelist, int (*select)(),
int (*compar)()) -- reads the directory dirname and builds an array of pointers to directory entries or -1 for an error. 
namelist is a pointer to an array of structure pointers.

(*select))() is a pointer to a function which is called with a pointer to a directory entry 
(defined in <sys/types> and should return a non zero value if the directory entry should be
 included in the array. If this pointer is NULL, then all the directory entries will be included.

The last argument is a pointer to a routine which is passed to qsort (see man qsort) -- 
a built in function which sorts the completed array. If this pointer is NULL, the array is not sorted.

alphasort(struct direct **d1, **d2) -- alphasort() is a built in routine 
which will sort the array alphabetically. 
*/

/*
#ifdef __USE_MISC
 File types for `d_type'.
enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4, directory
# define DT_DIR		DT_DIR  
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8, regular file
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };
*/

int file_select(const struct direct *entry) {
  if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
    return (TRUE);
}

int main() {
  int count, i;
  struct dirent **files;

  if (getcwd(pathname, MAXPATHLEN) == NULL) {
    printf("Error getting path$\n");
    exit(0);
  }
  printf("Current Working Directory = %s\n", pathname);
  count = scandir(pathname, &files, file_select, alphasort);

  /* If no files found, make a non-selectable menu item */
  if (count <= 0) {
    printf("No files in this directory\n");
    exit(0);
  }
  printf("Number of files = %d\n", count);
  for (i = 0; i < count; ++i) {
    printf("%s  ", files[i]->d_name);
    free(files[i]);
  }
  free(files);

  printf("\n"); /* flush buffer */
}
