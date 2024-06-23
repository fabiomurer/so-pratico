#include <linux/prctl.h>  /* Definition of PR_* constants */
#include <sys/prctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


#include <sys/mman.h> // for shared memory created
#include <sys/stat.h> // for mode constants
#include <fcntl.h> // for O_* constant


int main(void) {
    int pidf, pidn;
    int pidp = getpid();

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "%s", "The call to pipe() has failed.\n");           
        exit(EXIT_FAILURE);
    }


    if ((pidf = fork()) == 0) {
        if ((pidn = fork()) == 0) {
            // nipote
            if (prctl(PR_SET_CHILD_SUBREAPER, pidp) == -1)
                perror("prctl1");
            close(pipefd[0]);
            printf("(nipote) sono vivo!\n");
            sleep(1);
            printf("%d\n", getpid());
            int mypid = getpid();
            write(pipefd[1], &mypid, sizeof(pidn));
            printf("(nipote) muoio!\n");
        } else {
            // figlio
            printf("(figlio) sono vivo!\n");
            printf("(figlio) muoio!\n");
        }
    } else {
        // padre
        printf("(padre) sono vivo!\n");
        int status;
        int pid;
        close(pipefd[1]);

        read(pipefd[0], &pid, sizeof(pid));
        printf("%d\n", pid);
        waitpid(pid, &status, 0);
        
        printf("(padre) muoio!\n");
    }
    exit(EXIT_SUCCESS);
}