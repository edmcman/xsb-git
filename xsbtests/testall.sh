#! /bin/sh 

echo "-------------------------------------------------------"
echo "--- Running testall.sh                              ---"
echo "-------------------------------------------------------"

if test -f uniq_lock; then
   echo uniq_lock exists
   echo Probably somebody else is running the testsuite
   echo If not then remove uniq_lock
   echo and continue
   exit
fi

echo $$ > uniq_lock

XEMU=$1

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
   
rm -f uniq_lock
