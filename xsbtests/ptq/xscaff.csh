#! /bin/csh

if (-e int_s) then
	/bin/rm -f int_s
endif
if (-e int_te1)	then
	/bin/rm -f int_te1
endif
if (-e int_cn)	then
	/bin/rm -f int_cn
endif
if (-e int_iv) then
	/bin/rm -f int_iv
endif
if (-e int_s1) then
	/bin/rm -f int_s1
endif
if (-e int_te11)	then
	/bin/rm -f int_te11
endif
if (-e int_cn1)	then
	/bin/rm -f int_cn1
endif
if (-e int_iv1) then
	/bin/rm -f int_iv1
endif
xp4 -x 1024 -i << EOF
[xoldt,xptq,tables].
assert(file_tab_print([f(s/5,int_s),f(te1/7,int_te1),f(cn/6,int_cn),f(iv/6,int_iv)])).
tran([john,dates,mary]).
halt.
EOF
if (-e int_iv)	then
sort -u int_iv > int_iv1
endif
if (-e int_s)	then
sort -u int_s > int_s1
endif
if (-e int_te1)	then
sort -u int_te1 > int_te11
endif
if (-e int_cn)	then
sort -u int_cn > int_cn1
endif
