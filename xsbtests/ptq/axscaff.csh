if (-e int_s) then
	/bin/rm -f int_s
endif
if (-e int_sc1) then
	/bin/rm -f int_sc1
endif
if (-e int_sc2) then
	/bin/rm -f int_sc2
endif
if (-e int_sc3) then
	/bin/rm -f int_sc3
endif
if (-e int_sc4) then
	/bin/rm -f int_sc4
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
[xoldt,axptq,tables].
assert(file_tab_print([f(s/5,int_s),f(sc1/5,int_sc1),f(sc2/5,int_sc2),f(sc3/5,int_sc3),f(sc4/5,int_sc4),f(te1/7,int_te1),f(cn/6,int_cn),f(iv/6,int_iv)])).
tran([john,dates,mary]).
halt.
EOF
if (-e int_iv)	then
sort -u int_iv > int_iv1
endif
if (-e int_s)	then
sort -u int_s > int_s1
endif
if (-e int_sc1)	then
sort -u int_sc1 > int_sc11
endif
if (-e int_sc2)	then
sort -u int_sc2 > int_sc21
endif
if (-e int_sc3)	then
sort -u int_sc3 > int_sc31
endif
if (-e int_sc4)	then
sort -u int_sc4 > int_sc41
endif
if (-e int_te1)	then
sort -u int_te1 > int_te11
endif
if (-e int_cn)	then
sort -u int_cn > int_cn1
endif
