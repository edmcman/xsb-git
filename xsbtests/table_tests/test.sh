#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running table_tests/test.sh                     ---"
echo "-------------------------------------------------------"

XEMU=$1

$XEMU << EOF

[set_reduce].

[lists].

[sets].

[correct].

[utils].

EOF

#--------------------------------------------------
# Tests get_calls and get_returns and tries as code
#--------------------------------------------------
../gentest.sh $XEMU concomp "t, d1, d2."
#--------------------------------------------------
../gentest.sh $XEMU testdyntable "test."
#--------------------------------------------------
../gentest.sh $XEMU aggregs_test "test."
#--------------------------------------------------
../gentest.sh $XEMU trassert "test."
#------------------------------------
../gentest.sh $XEMU trdyntest "test."
#------------------------------------
../gentest.sh $XEMU trdynctest "test."
#------------------------------------
../gentest.sh $XEMU trans "trans(z),writeln(ok)."
#------------------------------------
../gentest.sh $XEMU ins "main."
#------------------------------------
../gentest.sh $XEMU float "test."
#-----------------------------------
../gentest.sh $XEMU tabbug1 "test."
#-----------------------------------
../gentest.sh $XEMU expand "test."
#-----------------------------------
../gentest.sh $XEMU internt "test."
#-----------------------------------
../gentest.sh $XEMU grammarlrk3 "test."
#-----------------------------------
../gentest.sh $XEMU lrbug "test."
#-----------------------------------
../gentest.sh $XEMU unw_tr1 "test."
#-----------------------------------
../gentest.sh $XEMU trdyn "test".
#-----------------------------------
../gentest.sh $XEMU tfinda "test".
#-----------------------------------
# test heap reclamation after check complete
../gentest.sh $XEMU testh "time2048."

 
echo "--------------------------------------------------------------------"
echo "Testing empty_answer"
$XEMU -m 3000 -i << EOF
[empty_answer].
test.
EOF

