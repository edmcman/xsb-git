#! /bin/csh -f

if (!($?USER)) then
   setenv USER `whoami`
endif

set XEMU = $argv[1]
set GREP = "/usr/bin/grep -i"
set LOG_FILE = /tmp/xsb_test_log.$USER
set RES_FILE = /tmp/xsb_test_res.$USER
set EMPTY_FILE = /tmp/xsb_empty.$USER

if (-r $RES_FILE) then
  echo "There was an old $RES_FILE"
  echo "removing..."
  \rm -f $RES_FILE
endif

if (-r $EMPTY_FILE) then
  echo "There was an old $EMPTY_FILE"
  echo "removing..."
  \rm -f $EMPTY_FILE
endif


if (-r $LOG_FILE) then
  echo "There was an old $LOG_FILE"
  echo "removing..."
  \rm -f $LOG_FILE
endif

echo "Testing XSB: results will be in  $LOG_FILE"
csh tmptest.csh $XEMU >& $LOG_FILE

# check for errors
# for seg fault
$GREP "fault" $LOG_FILE > $RES_FILE
# core dumped
$GREP "dumped" $LOG_FILE >> $RES_FILE
# when no output file is generated
$GREP "match" $LOG_FILE >> $RES_FILE
# for bus error
$GREP "bus" $LOG_FILE >> $RES_FILE
# for wrong results
$GREP "different" $LOG_FILE >> $RES_FILE
# when xsb aborts...
$GREP "abort" $LOG_FILE >> $RES_FILE
# for overflows
$GREP "overflow" $LOG_FILE >> $RES_FILE


cat << EOF > $EMPTY_FILE
EOF

diff $EMPTY_FILE  $RES_FILE
if ($status == 1) then
	echo "Failed testsuite"
	mail -s "Testsuite failed" $USER@cs.sunysb.edu < $LOG_FILE
else
	echo "Passed testsuite"
endif

\rm -f $EMPTY_FILE
#\rm -f $LOG_FILE
\rm -f $RES_FILE

echo "Done"

