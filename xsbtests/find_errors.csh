#! /bin/csh -f

echo ==============================================================
if (($#argv > 2)||($#argv == 0)) then
	echo "Usage: find_errors.csh <input file> <output file>"
	echo ""
	exit 1
endif

set LOG_FILE = $argv[1]
set RES_FILE = $argv[2]
set GREP = "/usr/bin/grep -i"

echo "Will read $LOG_FILE and dump errors to $RES_FILE"

echo "-----------------------------------------"
# for seg fault
$GREP "fault" $LOG_FILE > $RES_FILE
# core dumped
$GREP "dumped" $LOG_FILE >> $RES_FILE
# when no output file is generated
$GREP "match" $LOG_FILE >> $RES_FILE
# for bus error
$GREP "bus" $LOG_FILE >> $RES_FILE
# for really bad outcomes
$GREP "Missing" $LOG_FILE >> $RES_FILE
# for wrong results
$GREP "different" $LOG_FILE >> $RES_FILE
# when xsb aborts... it writes something like ! Aborting...
$GREP "bort" $LOG_FILE >> $RES_FILE
# for overflows (check for Overflow & overflow)
$GREP "verflow" $LOG_FILE >> $RES_FILE
# for ... missing command...
$GREP "not found" $LOG_FILE >> $RES_FILE
# sometimes after overflow the diff fails and a message with Missing
# is displayed
$GREP "Missing" $LOG_FILE >> $RES_FILE
# 
$GREP "fatal" $LOG_FILE >> $RES_FILE
echo "-----------------------------------------"

