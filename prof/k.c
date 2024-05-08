#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv[]) {
	pid_t pid = strtol(argv[1], NULL, 10);
	int signo = strtol(argv[2], NULL, 10);
	kill(pid, signo);
}
