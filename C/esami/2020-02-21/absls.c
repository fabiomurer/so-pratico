/*
Esercizio 2: Linguaggio C: 10 punti
Scrivere il comando absls che mostri per ogni file della directory passata come parametro il path
completo di ogni file (mostrando al posto dei link simbolici il path completo dei file puntati).
*/


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char* dirname = argv[1];

    DIR *dir; // dir streams
    struct dirent *entry;
    dir = opendir(dirname);
    if (dir == NULL) return -1;

    while((entry = readdir(dir)) != NULL) {
        char* abspath = realpath(entry->d_name, NULL);
        printf("%s\n", abspath);
        free(abspath);
    }
    closedir(dir);
    return 0;
}