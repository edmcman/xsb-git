#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running neg_tests/test.sh                       ---"
echo "-------------------------------------------------------"

XEMU=$1

#-------------------------------------------------
# Tests of negation that give correct results.
#-------------------------------------------------
../gentest.sh $XEMU neg1 "test."
../gentest.sh $XEMU neg2 "test."
../gentest.sh $XEMU neg3 "test."
../gentest.sh $XEMU ullman2 "test."
#------------------------------------------------------
# Tests of left-to-right modylarly stratified negation.
#------------------------------------------------------
../gentest.sh $XEMU lmod1 "test."
../gentest.sh $XEMU lmod2 "test."
../gentest.sh $XEMU lmod3 "test."
../gentest.sh $XEMU lmod4 "test."
../gentest.sh $XEMU lmod5 "test."
../gentest.sh $XEMU lmod6 "test."
../gentest.sh $XEMU lmod7 "test."
../gentest.sh $XEMU lmod8 "test."
../gentest.sh $XEMU lmod9 "test."
../gentest.sh $XEMU lmod10 "test."
../gentest.sh $XEMU lmod11 "test."
../gentest.sh $XEMU ullman1 "test."
#---------------------------------------------------------
# Tests of left-to-right dynamically stratified negation.
#---------------------------------------------------------
../gentest.sh $XEMU przy2 "test."
../gentest.sh $XEMU ldynstrat0 "test."
../gentest.sh $XEMU ldynstrat1 "test."
../gentest.sh $XEMU ldynstrat2 "test."
../gentest.sh $XEMU ldynstrat3 "test."
../gentest.sh $XEMU ldynstrat4 "test."
#--------------------------------------------------------------------
# Tests of modylarly stratified negation (that give correct results).
#--------------------------------------------------------------------
../gentest.sh $XEMU mod1 "test."
#-------------------------------------------------
# Genome tests involving unclassified negation.
#-------------------------------------------------
../gentest.sh $XEMU q7 "test."
