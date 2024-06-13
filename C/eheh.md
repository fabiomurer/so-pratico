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

### working with fdtimer

```c
fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        err(EXIT_FAILURE, "timerfd_create");

    if (timerfd_settime(fd, CLOCK_REALTIME, &new_value, NULL) == -1)
        err(EXIT_FAILURE, "timerfd_settime");


    for (tot_exp = 0; tot_exp < max_exp;) {
        s = read(fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            err(EXIT_FAILURE, "read");

        tot_exp += exp;
        struct itimerspec elapsed;
        timerfd_gettime(fd, &elapsed);
        float timepassed = delta * tot_exp + (
            delta - (float)elapsed.it_value.tv_sec - 
            (float)(elapsed.it_value.tv_nsec / 1e9)
        );
        printf("%f: %s\n", timepassed, string);
    }
```

### some wild forks

```c
/*
spawn a process that execute job(argv[i]) concurrent
es. argv = {1, 2, 3, 4, null} job=iseven t1 iseven(1) t2 iseven(2) ...
*/
int i = 1;
while (argv[i] != NULL) {
    pid_t fork_retvalue;
    if (fork_retvalue = fork()) {
        //genitore
        job(argv[i]);
        int status;
        wait(&status); // aspetta che finosca il figlio
        exit(EXIT_SUCCESS);
    }
    else {
        i++; // use next argv, next iteration is father
    }
}
```