#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti.
scrivere un programma para per lanciare parallelamente comandi
para sleep 20 // ls -l // ps // sleep 3
deve eseguire concorrentemente i vari comandi separati da // e aspettare la terminazione di tutti
*/

void execall(int num, char* argv[]) {
    int pos = 0;

    if (fork() == 0) {
        for (int i = 0; i < num; i++) {
            if (fork() == 0) {
                printf("%s\n", argv[pos]);
                execvp(argv[pos], &argv[pos]);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else {
                while (argv[pos] != NULL) {
                    pos++;
                }
                pos++;
            }
        }
    }
    int a;
    int wpid;
    while ((wpid = wait(&a)) > 0); // wait all children
}


int main(int argc, char* argv[]) {

    int num = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "//") == 0) {
            num++;
            argv[i] = NULL;
        }  
    }
    num++;

    execall(num, &argv[1]);
    exit(EXIT_SUCCESS);
}