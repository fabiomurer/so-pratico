/*Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Sia dato questo programma hw.c (vi viene fornito in /public/hw.c)
#include <stdio.h>
int main(int argc, char*argv[]) {
printf("hello world:");
for(argv++, argv--; argc > 0; argv++, argc--)
printf(" %s",*argv);
printf("\n");
return 0;
}
Il programma hw.c può essere compilato come libreria dinamica:
gcc --shared -o hw.so hw.c
La libreria dinamica non è un eseguibile
$ ./hw.so 1 2 3 4
Segmentation fault
ma può essere caricata a tempo di esecuzione tramite dlopen. Scrivere un programma "lancia" in
grado di eseguire il codice di hw.so
$ ./lancia hw.so 1 2 3 4
hello world: hw.so 1 2 3 4
(suggerimenti: dlopen non cerca nella directory corrente, occorre passare il path assoluto della libreria.
"main" in hw.so è una normale funzione: occorre cercare l'indirizzo della funzione main nella libreria
ed invocarla,)*/

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	void *handle;
	int (*fmain)(int, char**);
	char* error;
	
	char globalpath[1024] = "";
	snprintf(globalpath, 1024, "./%s", argv[1]);
	
	handle = dlopen(globalpath, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "./%s\n", dlerror());
		exit(EXIT_FAILURE);
    }
    
    dlerror();
    
    fmain = (void (*)(int, char**)) dlsym(handle, "main");
    
    error = dlerror();
    if (error != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
	
	(*fmain)(argc-1, &argv[1]);
	
	dlclose(handle);
	
	
	exit(EXIT_SUCCESS);
}