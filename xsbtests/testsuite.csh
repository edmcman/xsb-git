#! /bin/csh -f
#! /bin/sh

# Generated automatically from xsb.in by configure.

## File:      testsuite.csh
## Author(s): Juliana Freire
## Contact:   xsb-contact@cs.sunysb.edu
## 
## Copyright (C) The Research Foundation of SUNY, 1996-1998
## 
## XSB is free software; you can redistribute it and/or modify it under the
## terms of the GNU Library General Public License as published by the Free
## Software Foundation; either version 2 of the License, or (at your option)
## any later version.
## 
## XSB is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License for
## more details.
## 
## You should have received a copy of the GNU Library General Public License
## along with XSB; if not, write to the Free Software Foundation,
## Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
##
## $ID$
## 
##

#==================================================================
# This is supposed to automate the testsuite by checking the
# log for possible errors.
#==================================================================

echo ==============================================================

if -e uniq_lock then
   echo "************************************************************"
   echo uniq_lock exists
   echo Probably somebody else is running the testsuite
   echo If not then remove ./uniq_lock
   echo and continue
   echo "************************************************************"
   exit 0
endif


if (($#argv > 1)||($#argv == 0)) then
	echo "Usage: testsuite.csh <full path for emulator>"
	echo ""
	exit 1
endif

if (!($?USER)) then
   setenv USER `whoami`
endif

set XEMU = $argv[1]

#set GREP = "/usr/bin/grep -i"
set GREP = "grep -i"
set MSG_FILE = /tmp/xsb_test_msg.$USER
set LOG_FILE = /tmp/xsb_test_log.$USER
set LOG_FILE_LOCAL = xsb_test_log.$USER
set RES_FILE = /tmp/xsb_test_res.$USER
set EMPTY_FILE = /tmp/xsb_empty.$USER

if !(-x $XEMU) then
  echo "Can't execute $XEMU"
  echo "aborting..."
  goto EMU_NOT_FOUND
endif

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


# should parameterize: create a script that given an input file
# greps for errors and prints the results to an output file
# and then this script can also be used in buildtest

echo "Testing $XEMU"
echo "Results will be in  $LOG_FILE"

echo "Log for  $XEMU" > $LOG_FILE
echo "Date-Time: `date +"%y%m%d-%H%M"`" >> $LOG_FILE
if ($status) then
	echo "Date-Time: no date for NeXT..." >> $LOG_FILE
	set NEXT_DATE = 1
else
	set NEXT_DATE = 0
endif
csh testall.csh $XEMU >>& $LOG_FILE

echo "-----------------------------------------"
echo "The following core dumps occurred during this test run:"
find . -name core -print
echo "End of the core dumps list"
# check for seg fault
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
$GREP "abnorm" $LOG_FILE >> $RES_FILE
# sometimes after overflow the diff fails and a message with Missing
# is displayed
$GREP "Missing" $LOG_FILE >> $RES_FILE
# 
$GREP "fatal" $LOG_FILE >> $RES_FILE
echo "-----------------------------------------"

cat << EOF > $EMPTY_FILE
EOF

if ($NEXT_DATE == 1) then
	set NEW_LOG = $LOG_FILE.$NEXT_DATE
	\cp $LOG_FILE $NEW_LOG
else
	set NEW_LOG = $LOG_FILE.`date +"%y%m%d-%H%M"`
	\cp $LOG_FILE $NEW_LOG
endif

diff $EMPTY_FILE  $RES_FILE
if ($status == 1) then
	set HOSTNAME = `hostname`
	echo "Failed testsuite on $HOSTNAME"
        echo "Testsuite for $XEMU failed" > $MSG_FILE
	echo "Check the log file $NEW_LOG" >> $MSG_FILE
# Mail does not work on the HP
#	Mail -s "Testsuite failed" $USER@cs.sunysb.edu < $MSG_FILE
	mail $USER@cs.sunysb.edu < $MSG_FILE
	\rm -f $MSG_FILE
else
	echo "Passed testsuite"
endif

\rm -f $EMPTY_FILE
# \rm -f $LOG_FILE
\rm -f $RES_FILE
# \rm -f $MSG_FILE

echo "Done"
echo ==============================================================
exit 0

EMU_NOT_FOUND:
	echo "Can't execute $XEMU" >$MSG_FILE
	set HOSTNAME = `hostname`
	echo "Aborted testsuite on $HOSTNAME..." >> $MSG_FILE
# Mail does not work on the HP
#	Mail -s "Testsuite aborted" $USER@cs.sunysb.edu < $MSG_FILE
	mail $USER@cs.sunysb.edu < $MSG_FILE
	\rm -f $MSG_FILE
	exit 1

