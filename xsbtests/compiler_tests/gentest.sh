#! /bin/sh

EMU=$1
FILE=$2

DIR=`pwd`
BASEDIR=`basename $DIR`

echo "--------------------------------------------------------------------"

echo "Testing $FILE"
$EMU -m 6000 -i << EOF

compile($FILE,[spec_dump,table_dump,ti_dump]).

EOF

d=`diff ${FILE}.O ${FILE}.O.old`
if test -z "$d"; then 
	echo "$BASEDIR/$FILE.O tested"
	rm -f ${FILE}.O
else
	echo "$BASEDIR/$FILE different\!\!\!"
	diff ${FILE}.O ${FILE}.O.old
fi

d=`diff ${FILE}.spec ${FILE}.spec.old`
if test -z "$d"; then 
	echo "$BASEDIR/$FILE.spec tested"
	rm -f ${FILE}.spec
else
	echo "$BASEDIR/$FILE different\!\!\!"
	diff ${FILE}.spec ${FILE}.spec.old
fi

d=`diff ${FILE}.table ${FILE}.table.old`
if test -z "$d"; then 
	echo "$BASEDIR/$FILE.table tested"
	rm -f ${FILE}.table
else
	echo "$BASEDIR/$FILE different\!\!\!"
	diff ${FILE}.table ${FILE}.table.old
fi

d=`diff ${FILE}.ti ${FILE}.ti.old`
if test -z "$d"; then 
	echo "$BASEDIR/$FILE.ti tested"
	rm -f ${FILE}.ti
else
	echo "$BASEDIR/$FILE different\!\!\!"
	diff ${FILE}.ti ${FILE}.ti.old
fi
