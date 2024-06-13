#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere la funzione:
char **vreaddir(const char *path)
che restituisca l'elenco dei nomi dei file in una directory come vettore di stringhe terminato con un
puntatore NULL. (lo stesso formato di argv o envp).
Il vettore e le stringhe dei nomi sono allocate dinamicamente.
completare l'esercizio con un programma principale che testi il corretto funzionamento della funzione
vreaddir.
*/

int file_select(const struct direct *entry) {
  if (entry->d_type == DT_REG)
    return true;
  else
    return false;
}

char** vreaddir(const char *path) {
    int count, i;
    struct dirent **files;
    count = scandir(path, &files, file_select, alphasort);
  
    char** retdata = malloc(sizeof(char*) * count);
    retdata[count] = NULL;
    
    for (i = 0; i < count; ++i) {
        retdata[i] = malloc(sizeof(char) * (strlen(files[i]->d_name) + 1));
        strcpy(retdata[i], files[i]->d_name);
        free(files[i]);
    }
    free(files);
    return retdata;
}

/*
Esercizio 2: Linguaggio C: 10 punti
Rielaborare l'esercizio precedente per fare in modo che il vettore e le stringhe dei nomi siano allocati
con una unica operazione malloc (in modo da poter liberare lo spazio usato da vreaddir con una unica
operazione free.)
*/

int maxnamelen(struct dirent **files, int count) {
    int ml = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(files[i]->d_name) + 1;
        if (len > ml) ml = len;
    }
    return ml;
}

char** vreaddir2(const char *path) {
    int count, i;
    struct dirent **files;
    count = scandir(path, &files, file_select, alphasort);
    
    int ml = maxnamelen(files, count);
    
    char** retdata = malloc(sizeof(char*) * count);
    retdata[count] = NULL;
    
    for (i = 0; i < count; ++i) {
        retdata[i] = malloc(sizeof(char) * (strlen(files[i]->d_name) + 1));
        strcpy(retdata[i], files[i]->d_name);
        free(files[i]);
    }
    free(files);
    return retdata;
}

int main(int argc, char* argv[]) {
    char** lil = vreaddir(".");
    int i = 0;
    while (lil[i] != NULL) {
        printf("%s\n", lil[i]);
        i++;
    }
}