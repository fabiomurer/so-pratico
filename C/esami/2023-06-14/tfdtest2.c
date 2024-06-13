#include <bits/types/struct_itimerspec.h>
#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
/*
Estendere l'esercizio 1 in modo che gestisca molteplici timer:
$ mftdtest 2,1.1,a 2,1.2,b 3,0.5,c
0.500255 c
1.000232 c
1.100231 a
1.200266 b
1.500245 c
2.200212 a
2.400255 b
*/


void parseargs(char* arg, int* times, float* delta, char* string) {
    char* token;
    token = strtok(arg, ",");
    *times = atoi(token);
    token = strtok(NULL, ",");
    *delta = atof(token);
    token = strtok(NULL, ",");
    strcpy(string, token);
}

void tfdtest(char* arg) {
    int fd;
    ssize_t s;
    uint64_t exp, tot_exp, max_exp;
    struct timespec now;
    struct itimerspec new_value;

    int times;
    float delta;
    char string[254];

    parseargs(arg, &times, &delta, string);

    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        err(EXIT_FAILURE, "clock_gettime");


    // first tick
    new_value.it_value.tv_sec = (long)delta;
    new_value.it_value.tv_nsec = (long)((delta - (float)((int)delta)) * 1e9);
    
    // all others
    new_value.it_interval.tv_sec = (long)delta;
    new_value.it_interval.tv_nsec = (long)((delta - (float)((int)delta)) * 1e9);
    max_exp = times;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        err(EXIT_FAILURE, "timerfd_create");

    if (timerfd_settime(fd, CLOCK_REALTIME, &new_value, NULL) == -1)
        err(EXIT_FAILURE, "timerfd_settime");


    for (tot_exp = 0; tot_exp < max_exp;) {
        s = read(fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            err(EXIT_FAILURE, "read");
//
        tot_exp += exp;
        struct itimerspec elapsed;
        timerfd_gettime(fd, &elapsed);
        float timepassed = delta * tot_exp + (
            delta - (float)elapsed.it_value.tv_sec - 
            (float)(elapsed.it_value.tv_nsec / 1e9)
        );
        printf("%f: %s\n", timepassed, string);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    
    int i = 1;
    while (argv[i] != NULL) {
        pid_t fork_retvalue;
	    if (fork_retvalue = fork()) {
            //genitore
            tfdtest(argv[i]);
            int status;
            wait(&status); // aspetta che finosca il figlio
            exit(EXIT_SUCCESS);
        }
	    else {
            i++; // use next argv, next iteration is father
        }
    }
}