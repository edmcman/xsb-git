#!/bin/csh -f
#=============================================================================
#set $argv[1] = EMULATOR
#set $argv[2] = FILE_TO_CONSULT
#set $argv[3] = TEST_FILE
#set $argv[4] = PROLOG_COMMAND
#=============================================================================
echo "--------------------------------------------------------------------"
#util "Testing $argv[3]"
echo "Testing $argv[3]"
$argv[1] -m 3000 -i << EOF
[$argv[2]].
tell(temp).
$argv[4]
told.
EOF

# print out differences.
if (-e ${argv[3]}_new) then
	/bin/rm -f ${argv[3]}_new
endif
sort temp > ${argv[3]}_new

set d = `diff ${argv[3]}_new ${argv[3]}_old`
if ( $#d == 0 ) then 
	echo "$argv[3] tested"
	/bin/rm -f ${argv[3]}_new
else
	echo "$argv[3] different\!\!\!"
	diff ${argv[3]}_new ${argv[3]}_old
endif
/bin/rm -f temp
