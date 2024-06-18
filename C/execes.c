#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>


int main(void) {
    char* args[] = {"ls", "-l", NULL};
    execvp("ls", args);
    execlp("ls", "ls", "-l", NULL);
}