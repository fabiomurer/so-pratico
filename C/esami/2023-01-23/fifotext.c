/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma fifotext che:
* crei una named pipe (FIFO) al pathname indicato come primo (e unico) argomento.
* apra la named pipe in lettura
* stampi ogni riga di testo ricevuta
* se la named pipe viene chiusa la riapra
* se riceve la riga "FINE" termini cancellando la named pipe.
Esempio:
fifotext /tmp/ff
....
se in un altra shell si fornisce il comando: "echo ciao > /tmp/ff", fifotext stampa ciao e rimane in attesa
(questo esperimento si può provare più volte). Con il comando "echo FINE > /tmp/ff" fifotext termina.
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<signal.h>

int fifofile;
char* fifoname;

void sig_handler(int signo) {
    if (signo == SIGPIPE) {
        printf("reopen pipe\n");
        fifofile = open(fifoname, O_RDONLY);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("args not correct");
        exit(EXIT_FAILURE);
    }

    fifoname = argv[1];
    if (mkfifo(fifoname, 0666) != 0) {
        perror("mkfifo");
    }

    fifofile = open(fifoname, O_RDONLY);

    signal(SIGPIPE, sig_handler);

    char buff[254] = "";
    while (1) {
        int cread = read(fifofile, buff, 254);
        if (cread == -1) {
            perror("read pipe");
            break;
        }
        write(STDOUT_FILENO, buff, cread);
        
        if ((strcmp(buff, "FINE\n") == 0) || (strstr(buff, "\nFINE\n") != NULL)) {
            close(fifofile);
            unlink(fifoname); // or remove()
            exit(EXIT_SUCCESS);
        }
    }
    
    close(fifofile);
}