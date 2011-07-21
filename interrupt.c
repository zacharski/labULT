#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include "interrupt.h"
static const int SIGNAL_TYPE = SIGALRM;

static void setAlarm();
static void setSet(sigset_t *setp);
int loud = 1;

void
interruptsOn()
{
  int ret;
  sigset_t sigset;

  setSet(&sigset);
  ret = sigprocmask(SIG_UNBLOCK, &sigset, NULL);
  assert(!ret);
}


 
void
interruptsOff()
{
  int ret;
  
  sigset_t sigset;

  setSet(&sigset);
  ret = sigprocmask(SIG_BLOCK, &sigset, NULL);
  assert(!ret);
}


static void 
setSet(sigset_t *setp){
  int ret;
  ret = sigemptyset(setp);
  assert(!ret);
  ret = sigaddset(setp, SIGNAL_TYPE);
  assert(!ret);
  return;
}
 

void
interruptsQuiet()
{
  loud = 0;
}
 
 
/*
 * Should be called when you initialize threads package.
 * Many of the calls won't make sense at first -- study the
 * man pages!
 */
void 
registerHandler()
{
  struct sigaction action;
  int error;
  static int hasBeenCalled = 0;

  assert(!hasBeenCalled); /* should only register once */
  hasBeenCalled = 1;
  
  action.sa_handler = NULL;
  action.sa_sigaction = interruptHandler;
  error = sigemptyset(&action.sa_mask);/* SIGNAL_TYPE will be blocked by default
                                        * while handler invoked.
                                        * No mask needed */
  assert(!error);
  action.sa_flags = SA_SIGINFO; /* use sa_sigaction as handler
                                 * instead of sa_handler*/

  if(sigaction(SIGNAL_TYPE, &action, NULL)){
    perror("Setting up signal handler");
    assert(0);
  }
 
  setAlarm();
}
 
/*
 * STUB: once registerhandler is called, this routine
 * gets called each time SIGNAL_TYPE is sent to this process
 */
void 
interruptHandler(int sig, siginfo_t *sip, void *contextVP)
{

  /* Tell compiler not to complain about unused variable
     in this stub version of the fuction */
  /*NOTUSED*/
  ucontext_t *context = (ucontext_t *)contextVP;

  /*
   * SIGALRM is masked on entry 
   */
  sigset_t mask;
  int ret = sigprocmask(0, NULL, &mask);
  assert(!ret);
  assert(sigismember(&mask, SIGALRM));

  if(loud){
    printf("Made it to handler with interrupted thread's state at %p\n",
           context);
  }
  setAlarm();


  /* 
   * Your code here 
   */

  return;
}
 

/*
 * Use the alarm() system call to set an alarm for
 * some time into the future. At that time, this
 * process will receive a SIGALRM signal.
 *
 * NOTE: in Solaris, you must link the program with -lpthread
 * to tell it to deliver signals to the process, not 
 * to a particular thread. If you do not link with -lpthread
 * Solaris will use its default alarm() semantics, which
 * tries to deliver the alarm signal to the thread that
 * set the alarm. Since you are implementing your own threads
 * for this project, the Solaris version seems likely to
 * get confused. See the man page for alarm for details.
 */
static void
setAlarm()
{
  static const int SECONDS_TO_NEXT = 1;

  alarm(SECONDS_TO_NEXT);
  
}


int
alarmIsEnabled()
{
  sigset_t mask;
  int ret;

  ret = sigprocmask(0, NULL, &mask);
  assert(!ret);
  return(sigismember(&mask, SIGALRM) ? 0 : 1);
}

