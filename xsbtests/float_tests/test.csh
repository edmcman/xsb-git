echo "-------------------------------------------------------"
echo "--- Running float_tests/test.csh                    ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================
gentest.csh $XEMU vac "test."
gentest.csh $XEMU roots "test."
gentest.csh $XEMU roots_int_comp "test."
gentest.csh $XEMU read_test "test."
gentest.csh $XEMU assert_test "test."
