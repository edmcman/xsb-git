echo "-------------------------------------------------------"
echo "--- Running neg_tests/test.csh                      ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================
#-------------------------------------------------
# Tests of negation that give correct results.
#-------------------------------------------------
gentest.csh $XEMU neg1 "test."
gentest.csh $XEMU neg2 "test."
gentest.csh $XEMU neg3 "test."
gentest.csh $XEMU ullman2 "test."
#------------------------------------------------------
# Tests of left-to-right modylarly stratified negation.
#------------------------------------------------------
gentest.csh $XEMU lmod1 "test."
gentest.csh $XEMU lmod2 "test."
gentest.csh $XEMU lmod3 "test."
gentest.csh $XEMU lmod4 "test."
gentest.csh $XEMU lmod5 "test."
gentest.csh $XEMU lmod6 "test."
gentest.csh $XEMU lmod7 "test."
gentest.csh $XEMU lmod8 "test."
gentest.csh $XEMU lmod9 "test."
gentest.csh $XEMU lmod10 "test."
gentest.csh $XEMU lmod11 "test."
gentest.csh $XEMU ullman1 "test."
#---------------------------------------------------------
# Tests of left-to-right dynamically stratified negation.
#---------------------------------------------------------
gentest.csh $XEMU przy2 "test."
gentest.csh $XEMU ldynstrat0 "test."
gentest.csh $XEMU ldynstrat1 "test."
gentest.csh $XEMU ldynstrat2 "test."
gentest.csh $XEMU ldynstrat3 "test."
gentest.csh $XEMU ldynstrat4 "test."
#--------------------------------------------------------------------
# Tests of modylarly stratified negation (that give correct results).
#--------------------------------------------------------------------
gentest.csh $XEMU mod1 "test."
#-------------------------------------------------
# Genome tests involving unclassified negation.
#-------------------------------------------------
gentest.csh $XEMU q7 "test."
