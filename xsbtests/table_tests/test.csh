echo "-------------------------------------------------------"
echo "--- Running table_tests/test.csh                    ---"
echo "-------------------------------------------------------"
#=============================================================================
set XEMU = $argv[1]
#=============================================================================

$XEMU << EOF

[set_reduce].

[sets].

[correct].

[utils].

EOF
#--------------------------------------------------
# Tests get_calls and get_returns and tries as code
#--------------------------------------------------
gentest.csh $XEMU concomp "t, d1, d2."
#--------------------------------------------------
gentest.csh $XEMU testdyntable "test."
#--------------------------------------------------
gentest.csh $XEMU aggregs_test "test."
#--------------------------------------------------
gentest.csh $XEMU trassert "test."
#------------------------------------
gentest.csh $XEMU trdyntest "test."
#------------------------------------
gentest.csh $XEMU trdynctest "test."
#------------------------------------
gentest.csh $XEMU trans "trans(z),writeln(ok)."
#------------------------------------
gentest.csh $XEMU ins "main."
#------------------------------------
gentest.csh $XEMU float "test."
#-----------------------------------
gentest.csh $XEMU tabbug1 "test."
#-----------------------------------
gentest.csh $XEMU expand "test."
#-----------------------------------
gentest.csh $XEMU internt "test."
#-----------------------------------
gentest.csh $XEMU grammarlrk3 "test."
#-----------------------------------
gentest.csh $XEMU lrbug "test."
#-----------------------------------
gentest.csh $XEMU unw_tr1 "test."
#-----------------------------------
gentest.csh $XEMU trdyn "test".
#-----------------------------------
gentest.csh $XEMU tfinda "test".
#-----------------------------------
# test heap reclamation after check complete
gentest.csh $XEMU testh "time2048."

 
echo "--------------------------------------------------------------------"
echo "Testing empty_answer"
$argv[1] -m 3000 -i << EOF
[empty_answer].
test.
EOF

