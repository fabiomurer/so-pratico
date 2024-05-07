#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("number of arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("arg n. %d: %s\n", i, argv[i]);
    }
    return 0;
}