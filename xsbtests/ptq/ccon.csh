#! /bin/csh
if (-e temu_s) then
	/bin/rm -f temu_s
endif
if (-e temu_s1) then
	/bin/rm -f temu_s1
endif
xp4 -m 2000 -c 2000 -x 1024 -i << EOF
[sptq,tables].
tran([john,dates,mary]).
output_table(s(_,_,_,_,_),temu_s).
halt.
EOF
if (-e temu_s)	then
sort -u temu_s > temu_s1
endif
diff emu_s1 temu_s1
