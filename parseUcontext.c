#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <ucontext.h>

#define SOL 99

unsigned int probeUCStack(char *string);

int main(int argc, char **argv)
{

  ucontext_t mycontext;
  int err;


  printf("WORDSIZE %d\n", __WORDSIZE);
  assert(__WORDSIZE == 32); // Do this project on a 32-bit x86 linux machine
  printf("NGREG %d\n", NGREG);
  assert(NGREG == 19);  // Do this project on a 32-bit x86 linux machine

  err = getcontext(&mycontext);
  assert(!err);

  printf("A ucontext_t is %d bytes\n", -999);
  assert(0); // TBD: Fill in ucontext size above. Hint: use sizeof().

  unsigned int anotherSample = probeUCStack("Dummy argument.");

  /* 
   * Now, look inside of the ucontext you just saved.
   *
   * Warning: The automated grading scrips won't check all of these.
   * Sanity check them yourself. Think about what they mean and what
   * they must be!
   *
   * For example, are both argc and err on the stack? 
   */
  /*
   * First, think about program counters (called eip in x86)
   */
  printf("The memory address of the function main() is 0x%x\n", (unsigned int)-1);
  printf("The memory address of the program counter (EIP) saved in mycontext is 0x%x\n", (unsigned int)-1);

  /*
   * Now, think about stacks. 
   *
   * Be able to answer the following questions: Does the stack grow up or down? 
   * Does ucontext_t.uc_stack point to the current stack pointer? Top of the
   * stack? Bottom of the stack? None of the above? What are the stack related 
   * things in mycontext.uc_mcontext.gregs[]? What's the difference between
   * REG_UESP and REG_ESP in the gregs array?
   *
   * Hint: It might help to run the program using gdb or xxx (a graphical
   * front end to gdb) and put a breakpoint at entry to main and/or the 
   * call to getcontext().  Then use "info registers" to see the values 
   * of the registers and use "next"/"step" to advance to the next instruction.
   * You can also use "print mycontext" to see the values stored in that struct.
   *
   * Anther experiment you might try is changing the various candidates for the
   * stack pointer to nonsensical values and (probably using a debugger) see
   * which one actually gets restored to the processor.
   *
   * Don't move on to the next part of the lab until you know how to change
   * the stack in a context when you manipulate a context to create a new thread.
   */
  printf("The memory address of the local variable err is 0x%x\n", (unsigned int)-1);
  printf("The memory address of the argument argc is 0x%x\n", (unsigned int)-1);
  printf("The value of ucontext_t.uc_stack is 0x%x\n", (unsigned int)mycontext.uc_stack.ss_sp);
  printf("The value of anotherSample is 0x%x\n", anotherSample);
  printf("The stack pointer stored as one of the registers (ESP) in uc_mcontext is 0x%x\n", (unsigned int)-1);
  printf("The stack pointer stored as another one of the `registers` (UESP) in uc_mcontext is 0x%x\n", (unsigned int)-1);


  printf("The number of bytes pushed onto the stack between argc and err was 0x%x\n", (unsigned int)(0xFFFFFF));
  /* Which is the right one to use? */
  printf("The number of bytes pushed onto the stack between err and when the stack was saved to mycontext was 0x%x\n", 
         (unsigned int)(-1));


         return 0;  
}


/*
 * This function should call getcontext and
 * return the value of the uc_stack.ss_sp.
 * 
 * Is this the current stack pointer? The
 * top of the stack? The bottom of the stack?
 * One thing to do is to compare it to the
 * uc_stack.ss_sp saved in main().
 */
unsigned int 
probeUCStack(char *str)
{
  assert(0); /* Write code for this function */
  return 0xFFFFFFFF;
}
