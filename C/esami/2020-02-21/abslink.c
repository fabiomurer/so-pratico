/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti.
Il comando abslink, da implementare, deve sostituire un link simbolico con uno equivalente che sia
un riferimento ad un path assoluto.
Ad esempio: se mylink e' nella directory /home/user e punta a myfile, 'abspath mylink" deve sostituire
mylink con un symbolic link a /home/user/myfile.
Hint: considerate l'uso della funzione realpath.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char* symlinkname = argv[1];

    char* abspath = realpath(symlinkname, NULL);
    printf("%s\n", abspath);
    remove(symlinkname);
    
    if (symlink(abspath, symlinkname) == -1)
        perror("symlink");

    free(abspath);
}
