#! /bin/sh

# $1 must be the XSB to test
# $2 must be the path of the XSB installation directory
# This script expects to be run from the testsuite directory

XEMU=$1
xsbdir=$2
testdir=`pwd`
logfile=/tmp/xsb_super_log.`whoami`
lockfile=lock.super

test -x "$1" || \
 (echo "$1: XSB executable doesn't exist or has wrong permissions" ; exit)
test -d "$2" || \
 (echo "$2: XSB root directory doesn't exist or has wrong permissions" ; exit)

trap 'rm -fr $testdir/$lockfile $testdir/lock.test; exit 1' 1 2 15

if test -f "$testdir/$lockfile" ; then
 echo "./$lockfile exists. Remove it first"
 exit
else
 echo $$ > $lockfile
fi

cd $xsbdir/build
echo "Configuring $XEMU with default options"
configure > $logfile
echo "Making $XEMU with default options"
makexsb fast >> $logfile
cd $testdir
./testsuite.sh $2/bin/xsb

cd $xsbdir/build
echo "Configuring $XEMU with --enable-local-scheduling"
configure --config-tag=localsched --enable-local-scheduling >> $logfile
echo "Making $XEMU with --enable-local-scheduling"
makexsb --config-tag=localsched fast >> $logfile
cd $testdir
./testsuite.sh $2/bin/xsb-localsched

cd $xsbdir/build
echo "Configuring $XEMU with --enable-chat"
configure --config-tag=chat --enable-chat >> $logfile
echo "Making $XEMU with --enable-chat"
makexsb --config-tag=chat fast >> $logfile
cd $testdir
./testsuite.sh $2/bin/xsb-chat


cd $xsbdir/build
echo "Configuring $XEMU with --enable-chat --enable-local-scheduling"
configure --config-tag=localschedNchat --enable-chat --enable-local-scheduling >> $logfile
echo "Making $XEMU with --enable-chat --enable-local-scheduling"
makexsb --config-tag=localschedNchat fast >> $logfile
cd $testdir
./testsuite.sh $2/bin/xsb-localschedNchat

rm $testdir/$lockfile
