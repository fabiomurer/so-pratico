
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    char* f = argv[1];
    struct stat sb;

    if (stat(f, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    switch (sb.st_mode & S_IXUSR) {
    case S_IXUSR:
        printf("is executable\n");
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    default:
        printf("not executable\n");
    }

    exit(EXIT_SUCCESS);
}