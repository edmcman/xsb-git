if (-e emu_s) then
	/bin/rm -f emu_s
endif
if (-e emu_te1)	then
	/bin/rm -f emu_te1
endif
if (-e emu_cn)	then
	/bin/rm -f emu_cn
endif
if (-e emu_iv) then
	/bin/rm -f emu_iv
endif
if (-e emu_s1) then
	/bin/rm -f emu_s1
endif
if (-e emu_te11)	then
	/bin/rm -f emu_te11
endif
if (-e emu_cn1)	then
	/bin/rm -f emu_cn1
endif
if (-e emu_iv1) then
	/bin/rm -f emu_iv1
endif
xp4 -X 1024 -i << EOF
[ptq,tables].
tran([john,dates,mary]).
output_table(s(_,_,_,_,_),emu_s).
output_table(te1(_,_,_,_,_,_,_),emu_te1).
output_table(cn(_,_,_,_,_,_),emu_cn).
output_table(iv(_,_,_,_,_,_),emu_iv).
halt.
EOF
if (-e emu_iv)	then
sort -u emu_iv > emu_iv1
endif
if (-e emu_s)	then
sort -u emu_s > emu_s1
endif
if (-e emu_te1)	then
sort -u emu_te1 > emu_te11
endif
if (-e emu_cn)	then
sort -u emu_cn > emu_cn1
endif
