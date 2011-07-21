#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "basicThreadTests.h"
#include "interrupt.h"
#include "ULT.h"
/*
 *  Using posix mutex for some tests of phases 1 and 2
 */
#include <pthread.h> 

static const int verbose = 0;
static const int vverbose = 0;

static void hello(char *msg);
static int fact(int n);
static void suicide();
static void finale();
static int setFlag(int val);
static void startAlarmHelper();
static void doPotato(int mySlot);
static int tryMovePotato(int mySlot);
static void spin(int secs);



void basicThreadTests()
{
  Tid ret;
  Tid ret2;

  printf("Starting tests...\n");

  /*
   * Initial thread yields
   */
  ret = ULT_Yield(ULT_SELF);
  assert(ULT_isOKRet(ret));
  printf("Initial thread returns from Yield(SELF)\n");
  ret = ULT_Yield(0); /* See ULT.h -- initial thread must be Tid 0 */
  assert(ULT_isOKRet(ret));
  printf("Initial thread returns from Yield(0)\n");
  ret = ULT_Yield(ULT_ANY);
  assert(ret == ULT_NONE);
  printf("Initial thread returns from Yield(ANY)\n");
  ret = ULT_Yield(0xDEADBEEF);
  assert(ret == ULT_INVALID);
  printf("Initial thread returns from Yield(INVALID)\n");
  ret = ULT_Yield(16);
  assert(ret == ULT_INVALID);
  printf("Initial thread returns from Yield(INVALID2)\n");
  
  /*
   * Create a thread
   */
  ret = ULT_CreateThread((void (*)(void *))hello, "World");
  assert(ULT_isOKRet(ret));
  ret2 = ULT_Yield(ret);
  assert(ret2 == ret);

  /*
   * Create 10 threads
   */
  int ii;
  static const int NTHREAD = 10;
  Tid children[NTHREAD];
  char msg[NTHREAD][1024];
  for(ii = 0; ii < NTHREAD; ii++){
    ret = snprintf(msg[ii], 1023, "Hello from thread %d\n", ii);
    assert(ret > 0);
    children[ii] = ULT_CreateThread((void (*)(void *))hello, msg[ii]);
    assert(ULT_isOKRet(children[ii]));
  }
  for(ii = 0; ii < NTHREAD; ii++){
    ret = ULT_Yield(children[ii]);
    assert(ret == children[ii]);
  }


  /*
   * Destroy 11 threads we just created
   */
  ret = ULT_DestroyThread(ret2);
  assert(ret == ret2);
  for(ii = 0; ii < NTHREAD; ii++){
    ret = ULT_DestroyThread(children[ii]);
    assert(ret == children[ii]);
  }

  /*
   * Create maxthreads-1 threads
   */
  printf("Creating %d threads\n", ULT_MAX_THREADS-1);
  for(ii = 0; ii < ULT_MAX_THREADS-1; ii++){
    ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
    assert(ULT_isOKRet(ret));
  }
  /*
   * Now we're out of threads. Next create should fail.
   */
  ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
  assert(ret == ULT_NOMORE);
  /*
   * Now let them all run.
   */
  printf("Running %d threads\n", ULT_MAX_THREADS-1);
  for(ii = 0; ii < ULT_MAX_THREADS; ii++){
    ret = ULT_Yield(ii);
    if(ii == 0){ 
      /* 
       * Guaranteed that first yield will find someone. 
       * Later ones may or may not depending on who
       * stub schedules  on exit.
       */
      assert(ULT_isOKRet(ret));
    }
  }
  /*
   * They should have cleaned themselves up when
   * they finished running. Create maxthreads-1 threads.
   */
  printf("Creating %d threads\n", ULT_MAX_THREADS-1);
  for(ii = 0; ii < ULT_MAX_THREADS-1; ii++){
    ret = ULT_CreateThread((void (*)(void *))fact, (void *)10);
    assert(ULT_isOKRet(ret));
  }
  /*
   * Now destroy some explicitly and let the others run
   */
  printf("Destorying %d threads, running the rest\n",
         ULT_MAX_THREADS/2);
  for(ii = 0; ii < ULT_MAX_THREADS; ii+=2){
    ret = ULT_DestroyThread(ULT_ANY);
    assert(ULT_isOKRet(ret));
  }
  for(ii = 0; ii < ULT_MAX_THREADS; ii++){
    ret = ULT_Yield(ii);
  }
  printf("Trying some destroys even though I'm the only thread\n");
  ret = ULT_DestroyThread(ULT_ANY);
  assert(ret == ULT_NONE);
  ret = ULT_DestroyThread(42);
  assert(ret == ULT_INVALID);
  ret = ULT_DestroyThread(-42);
  assert(ret == ULT_INVALID);
  ret = ULT_DestroyThread(ULT_MAX_THREADS + 1000);
  assert(ret == ULT_INVALID);

  /*
   * Create a tread that destroys itself. 
   * Control should come back here after
   * that thread runs.
   */
  printf("Testing destroy self\n");
  int flag = setFlag(0);
  ret = ULT_CreateThread((void (*)(void *))suicide, NULL);
  assert(ULT_isOKRet(ret));
  ret = ULT_Yield(ret);
  assert(ULT_isOKRet(ret));
  flag = setFlag(0);
  assert(flag == 1); /* Other thread ran */
  /* That thread is gone now */
  ret = ULT_Yield(ret);
  assert(ret == ULT_INVALID);

}

void 
grandFinale()
{
  int ret;
  printf("For my grand finale, I will destroy myself\n");
  printf("while my talented assistant prints Done.\n");
  ret = ULT_CreateThread((void (*)(void *))finale, NULL);
  assert(ULT_isOKRet(ret));
  ULT_DestroyThread(ULT_SELF);
  assert(0);

}


static void
hello(char *msg)
{
  Tid ret;

  printf("Made it to hello() in called thread\n");
  printf("Message: %s\n", msg);
  ret = ULT_Yield(ULT_SELF);
  assert(ULT_isOKRet(ret));
  printf("Thread returns from first yield\n");

  ret = ULT_Yield(ULT_SELF);
  assert(ULT_isOKRet(ret));
  printf("Thread returns from second yield\n");

  while(1){
    ULT_Yield(ULT_ANY);
  }
  
}

static int
fact(int n){
  if(n == 1){
    return 1;
  }
  return n*fact(n-1);
}

static void suicide()
{
  int ret = setFlag(1);
  assert(ret == 0);
  ULT_DestroyThread(ULT_SELF);
  assert(0);
}

/*
 *  Using posix mutex for some tests of phases 1 and 2
 */
static pthread_mutex_t posix_mutex = PTHREAD_MUTEX_INITIALIZER;
static int flagVal;

static int setFlag(int val)
{
  int ret;
  int err;
  err = pthread_mutex_lock(&posix_mutex);
  assert(!err);
  ret = flagVal;
  flagVal = val;
  err = pthread_mutex_unlock(&posix_mutex);
  assert(!err);
  return ret;
}


static void finale()
{
  int ret;
  printf("Finale running\n");
  ret = ULT_Yield(ULT_ANY);
  assert(ret == ULT_NONE);
  ret = ULT_Yield(ULT_ANY);
  assert(ret == ULT_NONE);
  printf("Done.\n");
  /* 
   * Stub should exit cleanly if there are no threads left to run.
   */
  return; 
}


#define NPOTATO  10
static int potato[NPOTATO];
static Tid potatoTids[NPOTATO];
static const int tPotato = 30;

void preemptiveTests()
{

  static const int duration = 10;
  int ret;

  int ii;

  startAlarmHelper();

  spin(2);

  interruptsQuiet();

  potato[0] = 1;
  for(ii = 1; ii < NPOTATO; ii++){
    potato[ii] = 0;
  }

  printf("Running hot potato test. This will take %d seconds\n",
         duration);

  for(ii = 0; ii < NPOTATO; ii++){
    potatoTids[ii] = ULT_CreateThread((void (*)(void *))doPotato, (void *)ii);
    assert(ULT_isOKRet(potatoTids[ii]));
  }


  spin(duration);

  printf("Hot potato done. Cleaning up\n");

  for(ii = 0; ii < NPOTATO; ii++){
    ret = ULT_DestroyThread(ULT_ANY);
    assert(ULT_isOKRet(ret));
  }  

  printf("Done.\n");
}

static void spin(int secs)
{
  struct timeval start, end;
  int ret;

  ret = gettimeofday(&start, NULL);
  assert(!ret);
  while(1){
    ret = gettimeofday(&end, NULL);
    if(end.tv_sec - start.tv_sec >= secs){
      break;
    }
  }
}


static void
startAlarmHelper()
{
  int ret;
  char command[1024];

  pid_t myPid = getpid();
  snprintf(command, 1024, "./alarmHelper %d &\n", myPid);
  ret = system(command);
  printf("Started alarmHelper to speed up ALARM interupts\n");
  return;
}


static void
doPotato(int mySlot)
{
  int ret;
  int moved;
  int passes = 0;

  printf("Made it to doPotato %d\n", mySlot);
  while(1){
    assert(alarmIsEnabled());
    moved = tryMovePotato(mySlot);
    if(moved){
      passes++;
      printf("%d passes potato for %d-st/nd/rd time \n", mySlot, passes);
    }

    /*
     * Add some yields by some threads to scramble the list
     */
    if(mySlot > 4){
      int ii;
      for(ii = 0; ii < mySlot - 4; ii++){
        ret = ULT_Yield(ULT_ANY);
        assert(ULT_isOKRet(ret));
      }
    }
  }
}


static int
tryMovePotato(int mySlot)
{
  int err;
  int ret = 0;

  err = pthread_mutex_lock(&posix_mutex);
  assert(!err);

  if(potato[mySlot]){
    ret = 1;
    potato[mySlot] = 0;
    potato[(mySlot + 1) % NPOTATO] = 1;
  }

  err = pthread_mutex_unlock(&posix_mutex);
  assert(!err);
  return ret;
}



