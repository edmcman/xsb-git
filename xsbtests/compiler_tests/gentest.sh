#! /bin/sh

EMU=$1
FILE=$2

echo "--------------------------------------------------------------------"

echo "Testing $FILE"
$EMU -m 6000 -i << EOF

compile($FILE,[spec_dump,table_dump,ti_dump]).

EOF

d=`diff ${FILE}.O ${FILE}.O.old`
if test -z "$d"; then 
	echo "$FILE.O tested"
	rm -f ${FILE}.O
else
	echo "$FILE different\!\!\!"
	diff ${FILE}.O ${FILE}.O.old
fi

d=`diff ${FILE}.spec ${FILE}.spec.old`
if test -z "$d"; then 
	echo "$FILE.spec tested"
	rm -f ${FILE}.spec
else
	echo "$FILE different\!\!\!"
	diff ${FILE}.spec ${FILE}.spec.old
fi

d=`diff ${FILE}.table ${FILE}.table.old`
if test -z "$d"; then 
	echo "$FILE.table tested"
	rm -f ${FILE}.table
else
	echo "$FILE different\!\!\!"
	diff ${FILE}.table ${FILE}.table.old
fi

d=`diff ${FILE}.ti ${FILE}.ti.old`
if test -z "$d"; then 
	echo "$FILE.ti tested"
	rm -f ${FILE}.ti
else
	echo "$FILE different\!\!\!"
	diff ${FILE}.ti ${FILE}.ti.old
fi
