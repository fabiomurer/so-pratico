/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti.
Usando il metodo inotify implementare il programma dircat.
dircat ha 2 parametri: il pathname di una directory (vuota) D e di un file F.
Ogni volta che un file viene messo in D il programma dircat aggiunge a F una
riga di testata contenente il nome del file e ne copia il contenuto in F. Finita
la copia il file che era stato messo in D viene cancellato (la directory D torna
vuota).
*/

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 4096

/*
Esercizio 1: Linguaggio C (obbligatorio) 20 punti.
Usando il metodo inotify implementare il programma dircat.
dircat ha 2 parametri: il pathname di una directory (vuota) D e di un file F.
Ogni volta che un file viene messo in D il programma dircat aggiunge a F una riga di testata
contenente il nome del file e ne copia il contenuto in F. Finita la copia il file che era stato messo in D
viene cancellato (la directory D torna vuota).
(per fare gli esperimenti si consiglia di preparare i file di prova in un'altra directory e copiarli in D).
*/

int main (int argc, char* argv[]) {
    char* dir = argv[1];
    char* file = argv[2];

    int notifyfd = inotify_init();
    int watch;
    if ((watch =inotify_add_watch(notifyfd, dir, IN_MOVED_TO | IN_CREATE)) == -1) {
        perror("inotify add");
        exit(EXIT_FAILURE);
    }

    char buf[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event *event;
    ssize_t len;

    struct pollfd pfd[1] = {
		{notifyfd, POLLIN, 0}
    };

    while(1) {
        poll(pfd, 1, -1);

        len = read(notifyfd, buf, sizeof(buf));
        
        for (char *ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
            event = (const struct inotify_event *) ptr;

            char filename[1024] = "";
            snprintf(filename, 1024, "%s/%s", dir, event->name);

            if (event->mask & (IN_MOVED_TO | IN_CREATE)) {
                FILE* outf = fopen(file, "a");
                if (outf == NULL)
                    perror("fopen");

                fprintf(outf, "==START FILE:%s==\n", filename);
                int fdin = open(event->name, O_RDONLY);
                if (fdin == -1)
                    perror("open");

                char buf[BUFSIZE];
	            ssize_t n;
	            while ((n = read(fdin, buf, BUFSIZE)) > 0) {
                    fwrite(buf, sizeof(char), n, outf);
                }
                fprintf(outf, "\n==END==\n");
                fclose(outf);
                close(fdin);
                if (unlink(filename) == -1)
                    perror("ulink");
            }
        }
    }

    inotify_rm_watch(notifyfd, watch);
    close(notifyfd);
    exit(EXIT_SUCCESS);
}