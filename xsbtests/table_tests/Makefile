#--------------------------------------------------------------------------
#
#	File			: XSB/emu/Makefile
#	Authors			: Jiyang Xu, Terrance Swift, Kostis Sagonas
#	Last updated		: February 3, 1997
#
#--------------------------------------------------------------------------

#
#=======================================================================
# XSB Version Date:  97-2-3 (but it has to be given in hexadecimal).
#=======================================================================
#
DATE = 0x031661
#
#=======================================================================
# Directory where XSB will be installed.
#=======================================================================
#
DIR =
#
#=======================================================================
# Name of the executable file that will be created by this make
#	We suggest the use of xsb.
#=======================================================================
#
EXECUTABLE =
#
#=======================================================================
# Architecture (Type of machine):
#	SUN, SOLARIS, NeXT, linux, SGI, HP300, DECstation, AMIGA, IBM,
#	SEQUENT (not tested yet), or SONY_NEWS.
#=======================================================================
#
MACHINE =
#
#=======================================================================
# C compiler option -- please choose one of the following two:
#       cc;
#	gcc
# and consult the debugging mode and the warning below.
#
# CCC is the name of the compiler to be used by XSB for the
# C interface.  Normally, it should have the same value as CC,
# but may differ when, for example, xsb is produced by a cross-compiler.
#=======================================================================
#
CC =
CCC =$(CC)
#
#=======================================================================
# C compiler specific options -- please choose one of the following two:
#    for gcc = -ansi -pedantic -Wall
#    for cc = -Xa
#=======================================================================
#
COPTS =
#
#=======================================================================
# Debugging mode -- please choose one of the following:
#	-DOLDTPROFILE; 
#	-O2 [-DTRACING];
#	-g [-DDEBUG] [-DTRACING];
#	-O2 -DPROFILE
# where the []s denote optionality.
#
# WARNING !!! 
#	When making the emulator with the "-O*" options the temporary
#	space available may not be sufficient. For example on a SPARC
#	that had very little /tmp space the "-O*" option could not be used
#	for the file "emuloop.c".  In your system depending on the amount
#	of /tmp space of your machine there may be other files or you
#	may not have any problems at all.
#	If you are using the SUN C compiler, and have disk space in one
#	of your directories, say "dir", add the following option to the
#	entries of the files that cannot be compiled:
#		-temp=dir
#	If you are using the Gnu C compiler, consult its manual pages
#	to find out how you can change the default tmp dir or how you
#	can use pipes to avoid the use of tmp space.
#=======================================================================
#
DBGFLAGS =
OPTFLAGS =-O4
LOWOPTFLAGS =-O4
EMUFLAGS =
#
#=======================================================================
# scheduling options -- right now you don't have a choice...
#=======================================================================
#
SCHEDTYPE =
SCHEDOPTS =
#
#=======================================================================
# All of the above can be summarized as follows.
#=======================================================================
#
# "Standard" flags
CFLAGS = $(COPTS) $(DBGFLAGS) $(OPTFLAGS) $(EMUFLAGS) -D$(MACHINE) $(SCHEDOPTS)
#
# Flags for files requiring lower optimization level
LCFLAGS = $(COPTS) $(DBGFLAGS) $(LOWOPTFLAGS) $(EMUFLAGS) -D$(MACHINE) $(SCHEDOPTS)
#
#=======================================================================
# Stuff to automatically make the dependencies.
#=======================================================================
#
DEPEND = makedepend
DEPENDFLAGS = -m -Y
#

#=======================================================================
# Put in SRCS all files that you want to run makedepend.
#=======================================================================

SRCS =	init.c emuloop.c debug.c subp.c loader.c dis.c inst.c system.c \
	auxlry.c memory.c load_seg.c trace.c psc.c token.c builtin.c \
	dynload.c cutils.c function.c cinterf.c xsberror.c biassert.c \
	tr_utils.c tries.c scc.c slgdelay.c residual.c \
	hash.c heap.c findall.c xmain.c

OBJS =	init.o emuloop.o debug.o subp.o loader.o dis.o inst.o system.o \
	auxlry.o memory.o load_seg.o trace.o psc.o token.o builtin.o \
	dynload.o cutils.o function.o cinterf.o xsberror.o biassert.o \
	tr_utils.o  tries.o scc.o slgdelay.o residual.o \
	hash.o heap.o findall.o

ORASRCS = $(SRCS) orastuff.c

ORAOBJS = $(OBJS) orastuff.o

ORACLE_LIBRARY_PATH = /usr/local/pkg/comdb/oracle/product/7.1.3/lib

ORACLE_LINK_OPTS = -L$(ORACLE_LIBRARY_PATH) -lsql \
   $(ORACLE_LIBRARY_PATH)/osntab.o -lsqlnet -lora -lsqlnet -lora \
   -lnlsrtl -lcv6 -lcore -lnlsrtl -lcv6 -lcore


#ORACLE_LIBRARY_PATH = /home/fs7/users/oracle/lib 
#ORACLE_LINK_OPTS = -L$(ORACLE_LIBRARY_PATH) -lsql \
#    $(ORACLE_LIBRARY_PATH)/osntab.o -lsqlnet -lsqlnet \
#    -lora -lsqlnet  -lnetwork -lcore


#
#=======================================================================
# Linking command:
# SUN		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm -N
# SOLARIS	: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm -lnsl -ldl
# NEXT		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# linux		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# SGI		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# HP300		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# DECstation	: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# AMIGA		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# IBM		: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
# SEQUENT	: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm -lpps
# SONY_NEWS	: $(CC) -o $(EXECUTABLE) xmain.o $(OBJS) -lm
#=======================================================================
#
LINKOPTION =
LINKFLAGS = 
$(EXECUTABLE):: xmain.o $(OBJS)
	$(CC) -o $@ $(LINKFLAGS) xmain.o $(OBJS) $(LINKOPTION)

xsbmod:: $(OBJS)
	ld -o xsbmod.o -r $(OBJS)

xsbora:: xmain.o $(ORAOBJS)
	$(CC) -o $(EXECUTABLE) xmain.o $(ORAOBJS) $(ORACLE_LINK_OPTS) \
           $(LINKOPTION)

depend::
	$(DEPEND) $(DEPENDFLAGS) -- $(CFLAGS) -- $(SRCS)

clean::
	$(RM) xmain.o $(OBJS)

#=======================================================================
# Put in this group all actions (rules) of this Makefile.
#=======================================================================

init.o: init.c
	$(CC) -c $(CFLAGS) -DDATE=$(DATE) -DDIR=\"$(DIR)\" -DCC=\"$(CCC)\" init.c

dynload.o: dynload.c
	$(CC) -c $(CFLAGS) -DEXECUTABLE=\"$(EXECUTABLE)\" dynload.c

emuloop.o: emuloop.c
	$(CC) -c $(LCFLAGS) emuloop.c

.c.o:
	$(CC) -c $(CFLAGS) $*.c

#=======================================================================
# All dependencies are automatically generated by makedepend.
#=======================================================================

# DO NOT DELETE THIS LINE -- make depend depends on it.

init.o: config.h auxlry.h cell.h xsberror.h inst.h psc.h hash.h memory.h
init.o: register.h tries.h choice.h flags.h loader.h xmacro.h slgdelay.h
init.o: tr_utils.h
emuloop.o: config.h auxlry.h cell.h register.h xsberror.h inst.h psc.h
emuloop.o: deref.h memory.h heap.h sig.h emudef.h loader.h binding.h flags.h
emuloop.o: tries.h choice.h xmacro.h slgdelay.h subinst.h scc.h subp.h
emuloop.o: tr_utils.h tr_delay.h tr_code.i schedrev.i slginsts.i complete.i
emuloop.o: tc_insts.i unify.i emusubs.i
debug.o: config.h auxlry.h cell.h xsberror.h binding.h psc.h memory.h flags.h
debug.o: register.h deref.h tries.h choice.h xmacro.h slgdelay.h inst.h
subp.o: config.h auxlry.h cell.h xsberror.h psc.h memory.h register.h heap.h
subp.o: deref.h flags.h binding.h tries.h choice.h token.h sig.h inst.h
subp.o: xmacro.h slgdelay.h unify.i sp_unify.i
loader.o: config.h auxlry.h psc.h loader.h flags.h cell.h load_seg.h tries.h
loader.o: xmacro.h slgdelay.h xsberror.h
dis.o: config.h auxlry.h psc.h hash.h loader.h cell.h inst.h memory.h flags.h
dis.o: tries.h xmacro.h slgdelay.h
inst.o: auxlry.h inst.h subinst.h
system.o: config.h cinterf.h msyscall.h
auxlry.o: config.h
memory.o: auxlry.h memory.h config.h cell.h register.h tries.h choice.h
memory.o: xsberror.h psc.h xmacro.h slgdelay.h flags.h subp.h
load_seg.o: config.h auxlry.h inst.h cell.h memory.h register.h psc.h
load_seg.o: loader.h tries.h xmacro.h slgdelay.h
trace.o: config.h auxlry.h inst.h cell.h memory.h register.h tries.h choice.h
trace.o: flags.h psc.h xmacro.h slgdelay.h
psc.o: config.h auxlry.h cell.h xsberror.h tries.h psc.h hash.h xmacro.h
psc.o: slgdelay.h loader.h flags.h inst.h memory.h
token.o: config.h auxlry.h token.h cell.h psc.h subp.h register.h
builtin.o: config.h auxlry.h cell.h xsberror.h psc.h hash.h tries.h choice.h
builtin.o: deref.h memory.h heap.h register.h flags.h loader.h load_seg.h
builtin.o: binding.h xmacro.h slgdelay.h token.h inst.h subinst.h sig.h
builtin.o: subp.h tr_utils.h trassert.h dynload.h cinterf.h residual.h
builtin.o: bineg.i std_pred.i
dynload.o: config.h
cutils.o: config.h auxlry.h cell.h xsberror.h register.h binding.h memory.h
cutils.o: tries.h choice.h deref.h psc.h xmacro.h slgdelay.h
function.o: config.h auxlry.h cell.h register.h memory.h deref.h
cinterf.o: config.h auxlry.h cell.h register.h psc.h flags.h deref.h heap.h
cinterf.o: binding.h tries.h choice.h subp.h emuloop.h cinterf.h
xsberror.o: config.h auxlry.h cell.h psc.h subp.h register.h xsberror.h
biassert.o: config.h auxlry.h cell.h xsberror.h cinterf.h memory.h psc.h
biassert.o: heap.h register.h flags.h inst.h token.h load_seg.h
trassert.o: config.h auxlry.h cinterf.h cell.h psc.h tries.h xmacro.h
trassert.o: slgdelay.h inst.h xsberror.h tr_utils.h
tr_utils.o: config.h auxlry.h cinterf.h binding.h cell.h psc.h memory.h
tr_utils.o: register.h deref.h tries.h xmacro.h slgdelay.h choice.h inst.h
tr_utils.o: xsberror.h
tries.o: config.h auxlry.h inst.h cell.h psc.h deref.h memory.h register.h
tries.o: binding.h tries.h xmacro.h slgdelay.h choice.h cinterf.h xsberror.h
tries.o: tr_utils.h
scc.o: config.h auxlry.h cell.h register.h psc.h tries.h xmacro.h slgdelay.h
slgdelay.o: config.h auxlry.h cell.h psc.h register.h tries.h memory.h
slgdelay.o: choice.h xmacro.h slgdelay.h tr_utils.h inst.h xsberror.h
residual.o: config.h auxlry.h cell.h psc.h register.h heap.h binding.h
residual.o: tries.h xmacro.h slgdelay.h
hash.o: auxlry.h hash.h psc.h flags.h
heap.o: config.h auxlry.h memory.h inst.h cell.h register.h tries.h choice.h
heap.o: xsberror.h psc.h xmacro.h slgdelay.h
findall.o: config.h auxlry.h deref.h cell.h register.h memory.h psc.h
findall.o: xsberror.h
xmain.o: emuloop.h
