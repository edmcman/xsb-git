echo "-------------------------------------------------------"
echo "--- Running basic_tests/test.csh                    ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================
gentest.csh $XEMU tsstr13 "testcombo."
gentest.csh $XEMU tsstr23 "testcombo."
gentest.csh $XEMU tsstr33 "testcombo."
#------------------------------------
# simplest series
#------------------------------------
gentest.csh $XEMU tstr11 "tw,fail."
gentest.csh $XEMU tstr21 "tw,fail."
gentest.csh $XEMU tstr31 "tw,fail."
gentest.csh $XEMU tstr51 "tw,fail."
gentest.csh $XEMU tstr61 "tw,fail."
#------------------------------------
# simple series
#------------------------------------
gentest.csh $XEMU tstr12 "testcombo."
gentest.csh $XEMU tstr22 "testcombo."
gentest.csh $XEMU tstr32 "testcombo."
gentest.csh $XEMU tstr52 "testcombo."
gentest.csh $XEMU tstr62 "testcombo."
#------------------------------------
# transitive closure series
#------------------------------------
gentest.csh $XEMU tstr13 "tw."
gentest.csh $XEMU tstr23 "tw."
gentest.csh $XEMU tstr33 "tw."
gentest.csh $XEMU tstr53 "tw."
gentest.csh $XEMU tstr63 "tw."
gentest.csh $XEMU tstr73 "tw."
#------------------------------------
# loader/compiler series
#------------------------------------
gentest.csh $XEMU thstr13 "tw."
gentest.csh $XEMU thstr43 "tw."
#------------------------------------
# simple levels series
#------------------------------------
gentest.csh $XEMU thstr23 "tw1."
#------------------------------------
# cylinder series
#------------------------------------
gentest.csh $XEMU tcyl11 "tw(1)."
gentest.csh $XEMU tcyl12 "tw(1)."
gentest.csh $XEMU testsg "tw."
#------------------------------------
# interpreter series.
#------------------------------------
gentest.csh $XEMU interp "tinterp."
#------------------------------------
# tabletrysingle tests.
#------------------------------------
gentest.csh $XEMU tsing1 "a(X,Y),write(X),write(' '),write(Y),nl,fail."
#--------------------------------
# h series is hilog/tabling tests
#------------------------------------
gentest.csh $XEMU hirc "tc(manage)(X,Y),write(X),write(' '),write(Y),nl,fail."
#------------------------------------
gentest.csh $XEMU findall "test."
#------------------------------------
gentest.csh $XEMU altindex "test."
#------------------------------------
gentest.csh $XEMU longname "test."

