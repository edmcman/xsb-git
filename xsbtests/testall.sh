#! /bin/sh 

echo "-------------------------------------------------------"
echo "--- Running testall.sh                              ---"
echo "-------------------------------------------------------"


XEMU=$1

lockfile=lock.test
testdir=`pwd`

trap 'rm -f $testdir/$lockfile; exit 1' 1 2 15

if test -f $testdir/$lockfile; then
   echo "************************************************************"
   echo ./$lockfile exists
   echo Probably testsuite is already running...
   echo If not, remove ./$lockfile
   echo and continue
   echo "************************************************************"
   exit
else
   echo $$ > $lockfile
fi


# float_tests  omitted??? - mk
dirlist="basic_tests prolog_tests retract_tests \
	 table_tests ptq neg_tests sem_tests delay_tests \
	 wfs_tests ai_tests"

for dir in $dirlist ; do
  cd $dir
  if test -f core ; then
     rm -f core
  fi
  ./test.sh "$XEMU"
  cd ..
done
   
rm -f $lockfile
