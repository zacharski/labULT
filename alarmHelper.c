#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


long nsToSleep = 10000000;

int main(int argc, char ** argv)
{
  pid_t target;
  int ret;

  struct timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = nsToSleep;

  assert(argc == 2);
  target = atoi(argv[1]);
  printf("AlarmHelper will repeatedly signal pid %d\n", target);

  while(1){
    nanosleep(&delay, NULL);
    ret = kill(target, SIGALRM);
    if(ret){
      perror("(Expect to see this when parent exits): alarmHelper got error on kill. Stopping.");
      return 0;
    }
  }
}
