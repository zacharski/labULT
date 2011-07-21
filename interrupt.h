#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <signal.h>


void interruptsOn(void);
void interruptsOff(void);
void registerHandler(void);
void interruptHandler(int sig, siginfo_t *sip, void *vp);
int alarmIsEnabled();
void interruptsQuiet();
#endif
