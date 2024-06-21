# heheh

### create named pipe and read from it

```c
#include <fcntl.h>
#include <sys/stat.h>

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
char* args[] = {"ls", "-l", NULL};
execvp("ls", args);
execlp("ls", "ls", "-l", NULL);
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

### catch signals

```c
// reopen a fifo when closed
void sig_handler(int signo) {
    if (signo == SIGPIPE) {
        printf("reopen pipe\n");
        fifofile = open(fifoname, O_RDONLY);
    }
}
signal(SIGPIPE, sig_handler);
```

### exec `ls -l | awk '{print $1}'` with pipe and stdin/stdout replacement dup2

```c

// Data can be written to the file descriptor fildes[1] and read from the file descriptor fildes[0]
int pipefd[2];
if (pipe(pipefd) == -1) {
    perror("pipe");
}
if (fork() == 0) { // child
    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        perror("dup1");
    close(pipefd[1]);
    
    if (execlp("ls", "ls", "-l", NULL) == -1)
        perror("exec1");
} 
else {
    close(pipefd[1]);
    
    if (dup2(pipefd[0], STDIN_FILENO) == -1)
        perror("dup2");
    close(pipefd[0]);    
    
    if (execlp("awk", "awk", "{print $1}", NULL) == -1)
        perror("exec2");
}
```

### get time delta

```c
// mesure execution time of child
time_t start = time(NULL);
if (start == -1)
    perror("time");
        
wait(&exitstatus);

time_t finish = time(NULL);
if (finish == -1)
    perror("time");
            
double delta = difftime(finish, start);
            
if (delta < 1.0) { // if is less tha 1 sec 
    break;
}
```

### check if child exited normally

```c
int exitstatus;
wait(&exitstatus);
if (WIFEXITED(exitstatus)) {
    printf("exited normally");
}
```

### dlopen

```c
/*
execute function main from file *.so
*/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>
#include <unistd.h>

void *handle;
int (*fmain)(int, char**);
char* error;

handle = dlopen(globalpath, RTLD_LAZY); // globalpath es. ./ciao.so
if (!handle) {
	fprintf(stderr, "%s\n", dlerror());
	exit(EXIT_FAILURE);
}
    
dlerror();

fmain = (void (*)(int, char**)) dlsym(handle, "main");
    
error = dlerror();
if (error != NULL) {
	fprintf(stderr, "%s\n", error);
	exit(EXIT_FAILURE);
}
	
(*fmain)(argc-1, &argv[1]);
	
dlclose(handle);
```

### poll

```c
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

struct pollfd pfd[1] = {
	{notifyfd, POLLIN, 0}
};

int prv = poll(pfd, 1, -1); // timeout -1 aspetta per sempre
if (prv < 0)
	return(1); // timed out
if (pfd[0].revents & POLLIN) { // something to read
    len = read(notifyfd, buf, sizeof(buf));
}
```

### inotify

```c
int notifyfd = inotify_init();
int watch;
if ((watch =inotify_add_watch(notifyfd, dir, IN_MOVED_TO | IN_CREATE)) == -1) {
    perror("inotify add");
    exit(EXIT_FAILURE);
}
char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
const struct inotify_event *event;
ssize_t len;
struct pollfd pfd[1] = {
	{notifyfd, POLLIN, 0}
};

while(1) {
    poll(pfd, 1, -1);
    len = read(notifyfd, buf, sizeof(buf));
    
    for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
        event = (const struct inotify_event *) ptr;
        char filename[1024] = "";
        snprintf(filename, 1024, "%s/%s", dir, event->name);
        if (event->mask & (IN_MOVED_TO | IN_CREATE)) {
            // do stuff
        }
    }
}

inotify_rm_watch(notifyfd, watch);
close(notifyfd);
```

### stat

```c
//check if file is executable
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

char* f = argv[1];
struct stat sb;

if (stat(f, &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
}

switch (sb.st_mode & S_IXUSR) {
    case S_IXUSR:
        printf("is executable\n");
    default:
        printf("not executable\n");
}

exit(EXIT_SUCCESS);
```

### execs

```c
//The following program demonstrates the use of execs:

#include <stdio.h>
#include <unistd.h>
#include <execs.h>
#define BUFLEN 1024

int main(int argc, char *argv)
{
     char buf[BUFLEN];
     printf("type in a command and its arguments, e.g. 'ls -l'\n");
     if (fgets(buf, BUFLEN, stdin) != NULL) {
          execsp(buf);
          printf("exec error\n");
     }
}
```

### s2argv

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <execs.h>
#define BUFLEN 1024

int main(int argc, char *argv)
{
     char buf[BUFLEN];
     printf("type in a command and its arguments, e.g. 'ls -l'\n");
     if (fgets(buf, BUFLEN, stdin) != NULL) {
          char **argv=s2argv(buf);
          execvp(argv[0], argv);
          s2argv_free(argv);
          printf("exec error\n");
     }
}

```