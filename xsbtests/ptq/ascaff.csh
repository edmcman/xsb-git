#! /bin/csh
if (-e emu_s) then
	/usr/bin/rm -f emu_s
endif
if (-e emu_sc1) then
	/usr/bin/rm -f emu_sc1
endif
if (-e emu_sc2) then
	/usr/bin/rm -f emu_sc2
endif
if (-e emu_sc3) then
	/usr/bin/rm -f emu_sc3
endif
if (-e emu_sc4) then
	/usr/bin/rm -f emu_sc4
endif
if (-e emu_te1)	then
	/usr/bin/rm -f emu_te1
endif
if (-e emu_cn)	then
	/usr/bin/rm -f emu_cn
endif
if (-e emu_iv) then
	/usr/bin/rm -f emu_iv
endif
if (-e emu_s1) then
	/usr/bin/rm -f emu_s1
endif
if (-e emu_te11)	then
	/usr/bin/rm -f emu_te11
endif
if (-e emu_cn1)	then
	/usr/bin/rm -f emu_cn1
endif
if (-e emu_iv1) then
	/usr/bin/rm -f emu_iv1
endif
xp4 -x 1024 -i << EOF
[aptq,tables].
tran([john,dates,mary]).
output_table(s(_,_,_,_,_),emu_s).
output_table(sc1(_,_,_,_,_),emu_sc1).
output_table(sc2(_,_,_,_,_),emu_sc2).
output_table(sc3(_,_,_,_,_),emu_sc3).
output_table(sc4(_,_,_,_,_),emu_sc4).
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
if (-e emu_sc1) then
sort -u emu_sc1 > emu_sc11
endif
if (-e emu_sc2) then
sort -u emu_sc2 > emu_sc21
endif
if (-e emu_sc3) then
sort -u emu_sc3 > emu_sc31
endif
if (-e emu_sc4) then
sort -u emu_sc4 > emu_sc41
endif
if (-e emu_te1)	then
sort -u emu_te1 > emu_te11
endif
if (-e emu_cn)	then
sort -u emu_cn > emu_cn1
endif
