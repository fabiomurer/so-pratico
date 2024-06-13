/*
Esercizio 2: Linguaggio C: 10 punti
Scrivere un programma pargrcv che crei una named pipe (il pathname è passato come parametro) e
quando si ridireziona nella named pipe la sequenza di caratteri creata da argsend dell'esercizio 1,
pargrcv deve eseguire il comando.
$ ./pargrcv /tmp/mypipe
crea la named pipe /tmp/mypipe e si mette in attesa.
Da un'altro terminale il comando:
$ ./argsend ls -l /tmp > /tmp/mypipe
fa eseguire il comando "ls -l /" a pargrcv
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_words(char str[]) {
    int i = 0;
    bool flag = false;
    bool subsquent = false;
    int count = 0;
    while (!subsquent) {
        if (str[i] == '\0' && flag) subsquent = true;
        else if (str[i] == '\0') {
            count++;
            flag = true;
        } else {
            flag = false;
        }
        i++;
    }
    return count;
}

char* nextword(char str[]) {
    int i = 0;
    while (str[i] != '\0') i++;
    i++;
    return &str[i];
}

int main(int argc, char* argv[]) {
    char* fifoname = argv[1];
    if (mkfifo(fifoname, 0666) != 0) {
        perror("mkfifo");
    }
    
    int fifofile = open(fifoname, O_RDONLY);
    
    char buff[200] = "";
    //scanf("%s", buff);
    read(fifofile, buff, 200);
    
    //----------------- parte non importante
    int words = count_words(buff);

    char** args = malloc(sizeof(char*) * (words+1));

    char* word = buff;
    for (int i = 0; i < words; i++) {
        args[i] = malloc(sizeof(char) * (strlen(word) + 1));
        strcpy(args[i], word);
        word = nextword(word);
    }

    // execvp search file like shell
    execvp(args[0], &args[1]); //char* argv[]={"Test", "Test_1", NULL};
    perror("execve");   // execve() returns only on error 
    exit(EXIT_FAILURE);
}