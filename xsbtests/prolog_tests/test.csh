echo "-------------------------------------------------------"
echo "--- Running prolog_tests/test.csh                   ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================
#------------------------------------
# tests involving standard predicates
#------------------------------------
gentest.csh $XEMU std1 "test."
gentest.csh $XEMU std2 "testcombo."
#------------------------------------
# Test reading from strings
#------------------------------------
gentest.csh $XEMU readstrtest "test."
#------------------------------------
# Test number_chars
#------------------------------------
gentest.csh $XEMU n2c "test."
#------------------------------------
# keep the compiler honest
#------------------------------------
gentest.csh $XEMU first "test."
gentest.csh $XEMU newfirst "test."
#-----------------------------------------------------------------------
# the following make sure that the compiler will produce the right code
#-----------------------------------------------------------------------
gentest.csh $XEMU inline "go."
gentest.csh $XEMU unsafe1 "test."
gentest.csh $XEMU unsafe2 "test."
#-----------------------------------------------------------------------
# the following two tests are used to test multifile directive
#-----------------------------------------------------------------------
#------------------------------------
# Just to create some .O files that
# will be used later in multifile test
#------------------------------------
gentest.csh $XEMU mf_obj "do."
#------------------------------------
# Test multifile directive
#------------------------------------
gentest.csh $XEMU mf_test1 "test."
#------------------------------------
# Test Prolog calling C
gentest.csh $XEMU cinter1 "test."
#------------------------------------

