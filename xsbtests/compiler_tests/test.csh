echo "-------------------------------------------------------"
echo "--- Running copmiler_tests/test.csh                    ---"
echo "-------------------------------------------------------"
#============================================================================
set XEMU = $argv[1]
#============================================================================
gentest.csh $XEMU myspec
gentest.csh $XEMU myvarproc
gentest.csh $XEMU meta
gentest.csh $XEMU top_down

