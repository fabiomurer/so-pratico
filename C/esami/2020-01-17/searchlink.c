#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Scrivere un programma searchlink che dati due parametri (nell'ordine un file f ed una directory d)
metta in output l'elenco dei path all'interno dell'albero che ha radice in d che fanno riferimento ad f
o come link fisico o come link simbolico.
es:
searchlink myfile mydir
link a
link d/b
symlink e/s
significa che dir/a e dir/d/b si riferisono a f come link fisici mentre dir/e/s e' un link simbolico che punta
ad f.
*/

void listdir(const char *name, char* match)
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
            listdir(path, match);
        } 
        else if (entry->d_type == DT_LNK) {
            char* rpath = realpath(entry->d_name, NULL);
            if (rpath == NULL) perror("realpath");
            if (strcmp(rpath, match) == 0) {
                printf("symlink %s\n", entry->d_name);
            }
            free(rpath);
        } else {
            char* rpath = realpath(entry->d_name, NULL);
            if (rpath == NULL) perror("realpath");
            if (strcmp(rpath, match) == 0) {
                printf("link %s\n", entry->d_name);
            }
            free(rpath);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    char* file = argv[1];
    char* dir  = argv[2];

    char* file_realpath = realpath(file, NULL);
    
    listdir(dir, file_realpath);
}