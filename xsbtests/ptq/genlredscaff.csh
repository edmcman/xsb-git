#! /bin/csh

#set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
set XEMU = $argv[3]
if (-e ${argv[2]}_s) then
	/bin/rm -f ${argv[2]}_s
endif
if (-e ${argv[2]}_te1)	then
	/bin/rm -f ${argv[2]}_te1
endif
if (-e ${argv[2]}_cn)	then
	/bin/rm -f ${argv[2]}_cn
endif
if (-e ${argv[2]}_iv) then
	/bin/rm -f ${argv[2]}_iv
endif
if (-e ${argv[2]}_s1) then
	/bin/rm -f ${argv[2]}_s1
endif
if (-e ${argv[2]}_te11)	then
	/bin/rm -f ${argv[2]}_te11
endif
if (-e ${argv[2]}_cn1)	then
	/bin/rm -f ${argv[2]}_cn1
endif
if (-e ${argv[2]}_iv1) then
	/bin/rm -f ${argv[2]}_iv1
endif
$XEMU -m 4000 -c 2000 -i << EOF
[ptqlred,tables].
$argv[1].
output_table(s(_,_,_,_,_),${argv[2]}_s).
output_table(te1(_,_,_,_,_,_,_),${argv[2]}_te1).
output_table(cn(_,_,_,_,_,_),${argv[2]}_cn).
output_table(iv(_,_,_,_,_,_),${argv[2]}_iv).
halt.
EOF
if (-e ${argv[2]}_iv)	then
sort -u ${argv[2]}_iv > ${argv[2]}_iv1
endif
if (-e ${argv[2]}_s)	then
sort -u ${argv[2]}_s > ${argv[2]}_s1
endif
if (-e ${argv[2]}_te1)	then
sort -u ${argv[2]}_te1 > ${argv[2]}_te11
endif
if (-e ${argv[2]}_cn)	then
sort -u ${argv[2]}_cn > ${argv[2]}_cn1
endif
