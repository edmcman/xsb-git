#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running compiler_tests/test.sh                  ---"
echo "-------------------------------------------------------"

XEMU=$1

gentest.sh $XEMU myspec
gentest.sh $XEMU myvarproc
gentest.sh $XEMU meta
gentest.sh $XEMU top_down

