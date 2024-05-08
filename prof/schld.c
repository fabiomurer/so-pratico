#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sigh(int signo) {
	int status;
	wait(&status);
	write(1, "term\n", 5);
}

int main(int argc, char * argv[]) {
	signal(SIGCHLD, sigh);
	switch (fork()) {
		case 0:
			sleep(10);
			exit(42);
			break;
		default:
			break;
		case -1:
			perror("fork");
	}
	for (;;) {
		sleep(1);
		printf("waiting...\n");
	}
}
