#!/bin/sh

echo
echo ====================================================
echo grade.sh does some very simple tests on the text
echo output of your program. Passing grade.sh is
echo not a guarantee that your code is correct or
echo that it will receive a good grade. You 
echo should also do your own tests.
echo
echo Attempting to mislead the grading script
echo is not permitted and will be treated as
echo academic dishonesty.
echo ====================================================
echo
make -k
passed=0
failed=0
xcpassed=0
xcfailed=0

echo "Test ucontext parsing"
./parseUcontext > checkUcontext.grade.out
echo ooooooooooooooooooooooooooooooooo
echo Look at checkUcontext.grade.out
echo ooooooooooooooooooooooooooooooooo
cat checkUcontext.grade.out | awk -f checkUcontext.awk
if cat checkUcontext.grade.out | awk -f checkUcontext.awk | grep "Might-be-OK-count: 7" > /dev/null
then
  passed=`expr 1 + $passed`
  echo OK: Basic parsing of context looks OK
else
  failed=`expr 1 + $failed`
fi

echo ooooooooooooooooooooooooooooooooo
echo "Test nonpreemptive"
./doTest > doTest.grade.out
echo ooooooooooooooooooooooooooooooooo
echo Look at doTest.grade.out
echo ooooooooooooooooooooooooooooooooo
if (cat doTest.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest.expected) | awk -f checkAll.awk | grep "checkAll-OK" > /dev/null
then
   passed=`expr 1 + $passed`
   echo OK: Basic non-preemptive looks OK
else
   failed=`expr 1 + $failed`
   echo ***********
   (cat doTest.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest.expected) | awk -f checkAll.awk
   echo Basic non-preemptive looks broken
   echo ***********
fi


echo ooooooooooooooooooooooooooooooooo
echo "Test (XC) preemptive (this will take 10-15 seconds)"
./doTest2 > doTest2.grade.out
echo ooooooooooooooooooooooooooooooooo
echo Look at doTest2.grade.out
echo ooooooooooooooooooooooooooooooooo
if (cat doTest2.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest2.expected) | awk -f checkAll.awk | grep "checkAll-OK" > /dev/null
then
   xcpassed=`expr 1 + $xcpassed`
   echo OK: Basic tests still look OK
else
   xcfailed=`expr 1 + $xcfailed`
   echo ooooooooooo
   (cat doTest2.grade.out; echo XYZ_BREAK_TOKEN_XYZ; cat doTest2.expected) | awk -f checkAll.awk
   echo Basic non-preemptive looks broken
   echo ooooooooooo
fi


#
# Should make it around at least 5 times...
# I usually get around 22 times in my tests.
# But if this fails, be sure to check it manually. Maybe
# the machine is just slow. Try increasing duration
# of test.
#
if cat doTest2.grade.out | grep "9 passes potato for 5-st/nd/rd time" > /dev/null
then
   xcpassed=`expr 1 + $xcpassed`
   echo 9-5 OK
else
   xcfailed=`expr 1 + $xcfailed`
   echo 9-5 MAY BE WRONG but check doTest2.grade.out manually it is timing sensitive 
fi

echo oooooooooooooooooooo
echo oooooooooooooooooooo
echo oooooooooooooooooooo

echo "Output passed: $passed  Output failed: $failed"
echo "Output xcpassed: $xcpassed  Output xcfailed: $xcfailed"










