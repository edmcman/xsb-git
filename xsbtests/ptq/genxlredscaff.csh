#! /bin/csh

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
xp4 -m 8000 -c 4000 -i << EOF
[xoldt,xptqlred,tables].
assert(file_tab_print([f(s/5,${argv[2]}_s),f(te1/7,${argv[2]}_te1),f(cn/6,${argv[2]}_cn),f(iv/6,${argv[2]}_iv)])).
$argv[1].
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
