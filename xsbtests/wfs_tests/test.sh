#! /bin/sh

#============================================================================
echo "-------------------------------------------------------"
echo "--- Running wfs_tests/test.sh                       ---"
echo "-------------------------------------------------------"

XEMU=$1

file_list=p*.P

for file in $file_list ; do
	prog=`basename $file .P`
	gentest.sh $XEMU test $prog "test($prog)."
done
