#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <dirent.h>
#include <unistd.h>

int main(void) {
    char name[MAXNAMLEN];
    getcwd(name, MAXNAMLEN);

    DIR *dir; // dir streams
    struct dirent *entry;
    dir = opendir(name);
    if (dir == NULL) return -1;

    while((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}