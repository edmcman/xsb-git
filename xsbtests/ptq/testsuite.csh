if (!($?USER)) then
   setenv USER `whoami`
endif

if (-e testptq.out{$USER}) then
	/bin/rm -f testptq.out{$USER}
endif
csh testptq.csh $argv[1] > testptq.out{$USER}
grep \!\!\! testptq.out{$USER}
if ($status == 0) then
	mail $USER < testptq.out{$USER}
	exit
else
	/bin/rm -f testptq.out{$USER}
endif
cd '..'
if (-e testall.out{$USER}) then
	/bin/rm -f testall.out{$USER}
endif
#csh testall.csh $argv[1] > testall.out{$USER}
#grep \!\!\! testall.out{$USER}
#if ($status == 0) then
#	mail $USER < testall.out{$USER}
#else
#	/bin/rm -f testall.out{$USER}
#	mail -s "Testsuite result" $USER@cs.sunysb.edu << +
#Version passed current suite.
#`date`
#+
endif

