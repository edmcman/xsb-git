#! /bin/sh

EMU=$1
FILE=$2
CMD=$3

echo "--------------------------------------------------------------------"
echo "Testing $FILE"
$EMU -m 3000 -i << EOF
[$FILE].
tell(temp).
$CMD
told.
EOF

# print out differences.
if test -f ${FILE}_new; then
	rm -f ${FILE}_new
fi
    
sort temp > ${FILE}_new

#-----------------------
# print out differences.
#-----------------------
d=`diff ${FILE}_new ${FILE}_old`
if test -z "$d"; then 
	echo "$FILE tested"
	rm -f ${FILE}_new
else
	echo "$FILE different\!\!\!"
	diff ${FILE}_new ${FILE}_old
fi

rm -f temp