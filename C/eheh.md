# heheh

### create named pipe and read from it

```c
char* fifoname = argv[1];
if (mkfifo(fifoname, 0666) != 0) {
    perror("mkfifo");
}

int fifofile = open(fifoname, O_RDONLY);

char buff[200] = "";
//scanf("%s", buff);
read(fifofile, buff, 200);
```

### exec like shell

```c
// execvp search file like shell
execvp(args[0], &args[1]); //char* argv[]={"Test", "Test_1", NULL};
perror("execve");   /* execve() returns only on error */
exit(EXIT_FAILURE);
```

### print null char ?.?

```c
putchar('\0');
```

### list content in dir

```c
/*basic*/
int main(void) {
    char name[MAXNAMLEN];
    getcwd(name, MAXNAMLEN);

    DIR *dir; // dir streams
    struct dirent *entry;
    dir = opendir(name);
    if (dir == NULL) return -1;

    while((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}

/*morefeautres*/

int file_select(const struct direct *entry) {
  if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
    return (TRUE);
}

int main() {
  int count, i;
  struct dirent **files;

  count = scandir(pathname, &files, file_select, alphasort);
  for (i = 0; i < count; ++i) {
    printf("%s  ", files[i]->d_name);
    free(files[i]);
  }
  free(files);

  printf("\n"); /* flush buffer */
}

```