#-----------------------------------------------------------------------#
#
#	File		: Semantica/Makefile
#	Authors		: Kostis F. Sagonas
#	Last updated	: November 12, 1995
#
#-----------------------------------------------------------------------#

ALL =	sem_lex.O sem_parse.O sem_pe.O semantica.O sem_pretty.O sem_incl.O \
	sem_eval.O sem_dnf.O

Option = [optimize, no_check]

#-----------------------------------------------------------------------#
#
Prolog = xsb -i
#
#-----------------------------------------------------------------------#

# continue to the next when one error occurs
.IGNORE:

# suffix rules
.SUFFIXES: .P .O .H
.P.O:
	echo 'mc($*, $(Option)).' >> cmd...

all: $(ALL)
	$(Prolog) < cmd...
	/bin/rm -f cmd...
	o.diff

semtest:
	for file in `ls CSH/sem*.csh`; do csh $$file; done

.DONE:
	echo Done
