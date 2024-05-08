#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigh(int signo) {
	write(1, "SIG\n", 4);
}

int main(int argc, char * argv[]) {
	signal(SIGUSR1, sigh);
	for (;;) {
		sleep(1);
		printf("waiting...\n");
	}
}
