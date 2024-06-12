/*
Scrivere un secondo programma argrecv che preso in input l'output di argsend esegua il comando
con gli argomenti passati a argsend. Per esempio:
$ ./argsend ls -l /tmp | ./argrecv
total 8988
-rw-r--r-- 1 renzo renzo 150532 Jan 9 16:57 ....
.....
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

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

    char buff[200] = "";
    scanf("%s", buff);
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
    perror("execve");   /* execve() returns only on error */
    exit(EXIT_FAILURE);
}