#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running ai_tests/test.sh                        ---"
echo "-------------------------------------------------------"
#=============================================================================
XEMU=$1
#=============================================================================
../gentest.sh $XEMU cs_o "test."
../gentest.sh $XEMU cs_r "test."
../gentest.sh $XEMU disj "test."
../gentest.sh $XEMU gabriel "test."
../gentest.sh $XEMU kalah "test."
../gentest.sh $XEMU peep "test."
../gentest.sh $XEMU pg "test."
../gentest.sh $XEMU plan "test."
# ../gentest.sh $XEMU press1 "test."	# Not certain on the correct results
../gentest.sh $XEMU qsort "test."
../gentest.sh $XEMU queens "test."
../gentest.sh $XEMU read "test."
