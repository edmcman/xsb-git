#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running prolog_tests/test.sh                    ---"
echo "-------------------------------------------------------"

XEMU=$1

#------------------------------------
# tests involving standard predicates
#------------------------------------
../gentest.sh $XEMU std1 "test."
../gentest.sh $XEMU std2 "testcombo."
#------------------------------------
# Test reading from strings
#------------------------------------
../gentest.sh $XEMU readstrtest "test."
#------------------------------------
# Test number_chars
#------------------------------------
../gentest.sh $XEMU n2c "test."
#------------------------------------
# keep the compiler honest
#------------------------------------
../gentest.sh $XEMU first "test."
../gentest.sh $XEMU newfirst "test."
#-----------------------------------------------------------------------
# the following make sure that the compiler will produce the right code
#-----------------------------------------------------------------------
../gentest.sh $XEMU inline "go."
../gentest.sh $XEMU unsafe1 "test."
../gentest.sh $XEMU unsafe2 "test."
#-----------------------------------------------------------------------
# the following two tests are used to test multifile directive
#-----------------------------------------------------------------------
#------------------------------------
# Just to create some .O files that
# will be used later in multifile test
#------------------------------------
../gentest.sh $XEMU mf_obj "do."
#------------------------------------
# Test multifile directive
#------------------------------------
../gentest.sh $XEMU mf_test1 "test."
#------------------------------------
# Test Prolog calling C
../gentest.sh $XEMU cinter1 "test."
#------------------------------------

