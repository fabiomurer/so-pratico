/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma argsend che converta i parametri del programma (da argv[1] in poi) in una
unica sequenza di caratteri: vengono concatenati i parametri (compreso il terminatore della stringa).
Esempio di funzionamento:
$ ./argsend ls -l /tmp | od -c
0000000 l s \0 - l \0 / t m p \0
0000013
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        int len = strlen(argv[i]); // last is '\0'
        printf("%s", argv[i]);
        putchar('\0');
    }
    return 0;
}