#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running basic_tests/test.sh                     ---"
echo "-------------------------------------------------------"

XEMU=$1

../gentest.sh $XEMU tsstr13 "testcombo."
../gentest.sh $XEMU tsstr23 "testcombo."
../gentest.sh $XEMU tsstr33 "testcombo."
#------------------------------------
# simplest series
#------------------------------------
../gentest.sh $XEMU tstr11 "tw,fail."
../gentest.sh $XEMU tstr21 "tw,fail."
../gentest.sh $XEMU tstr31 "tw,fail."
../gentest.sh $XEMU tstr51 "tw,fail."
../gentest.sh $XEMU tstr61 "tw,fail."
#------------------------------------
# simple series
#------------------------------------
../gentest.sh $XEMU tstr12 "testcombo."
../gentest.sh $XEMU tstr22 "testcombo."
../gentest.sh $XEMU tstr32 "testcombo."
../gentest.sh $XEMU tstr52 "testcombo."
../gentest.sh $XEMU tstr62 "testcombo."
#------------------------------------
# transitive closure series
#------------------------------------
../gentest.sh $XEMU tstr13 "tw."
../gentest.sh $XEMU tstr23 "tw."
../gentest.sh $XEMU tstr33 "tw."
../gentest.sh $XEMU tstr53 "tw."
../gentest.sh $XEMU tstr63 "tw."
../gentest.sh $XEMU tstr73 "tw."
#------------------------------------
# loader/compiler series
#------------------------------------
../gentest.sh $XEMU thstr13 "tw."
../gentest.sh $XEMU thstr43 "tw."
#------------------------------------
# simple levels series
#------------------------------------
../gentest.sh $XEMU thstr23 "tw1."
#------------------------------------
# cylinder series
#------------------------------------
../gentest.sh $XEMU tcyl11 "['tcyl-24-24-2'],tw(1)."
../gentest.sh $XEMU tcyl12 "['tcyl-24-24-2'],tw(1)."
../gentest.sh $XEMU testsg "[acyl,'tcyl-24-24-2'],tw."
#------------------------------------
# interpreter series.
#------------------------------------
../gentest.sh $XEMU interp "tinterp."
#------------------------------------
# tabletrysingle tests.
#------------------------------------
../gentest.sh $XEMU tsing1 "a(X,Y),write(X),write(' '),write(Y),nl,fail."
#--------------------------------
# h series is hilog/tabling tests
#------------------------------------
../gentest.sh $XEMU hirc "tc(manage)(X,Y),write(X),write(' '),write(Y),nl,fail."
#------------------------------------
../gentest.sh $XEMU findall "test."
#------------------------------------
../gentest.sh $XEMU altindex "test."
#------------------------------------
../gentest.sh $XEMU longname "test."

