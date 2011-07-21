#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
 
/*
 * Scheduling quanta in milliseconds
 */
static const int QUANTA_US = 1000000;
 
int
main(int argc, char **argv)
{
  int pid, error;
 
  if(argc != 2){
    fprintf(stderr, "Usage: signalMe pid\n");
    exit(-1);
  }
  pid = atoi(argv[1]);
  printf("signalMe will send periodic signals to %d\n", pid);
  while(1){
    printf("SignalMe sending signal\n");
    error = kill(pid, SIGUSR1);
    if(error){
      if(errno == ESRCH){
        printf("signalMe: child %d is done\n", pid);
      }
      else{
        perror("signalMe: error");
      }
      exit(0);
    }
    usleep(QUANTA_US);
  }
}
 
 
