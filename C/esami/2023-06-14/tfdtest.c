#include <bits/types/struct_itimerspec.h>
#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Facendo uso dei timerfd (vedi timerfd_create) scrivere un programma che stampi una stringa a
intervalli regolari. (il parametro ha tre campi separati da virgola: il numero di iterazioni, l'intervallo fra
iterazione e la stringa da salvare:
tfdtest 4,1.1,ciao
deve stampare ciao quattro volte, rispettivamente dopo 1.1 secondi, 2.2 secondi, 3.3 secondi 4.4
secondi e terminare. L'esecuzione dovrebbe essere simile alla seguente:
$ tfdtest 4,1.1,ciao
1.100267 ciao
2.200423 ciao
3.300143 ciao
4.400053 ciao
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

int main(int argc, char *argv[]) {
    int fd;
    ssize_t s;
    uint64_t exp, tot_exp, max_exp;
    struct itimerspec new_value;

    int times;
    float delta;
    char string[254];

    parseargs(argv[1], &times, &delta, string);

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