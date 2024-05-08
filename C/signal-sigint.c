/*


Most of the Linux users use the key 
combination Ctr+C to terminate processes in Linux.

Have you ever thought of what goes behind this? 
Well, whenever Ctrl+C is pressed, a signal SIGINT 
is sent to the process. The default action of this 
signal is to terminate the process. But this signal 
can also be handled. The following code demonstrates this: 
*/

#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
}

int main(void)
{
  if (signal(SIGINT, sig_handler) == SIG_ERR)
  printf("\ncan't catch SIGINT\n");
  // A long long wait so that we can easily issue a signal to this 
  // process
  while(1) 
    sleep(1);
  return 0;
}