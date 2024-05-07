/*
Scrivere un programma search_name che deve cercare nel sottoalbero della directory corrente tutti i
file eseguibili con un nome file specifico passato come primo e unico parametro indicando per ogni
file il tipo di eseguibile (script o eseguibile binario).
Ad esempio il comando:
./search_name testprog
deve cercare i file eseguibili chiamati testprog nell'albero della directory corrente. Poniamo
siano . /testprog, ./dir1/testprog, ./dir/dir3/testprog, search_name deve stampare:
. /testprog: script
./dir1/testprog: ELF executable
./dir/dir3/testprog: ELF executable
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>


typedef enum filetypes {
    elf,
    script,
    notexec
} filetype;

bool iself(char path[]) {
    char buffer[4];
    char elf[] = {0x7f, 'E', 'L', 'F'};
    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);  // Close the file before returning on error
        exit(EXIT_FAILURE);
    }
     // Close the file
    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    if (memcmp(elf, buffer, sizeof(elf)) == 0) return true;
    else return false;
}

filetype filecheck(char path[]) {
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    switch (sb.st_mode & S_IXUSR) {
    case S_IXUSR:
        if (iself(path)) return elf;
        else return script;
        break;
    default:
        return notexec;
  }
}

void listdir(const char *name, char* name1)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            listdir(path, name1);
        } 
        else if ((entry->d_type == DT_REG) && (strcmp(entry->d_name, name1) == 0)){
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", name, entry->d_name);
            filetype ft = filecheck(filepath);

            switch (ft) {
                case elf:
                    printf("%s: ELF executable", filepath);
                    break;
                case script:
                    printf("%s: script", filepath);
                    break;
                default:
                    break;
            }
            printf("\n");
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: %s <name>", argv[0]);
        exit(EXIT_FAILURE);
    }
    listdir(".", argv[1]);
    exit(EXIT_SUCCESS);
}