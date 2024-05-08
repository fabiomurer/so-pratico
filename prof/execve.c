#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


/*
exec rimpiazza il processo con il programma specificato

*/


/*
       #include <unistd.h>

       extern char **environ;

       int execl(const char *pathname, const char *arg, ...
                       / *, (char *) NULL * /);
       int execlp(const char *file, const char *arg, ...
                       / *, (char *) NULL  * /);
       int execle(const char *pathname, const char *arg, ...
                       / *, (char *) NULL, char *const envp[] * /);
       int execv(const char *pathname, char *const argv[]);
       int execvp(const char *file, char *const argv[]);
       int execvpe(const char *file, char *const argv[], char *const envp[]);

   Feature Test Macro Requirements for glibc (see
   feature_test_macros(7)):

       execvpe():
           _GNU_SOURCE

DESCRIPTION         top

       **The exec() family of functions replaces the current process image
       with a new process image**.  The functions described in this manual
       page are layered on top of execve(2).  (See the manual page for
       execve(2) for further details about the replacement of the
       current process image.)

       The initial argument for these functions is the name of a file
       that is to be executed.

       The functions can be grouped based on the letters following the
       "exec" prefix.

   l - execl(), execlp(), execle()
       The const char *arg and subsequent ellipses can be thought of as
       arg0, arg1, ..., argn.  Together they describe a list of one or
       more pointers to null-terminated strings that represent the
       argument list available to the executed program.  The first
       argument, by convention, should point to the filename associated
       with the file being executed.  **The list of arguments must be
       terminated by a null pointer**, and, since these are variadic
       functions, this pointer must be cast (char *) NULL.

       By contrast with the 'l' functions, the 'v' functions (below)
       specify the command-line arguments of the executed program as a
       vector.

   v - execv(), execvp(), execvpe()
       The char *const argv[] argument is an array of pointers to null-
       terminated strings that represent the argument list available to
       the new program.  The first argument, by convention, should point
       to the filename associated with the file being executed.  The
       array of pointers must be terminated by a null pointer.

   e - execle(), execvpe()
       The environment of the new process image is specified via the
       argument envp.  The envp argument is an array of pointers to
       null-terminated strings and must be terminated by a null pointer.

       All other exec() functions (which do not include 'e' in the
       suffix) take the environment for the new process image from the
       external variable environ in the calling process.

   p - execlp(), execvp(), execvpe()
       These functions duplicate the actions of the shell in searching
       for an executable file if the specified filename does not contain
       a slash (/) character.  The file is sought in the colon-separated
       list of directory pathnames specified in the PATH environment
       variable.  If this variable isn't defined, the path list defaults
       to a list that includes the directories returned by
       confstr(_CS_PATH) (which typically returns the value
       "/bin:/usr/bin") and possibly also the current working directory;
       see NOTES for further details.

       execvpe() searches for the program using the value of PATH from
       the caller's environment, not from the envp argument.

       If the specified filename includes a slash character, then PATH
       is ignored, and the file at the specified pathname is executed.

       In addition, certain errors are treated specially.

       If permission is denied for a file (the attempted execve(2)
       failed with the error EACCES), these functions will continue
       searching the rest of the search path.  If no other file is
       found, however, they will return with errno set to EACCES.

       If the header of a file isn't recognized (the attempted execve(2)
       failed with the error ENOEXEC), these functions will execute the
       shell (/bin/sh) with the path of the file as its first argument.
       (If this attempt fails, no further searching is done.)

       All other exec() functions (which do not include 'p' in the
       suffix) take as their first argument a (relative or absolute)
       pathname that identifies the program to be executed.

*/
int main() {
	char *newargv[] = {"ls", "/", NULL};
	execve("/usr/bin/ls", newargv, NULL);
	//execlp("lllls", "ls", "/", NULL);
	//printf("errore %s\n", strerror(errno));
	//perror("exevlp");
}
