


CC	:= gcc 
CFLAGS := -g -Wall -Werror $(LABDEFS)

TARGETS := doTest doTest2 showHandler libULT.a alarmHelper parseUcontext

# Make sure that 'all' is the first target
all: $(TARGETS)

clean:
	rm -rf core *.o *.out $(TARGETS)

realclean: clean
	rm -rf *~ *.bak


tags:
	etags *.c *.h

TURNIN := /lusr/bin/turnin
GRADER := muqeet
LAB_NAME := handin-372-labULT
handin: handin.tar
	echo "Turning in handin.tar containing the following files:"
	tar tf handin.tar
	$(TURNIN) --submit $(GRADER) $(LAB_NAME) handin.tar


handin.tar: clean
	tar cf handin.tar `find . -type f | grep -v '^\.*$$' | grep -v '/CVS/' | grep -v '/\.svn/' | grep -v '/\.git/' | grep -v 'lab[0-9].*\.tar\.gz' | grep -v '/\~/' | grep -v 'proj-ULT.html' | grep -v '/\.tar/'` 





# Rules for the ULT library pieces

ULT_SRC :=  ULT.c interrupt.c
MOREH := cfuncproto.h
MOREO :=
ULT_OBJS := $(ULT_SRC:.c=.o)

# Search on the terms "makefile" "wildcards" to figure out this rule
# And, of course, look at the manual page for the ar utility
libULT.a: $(ULT_OBJS)
	ar rcs $@ $(ULT_OBJS)

ULT_LIB :=  -L . -l ULT 

#
# Individual .c, .o files.
#
# To keep things simple and obvious in the makefile
# I list the dependencies explicitly. For large
# projects with more interesting dependencies, it
# is better to generate them automatically.	
#  See 
#
#        Recursive Make Considered Harmful
#        http://aegis.sourceforge.net/auug97.pdf
#
# for an explanation (as well as a very nice discussion
# about structuring Makefiles for larger projects.
#
# Explicit dependencies instead, to keep things simple:
parseUcontext: parseUcontext.c 
	gcc $(CFLAGS) -o $@ $<

ULT.o: ULT.c ULT.h  $(MOREH)
	gcc -c $(CFLAGS) -o $@ ULT.c

interrupt.o: interrupt.c interrupt.h
	gcc -c $(CFLAGS) -o $@ interrupt.c


doTest.o: doTest.c basicThreadTests.h interrupt.h ULT.h  $(MOREH)
	gcc -c $(CFLAGS) -o $@ doTest.c

doTest: doTest.o libULT.a basicThreadTests.o 
	gcc $(CFLAGS) -o $@ doTest.o basicThreadTests.o  $(ULT_LIB)

doTest2.o: doTest2.c basicThreadTests.h interrupt.h ULT.h  $(MOREH)
	gcc -c $(CFLAGS) -o $@ doTest2.c

doTest2: doTest2.o libULT.a basicThreadTests.o 
	gcc $(CFLAGS) -o $@ doTest2.o basicThreadTests.o  $(ULT_LIB)

showHandler.o: showHandler.c interrupt.h ULT.h $(MOREH)
	gcc -c $(CFLAGS) -o $@ showHandler.c

showHandler: showHandler.o libULT.a
	gcc $(CFLAGS) -o $@ showHandler.o $(ULT_LIB)

alarmHelper: alarmHelper.c
	gcc $(CFLAGS) -o $@ $<

