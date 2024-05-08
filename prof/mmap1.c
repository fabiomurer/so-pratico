#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
	struct stat st;
	stat(argv[1], &st);
	printf("file %s length %d\n", argv[1], st.st_size);

	int fd = open(argv[1], O_RDONLY);

	char *mmfd = mmap((void *)0x7f0000000000, st.st_size, PROT_READ, MAP_SHARED,
			fd, 0);

	printf("%p\n", mmfd);

	close(fd);

	write(1, mmfd, st.st_size);

	printf("%c %c %c\n", mmfd[0], mmfd[1],mmfd[2]);
	munmap(mmfd, st.st_size);
}
