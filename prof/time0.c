#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	time_t now = time(NULL);
	printf("%ld\n", now);
	printf("%s\n", ctime(&now));
}
