#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
SIGSUR1 e 2 possono essere riprogrammati
per usarlo es. kill -s 10(che è SIGSUR1) 6014
*/

void sigh(int signo) {
	write(1, "SIG\n", 4);
}

int main(int argc, char * argv[]) {
	printf("my pid is: %d", getpid());
	signal(SIGUSR1, sigh);
	for (;;) {
		sleep(1);
		printf("waiting...\n");
	}
}
