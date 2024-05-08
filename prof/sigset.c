#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

void printsigset(sigset_t *s) {
  printf("Sigset: ");
  for (int i = 1; i < 32; i++) {
    if (sigismember(s, i))
      printf("%d ", i);
  }
  printf("\n");
}

void printcurmask(void) {
	sigset_t curmask;
	sigprocmask(SIG_SETMASK, NULL, &curmask);
  printsigset(&curmask);
}

void sigh(int signo) {
	write(1, "SIG!\n", 5);
}

int main(int argc, char *argv[]) {
	struct sigaction act = {
		.sa_handler = sigh
	};
	printf("pid=%d\n", getpid());
	sigaction(SIGUSR1, &act, NULL);
  sigset_t oldset;
  sigset_t newmask;
	printcurmask();

  sigemptyset(&newmask);
  sigaddset(&newmask, SIGUSR1);
  sigaddset(&newmask, SIGUSR2);
  sigprocmask(SIG_BLOCK, &newmask, &oldset);
	printcurmask();

	for (int i = 0; i < 10; i++) {
		sleep(1);
		sigset_t pending;
		sigpending(&pending);
		printf("P ");
		printsigset(&pending);
	}
	sigprocmask(SIG_SETMASK, &oldset, NULL);
 	sleep(2);
}

