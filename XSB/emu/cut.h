/* File:      cut.h
** Author(s): Kostis Sagonas
** Contact:   xsb-contact@cs.sunysb.edu
** 
** Copyright (C) The Research Foundation of SUNY, 1986, 1993-1998
** 
** XSB is free software; you can redistribute it and/or modify it under the
** terms of the GNU Library General Public License as published by the Free
** Software Foundation; either version 2 of the License, or (at your option)
** any later version.
** 
** XSB is distributed in the hope that it will be useful, but WITHOUT ANY
** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
** FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License for
** more details.
** 
** You should have received a copy of the GNU Library General Public License
** along with XSB; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** $Id: cut.h,v 1.1.1.1 1998-11-05 16:55:14 sbprolog Exp $
** 
*/


/*----------------------------------------------------------------------*/
/* This file contains code for cuts.  It was written in December 1997	*/
/* by Kostis Sagonas based on the code written by Terry Swift and Rui	*/
/* Marques for unwinding the trail.  The input of Bart Demoen is also	*/
/* acknowledged.							*/
/* 									*/
/* The main problem is to find which part of the trail to clean up.	*/
/* Cleaning the trail means deleting trail frames for bindings that are	*/
/* no longer conditional, i.e. unbinding these variables.  In general,	*/
/* one needs to tidy/unwind the part of the trail that lies between the	*/
/* current choice point (breg) and the choice point that is immediately	*/
/* _after_ the choice point that we are really cut back to (cut_breg).	*/
/* Of course, if we are cutting to the current choice point which often	*/
/* happens, or if there is nothing in between, there is no need to	*/
/* check whether there is trail to unwind.				*/
/* This is the general scheme for Prolog and is more or less the easy	*/
/* case.  The code that pretty much implements this, is as follows:	*/
/*									*/
/*	#define cut_code(OP1)						*/
/*	    deref(OP1);							*/
/*	    cut_breg = (CPtr)(tcpstack.high - int_val(OP1));		*/
/*	    cut_restore_trail_condition_registers(cut_breg);		*/
/*	    if (breg != cut_breg) {					*/
/*		while (cp_prevbreg(breg) != cut_breg)			*/
/*		    breg = cp_prevbreg(breg);				*/
/*		unwind_trail(breg,xtemp1,xtemp2);			*/
/*		breg = cut_breg;					*/
/*	    } goto contcase;						*/
/*									*/
/* However, in tabled execution, cuts are slightly more complicated	*/
/* than in Prolog.  It might be the case, for example, that breg and	*/
/* cut_breg do not lie on the same execution branch (this happens, but 	*/
/* very rarely).  In such cases, the following code might unnecessarily	*/
/* scan trail entries, since I do not see an easy way of limiting the	*/
/* part of the trail that we are really cutting... so I chose to retain	*/
/* the functionality of XSB 1.7.2.  As I said, such cases happen very 	*/
/* very rarely (only 3 cases in the whole testsuite) so I would not	*/
/* lose my sleep over it...  I *believe* that the following works for	*/
/* Prolog code the way it really should.		- Kostis.	*/
/*----------------------------------------------------------------------*/

#ifdef DEBUG
#define CHECK_TABLE_CUT()\
    if( *breg == (Cell) &retry_active_inst ||                               \
        *breg == (Cell) &completion_suspension_inst || *breg <= 2 )         \
    {   fprintf( stderr, "warning: cutting over non prolog CP: @inst %d: ", \
                 xctr ) ;                                                   \
        if( *breg == (Cell) &retry_active_inst )                            \
            fprintf( stderr, "retry_active node\n" );                       \
        else if( *breg == (Cell) &completion_suspension_inst )              \
            fprintf( stderr, "completion_suspension_inst node\n" );         \
        else if( *breg <= 2 )                                               \
            fprintf( stderr, "negation stuff\n" );                          \
        fprintf( stderr, "b=%p bf=%p tr=%p trf=%p, cut to %p\n",            \
                 breg, bfreg, trreg, trfreg, cut_breg ) ;                   \
                break ;                                                     \
    }
#else
#define CHECK_TABLE_CUT()
#endif

#define cut_code(OP1)	\
   { CPtr cut_breg;					\
								\
     deref(OP1);						\
     cut_breg = (CPtr)(tcpstack.high - int_val(OP1));		\
     cut_restore_trail_condition_registers(cut_breg);		\
     if (breg != cut_breg) { /* not cutting back to the current CP */\
/*      fprintf(stderr,						\
           "Tidying trail (cutbreg = %p, breg = %p)\n", cut_breg,breg); */\
	while (cp_prevbreg(breg) < cut_breg) {			\
           CHECK_TABLE_CUT() ;                                  \
	   breg = cp_prevbreg(breg);				\
	}							\
	if (breg > cut_breg) breg = cut_breg; /* happens rarely */\
        unwind_trail(breg,xtemp1,xtemp2);			\
	breg = cut_breg;					\
     }								\
     goto contcase;						\
   }

/*----------------------------------------------------------------------*/
/* Takes a pointer to the choice point frame we are cutting back to.	*/
/*----------------------------------------------------------------------*/

#define cut_restore_trail_condition_registers(CUTB) \
    if ((CPtr)  *CUTB >= (CPtr) pdl.low || \
		*CUTB == (Cell) &retry_active_inst || \
		*CUTB == (Cell) &completion_suspension_inst) { \
	ebreg = cp_ebreg(CUTB); \
	hbreg = cp_hreg(CUTB); \
    }

/*----------------------------------------------------------------------*/
/* Deletes all trail frames that are no longer conditional.		*/
/* Most probably, it does *NOT* work for cuts over tables!!		*/
/*----------------------------------------------------------------------*/

#define trail_parent(t)         ((CPtr *)*(t))
#define trail_value(t)          ((CPtr *)*((t)-1))
#define trail_variable(t)       ((CPtr *)*((t)-2))
#define good_trail_register(t)	(conditional(((CPtr) *((t)-2))))

#define unwind_trail(tbreg, t1, t2) {	\
    while (!good_trail_register(trreg) &&				\
	   trreg > trfreg &&						\
	   trreg > cp_trreg(tbreg))					\
      trreg = trail_parent(trreg);					\
    (t2) = (CPtr) trail_parent((t1) = (CPtr)trreg);			\
    while ((t2) > (CPtr) cp_trreg(tbreg) && (t2) > (CPtr) trfreg) {	\
      if (!good_trail_register(t2)) {					\
	(t2) = (CPtr) trail_parent(t2);					\
	*(t1) = (Cell) (t2);						\
      } else {								\
	(t1) = (t2);							\
	(t2) = (CPtr) trail_parent(t2);					\
      }									\
    }									\
  }

