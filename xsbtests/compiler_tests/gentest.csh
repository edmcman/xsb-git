#=============================================================================
#set $argv[1] = EMULATOR
#set $argv[2] = FILE 
#=============================================================================
echo "--------------------------------------------------------------------"
#util "Testing $argv[2]"
echo "Testing $argv[2]"
$argv[1] -m 6000 -i << EOF

compile($argv[2],[spec_dump,table_dump,ti_dump]).

EOF

# print out differences.
#if (-e ${argv[2]}_new) then
#	/bin/rm ${argv[2]}_new
#endif
#sort temp > ${argv[2]}_new

set d = `diff ${argv[2]}.O ${argv[2]}.O.old`
if ( $#d == 0 ) then 
	echo "$argv[2].O tested"
	\rm -f ${argv[2]}.O
else
	echo "$argv[2] different\!\!\!"
	diff ${argv[2]}.O ${argv[2]}.O.old
endif

set d = `diff ${argv[2]}.spec ${argv[2]}.spec.old`
if ( $#d == 0 ) then 
	echo "$argv[2].spec tested"
	\rm -f ${argv[2]}.spec
else
	echo "$argv[2] different\!\!\!"
	diff ${argv[2]}.spec ${argv[2]}.spec.old
endif

set d = `diff ${argv[2]}.table ${argv[2]}.table.old`
if ( $#d == 0 ) then 
	echo "$argv[2].table tested"
	\rm -f ${argv[2]}.table
else
	echo "$argv[2] different\!\!\!"
	diff ${argv[2]}.table ${argv[2]}.table.old
endif

set d = `diff ${argv[2]}.ti ${argv[2]}.ti.old`
if ( $#d == 0 ) then 
	echo "$argv[2].ti tested"
	\rm -f ${argv[2]}.ti
else
	echo "$argv[2] different\!\!\!"
	diff ${argv[2]}.ti ${argv[2]}.ti.old
endif
