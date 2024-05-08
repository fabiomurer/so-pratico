#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigh(int signo) {
	write(1, "SIG\n", 4);
}

int main(int argc, char * argv[]) {
	struct sigaction act = {
		.sa_handler = sigh
	};
	sigaction(SIGUSR1, &act, NULL);
	for (;;) {
		sleep(1);
		printf("waiting...\n");
	}
}
