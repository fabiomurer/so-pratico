#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigh(int signo) {
	write(1, "PRR\n", 4);
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigh);
	signal(SIGTERM, sigh);
	for (;;) {
		sleep(1);
		printf("waiting...\n");
	}
}
