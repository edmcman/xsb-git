echo "-------------------------------------------------------"
echo "--- Running ai_tests/test.csh                       ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================
gentest.csh $XEMU cs_o "test."
gentest.csh $XEMU cs_r "test."
gentest.csh $XEMU disj "test."
gentest.csh $XEMU gabriel "test."
gentest.csh $XEMU kalah "test."
gentest.csh $XEMU peep "test."
gentest.csh $XEMU pg "test."
gentest.csh $XEMU plan "test."
# gentest.csh $XEMU press1 "test."	# Not certain on the correct results
gentest.csh $XEMU qsort "test."
gentest.csh $XEMU queens "test."
gentest.csh $XEMU read "test."
