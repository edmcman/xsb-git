#-----------------------------------------------------------------------#
#
#	File		: Semantica/Makefile
#	Authors		: Kostis Sagonas
#	Last updated	: December 14, 1998
#
#-----------------------------------------------------------------------#

ALL =	sem_lex.O sem_parse.O sem_pe.O semantica.O sem_pretty.O sem_incl.O \
	sem_eval.O sem_dnf.O

Option = [optimize]

#-----------------------------------------------------------------------#
#
Prolog = xsb
#
#-----------------------------------------------------------------------#

# continue to the next when one error occurs
.IGNORE:

# suffix rules
.SUFFIXES: .P .O .H
.P.O:
	echo 'mc($*, $(Option)).' >> cmd...

all: $(ALL)
	touch cmd...
	$(Prolog) < cmd...
	/bin/rm -f cmd...

semtest:
	for file in `ls CSH/sem*.csh`; do csh $$file; done

.DONE:
	echo Done
