#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running float_tests/test.sh                     ---"
echo "-------------------------------------------------------"

XEMU=$1

../gentest.sh $XEMU vac "test."
../gentest.sh $XEMU roots "test."
../gentest.sh $XEMU roots_int_comp "test."
../gentest.sh $XEMU read_test "test."
../gentest.sh $XEMU assert_test "test."
