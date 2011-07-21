#include <assert.h>
#include <stdio.h>
#include "interrupt.h"
#include "ULT.h"
#include "basicThreadTests.h"

int main(int argc, char **argv)
{
  registerHandler();


  /* 
   * Make sure basic thread stuff still works
   */
  basicThreadTests();

  /*
   * Test preemptive threads
   */
  preemptiveTests();

  return 0;
}
