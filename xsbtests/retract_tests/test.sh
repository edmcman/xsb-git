#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running retract_tests/test.sh                   ---"
echo "-------------------------------------------------------"

XEMU=$1

#---------------------------------------
# Assert and retract tests.
#---------------------------------------
../gentest.sh $XEMU testretract "test."
#---------------------------------------
../gentest.sh $XEMU boyer_assert "boyer."
#---------------------------------------
