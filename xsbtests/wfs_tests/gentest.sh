#! /bin/sh

EMU=$1
CONSULT_FILE=$2
TEST_FILE=$3
CMD=$4

echo "--------------------------------------------------------------------"
echo "Testing $TEST_FILE"
$EMU -m 3000 -i << EOF
[$CONSULT_FILE].
tell(temp).
$CMD
told.
EOF

# print out differences.
if test -f ${TEST_FILE}_new; then
	rm -f ${TEST_FILE}_new
fi
    
sort temp > ${TEST_FILE}_new

#-----------------------
# print out differences.
#-----------------------
d=`diff ${TEST_FILE}_new ${TEST_FILE}_old`
if test -z "$d"; then 
	echo "$TEST_FILE tested"
	rm -f ${TEST_FILE}_new
else
	echo "$TEST_FILE different\!\!\!"
	diff ${TEST_FILE}_new ${TEST_FILE}_old
fi

rm -f temp
