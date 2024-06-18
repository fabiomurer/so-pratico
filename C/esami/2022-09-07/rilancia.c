/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma C rilancia: che esegua il programma indicato in argv[1] con i relativi parametri
(in argv[2] e seguenti):
es: rilancia tr a-z A-Z
esegue il comando tr coi parametri.
Se il programma lanciato termina senza errori (non per colpa di un segnale e con valore di ritorno 0)
rilancia deve rieseguire il programma (nell'esempio tr) con i medesimi parametri.
*/

#include <bits/types/time_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>

void rilacia1(int argc, char* argv[]) {
    while (1) {
        if (fork() == 0) {
            execvp(argv[1], &argv[1]);
        } else {
            int exitstatus;
            wait(&exitstatus);
            if (!WIFEXITED(exitstatus)) { // relaunch
                break;
            }
        }
    }
}

/*
Esercizio 2: Linguaggio C: 10 punti
Se si esegue il programa dell'esercizio 1 in questo modo:
rilancia cat /etc/hostname
il comando (cat) viene eseguito ripetutamente all'infinito.
Modificare il programma rilancia per fare in modo che se l'esecuzione del programma (in questo caso
cat /etc/hostname) dura meno di un secondo non si proceda alla riattivazione.
*/
#include <time.h>
void rilacia2(int argc, char* argv[]) {
    while (1) {
        if (fork() == 0) {
            execvp(argv[1], &argv[1]);
        } else {
            int exitstatus;

            time_t start = time(NULL);
            if (start == -1)
                perror("time");
        
            wait(&exitstatus);

            time_t finish = time(NULL);
            if (finish == -1)
                perror("time");
            
            double delta = difftime(finish, start);
            
            if (delta < 1.0) {
                break;
            }

            if (!WIFEXITED(exitstatus)) { // relaunch
                break;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    
    //rilacia1(argc, argv);
    rilacia2(argc, argv);
    exit(EXIT_SUCCESS);
}