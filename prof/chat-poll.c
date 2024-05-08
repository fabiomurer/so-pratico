#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#define BUFSIZE 1024

/*
 * POLLOUT Writing is now possible, though a write larger than the
 * 		available space in a socket or pipe will still block
 * POLLIN There is data to read.
*/

/*
RETURN VALUE         top

       On success, poll() returns a nonnegative value which is the
       number of elements in the pollfds whose revents fields have been
       set to a nonzero value (indicating an event or an error).  A
       return value of zero indicates that the system call timed out
       before any file descriptors became ready.

       On error, -1 is returned, and errno is set to indicate the error.

poll() performs a similar task to select(2): it waits for one of
       a set of file descriptors to become ready to perform I/O.  The
       Linux-specific epoll(7) API performs a similar task, but offers
       features beyond those found in poll().

       The set of file descriptors to be monitored is specified in the
       fds argument, which is an array of structures of the following
       form:

           struct pollfd {
               int   fd;          file descriptor 
               short events;      requested events 
               short revents;     returned events 
           };

       The caller should specify the number of items in the fds array in
       nfds.

       The field fd contains a file descriptor for an open file.  If
       this field is negative, then the corresponding events field is
       ignored and the revents field returns zero.  (This provides an
       easy way of ignoring a file descriptor for a single poll() call:
       simply set the fd field to its bitwise complement.)

       The field events is an input parameter, a bit mask specifying the
       events the application is interested in for the file descriptor
       fd.  This field may be specified as zero, in which case the only
       events that can be returned in revents are POLLHUP, POLLERR, and
       POLLNVAL (see below).

       The field revents is an output parameter, filled by the kernel
       with the events that actually occurred.  The bits returned in
       revents can include any of those specified in events, or one of
       the values POLLERR, POLLHUP, or POLLNVAL.  (These three bits are
       meaningless in the events field, and will be set in the revents
       field whenever the corresponding condition is true.)

       If none of the events requested (and no error) has occurred for
       any of the file descriptors, then poll() blocks until one of the
       events occurs.

       The timeout argument specifies the number of milliseconds that
       poll() should block waiting for a file descriptor to become
       ready.  The call will block until either:

       •  a file descriptor becomes ready;

       •  the call is interrupted by a signal handler; or

       •  the timeout expires.

       Being "ready" means that the requested operation will not block;
       thus, poll()ing regular files, block devices, and other files
       with no reasonable polling semantic always returns instantly as
       ready to read and write.

       Note that the timeout interval will be rounded up to the system
       clock granularity, and kernel scheduling delays mean that the
       blocking interval may overrun by a small amount.  Specifying a
       negative value in timeout means an infinite timeout.  Specifying
       a timeout of zero causes poll() to return immediately, even if no
       file descriptors are ready.

       The bits that may be set/returned in events and revents are
       defined in <poll.h>:
*/

int main(int argc, char *argv[]) {
	int fdin = open(argv[1], O_RDONLY | O_NONBLOCK);
	int fdout = open(argv[2], O_WRONLY);
	char buf[BUFSIZE];

	struct pollfd pfd[2] = {
		{STDIN_FILENO, POLLIN, 0},
		{fdin, POLLIN, 0}};

	for (;;) {
		int prv = poll(pfd, 2, -1); // timeout -1 aspetta per sempre
		if (prv < 0)
			return(1);
		if (pfd[0].revents & POLLIN) {
			ssize_t n = read(STDIN_FILENO, buf, BUFSIZE);
			if (n <= 0)
				return 0;
			write (fdout, buf, n);
		}
		if (pfd[1].revents & POLLIN) {
			ssize_t n = read(fdin, buf, BUFSIZE);
			if (n <= 0)
				return 0;
			write (STDOUT_FILENO, buf, n);
		}
	}
}
