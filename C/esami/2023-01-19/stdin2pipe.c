
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>

/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere il programma stdin2pipe che prenda in input due righe di testo, ogni riga contiene un
comando coi rispettivi parametri. Il programma deve lanciare entrambi i comandi in modo tale
l'output del primo diventi input del secondo.
Per esempio, dato il file cmds:
ls -l
awk '{print $1}'
l'esecuzione di:
stdin2pipe < cmds
sia equivalente al comando:
ls -l | awk '{print $1}'
*/
#define MAXLINELEN 2048
#define MAXARGLEN  64

int main(void) {
    char* line1 = NULL;
    char* line2 = NULL;
    size_t len1 = 0;
    size_t len2 = 0;
    int nread1 = getline(&line1, &len1, stdin);
    int nread2 = getline(&line2, &len2, stdin);

    char* arg1[MAXARGLEN];
    char* arg2[MAXARGLEN];
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
    }

    if (fork() == 0) {
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

    //free(line1);
    //free(line2);
    exit(EXIT_SUCCESS);
}