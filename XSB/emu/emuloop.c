/* File:      emuloop.c
** Author(s): Warren, Swift, Xu, Sagonas, Johnson
** Contact:   xsb-contact@cs.sunysb.edu
** 
** Copyright (C) The Research Foundation of SUNY, 1986, 1993-1998
** Copyright (C) ECRC, Germany, 1990
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
** $Id: emuloop.c,v 1.1.1.1 1998-11-05 16:55:15 sbprolog Exp $
** 
*/


#include <stdio.h>
#include <stdlib.h>

#ifdef FOREIGN
#ifndef SOLARIS
#include <sys/un.h>
#endif
#endif

#include "configs/config.h"
#include "debugs/debug.h"

#include "auxlry.h"
#include "cell.h"
#include "register.h"
#include "xsberror.h"
#include "inst.h"
#include "psc.h"
#include "deref.h"
#include "memory.h"
#include "heap.h"
#include "sig.h"
#include "emudef.h"
#include "loader.h"
#include "binding.h"
#include "flags.h"
#include "tries.h"
#include "choice.h"
#include "xmacro.h"
#include "subinst.h"
#include "scc.h"
#include "subp.h"
#include "tr_utils.h"
#include "cut.h"

Cell	CallNumVar;
NODEptr TrieRetPtr;
ALPtr   OldRetPtr;
CPtr	VarPosReg;

extern tab_inf_ptr UglyHackForTip;

/*----------------------------------------------------------------------*/

#include "tr_delay.h"
#include "tr_code.i"

/*----------------------------------------------------------------------*/

#define pad		(lpcreg++)
#define ppad		(lpcreg+=2)
#define pppad		(lpcreg+=3)
#define opregaddr	(rreg+(*lpcreg++))
#define opvaraddr	(ereg-(Cell)(*lpcreg++))

#ifdef BITS64
#define pad64		(lpcreg += 4)
#else
#define pad64
#endif

#define opreg		cell(opregaddr)
#define opvar		cell(opvaraddr)
#define op1byte		op1 = (Cell)(*lpcreg++)
#define op2word		op2 = (Cell)(*(CPtr)lpcreg); lpcreg+=sizeof(Cell)
#define op3word		op3 = *(CPtr)lpcreg; lpcreg+=sizeof(Cell)

#define ADVANCE_PC	(lpcreg+=sizeof(Cell))

/* Be sure that flag only has the following two values.	*/

#define WRITE		1
#define READFLAG	0

/*----------------------------------------------------------------------*/
/* The following macros work for all CPs.  Make sure this remains	*/
/* the case...								*/
/*----------------------------------------------------------------------*/

#define Fail1 lpcreg = cp_pcreg(breg);

#define restore_trail_condition_registers(breg) \
      if (*breg != (Cell) &check_complete_inst) { \
	ebreg = cp_ebreg(breg); \
	hbreg = cp_hreg(breg); \
      } 

/*----------------------------------------------------------------------*/

extern int  builtin_call(int), unifunc_call(int, CPtr);

#ifdef DEBUG
extern void debug_inst(byte *, CPtr);
extern void print_subgoal(FILE *, SGFrame);
extern void print_delay_list(FILE *, CPtr);
#endif

int  (*dyn_pred)();

bool    neg_delay;
int     num_unwinds = 0;
int     xwammode, level_num, xctr;
CPtr    xcurcall, xtemp3, xtemp5, xtemp6, xtemp12, hreg1, xtemp14;
SGFrame xtemp15;

/*----------------------------------------------------------------------*/

#include "schedrev.i"
#ifndef LOCAL_EVAL 
#include "wfs.i" 
#endif 

/*----------------------------------------------------------------------*/

static int emuloop(byte *);

/*======================================================================*/
/*======================================================================*/

int xsb(int flag, int argc, char *argv[])
{ 
   char *startup_file;
   FILE *fd;
   Cell magic;
   char message[256];
   static double realtime;	/* To retain its value across invocations */

   extern void dis(int);
   extern char *init_para(int, char **);
   extern void init_flags(), init_machine(), init_symbols();
#ifdef FOREIGN
#ifndef FOREIGN_ELF
   extern char tfile[];
#endif
#endif

   if (flag == 0) {  /* initialize xsb */
     realtime = real_time();
     setbuf(stdout, NULL);
     init_flags();
     startup_file = init_para(argc, argv);	/* init parameters */
     init_machine();		/* init space, regs, stacks */
     init_inst_table();		/* init table of instruction types */
     init_symbols();		/* preset a few symbols in PSC table */
     init_interrupt();		/* catch ^C interrupt signal */

     fd = fopen(startup_file, "rb");   /* "b" needed for DOS. -smd */
     if (!fd) {
       sprintf(message, "The startup file, %s, could not be found!",
	       startup_file);
       xsb_exit(message);
     }
/*
     fread(&magic, 4, 1, fd);
     fix_bb(&magic);
*/
     get_obj_word_bb(&magic);
     fclose(fd);
     if (magic == 0x11121304)
       inst_begin = loader(startup_file,0);
     else
       xsb_exit("Incorrect startup file format");

     if (!inst_begin)
       xsb_exit("Error in loading startup file");

     if ( xsb_mode == DISASSEMBLE ) {
       dis(1);
       exit(0);
     }

     return(0);

   } else if (flag == 1) {  /* continue execution */

     return(emuloop(inst_begin));

   } else if (flag == 2) {  /* shutdown xsb */

#ifdef FOREIGN
#ifndef FOREIGN_ELF
     if (fopen(tfile, "r")) unlink(tfile);
#endif
#endif

     if ( xsb_mode != C_CALLING_XSB ) {
       realtime = real_time() - realtime;
       fprintf(stderr, "\nEnd XSB (cputime %.2f secs, elapsetime ",
	       cpu_time());
       if (realtime < 600.0)
	 fprintf(stderr, "%.2f secs)\n", realtime);
       else
	 fprintf(stderr, "%.2f mins)\n", realtime/60.0);
     }
     return(0);
   }
   return(1);
}  /* end of xsb() */

/*======================================================================*/
/* the main emulator loop.						*/
/*======================================================================*/

static int emuloop(byte *startaddr)
{
  Psc psc;
  register byte *lpcreg;
  register CPtr rreg;	/* for SUN */
  register Cell op1, op2;	/* (*Cptr) */
  CPtr op3;
  byte flag = READFLAG;  	/* read/write mode flag */
  Cell opa[3];
  
  int i, j, arity;	/* to unify subfields of op1 and op2 */  
  
  int restore_type;	/* 0 for retry restore; 1 for trust restore */
  
  int xflag;
  register CPtr xtemp1, xtemp2;
  CPtr xtemp4;	/* Temporary variable for breg (used in slginsts.i) */
#ifdef LOCAL_EVAL
  CPtr xtemp6=0;  /* Temporary variable for next breg (used in complete.i) */
#endif
  CPtr xtemp9;	/* Temporary variable for ebreg (used in slginsts.i) */
  char message[80];
  
  rreg = reg; /* for SUN */
  op1 = op2 = (Cell) NULL;
  lpcreg = startaddr;

 contcase:	/* the main loop */

#ifdef DEBUG
  if (flags[PIL_TRACE]) debug_inst(lpcreg, ereg);
  xctr++;
#ifdef STACKS_DEBUG
    if ((pb)ereg < (pb)hreg + OVERFLOW_MARGIN/4 ||
        (pb)efreg < (pb)hreg + OVERFLOW_MARGIN/4 ||
        (pb)ebreg < (pb)hreg + OVERFLOW_MARGIN/4) {
      fprintf(stderr,
              "! Detected global stack overflow in the EMULOOP\n");
      local_global_exception(lpcreg);
    }
#endif
#endif
#ifdef PROFILE
  if (flags[PROFFLAG]) {
    inst_table[(int) *(lpcreg)][sizeof(Cell)+1]
      = inst_table[(int) *(lpcreg)][sizeof(Cell)+1] + 1;
    if (flags[PROFFLAG] > 1 && (int) *lpcreg == builtin) 
      builtin_table[(int) *(lpcreg+3)][1] = 
	builtin_table[(int) *(lpcreg+3)][1] + 1;
  }
#endif
  
  switch (*lpcreg++) {
    
  case getpvar:  /* PVR */
    pad;
    op1 = (Cell)(opvaraddr);
    /* tls 12/8/92 */
    bld_copy0((CPtr)op1, opreg);
    pad64;
    goto contcase;
    
  case getpval: /* PVR */
    pad; op1 = opvar; op2 = opreg;
    pad64;
    goto nunify;

  case getstrv: /* PPV-S */
    ppad; op1 = opvar; pad64; op2word;
    nunify_with_str(op1,op2);
    goto contcase;

  case gettval: /* PRR */
    pad; op1 = opreg; op2 = opreg;
    pad64;
    goto nunify;

  case getcon: /* PPR-C */
    ppad; op1 = opreg; pad64; op2word;
    nunify_with_con(op1,op2);
    goto contcase;

  case getnil: /* PPR */
    ppad; op1 = opreg;
    pad64;
    nunify_with_nil(op1);
    goto contcase;	

  case getstr: /* PPR-S */
    ppad; op1 = opreg; pad64; op2word;
    nunify_with_str(op1,op2);
    goto contcase;

  case getlist: /* PPR */
    ppad; op1 = opreg;
    pad64;
    nunify_with_list_sym(op1);
    goto contcase;

/* tls 12/8/92 */
  case unipvar: /* PPV */
    ppad; op1 = (Cell)(opvaraddr);
    pad64;
    if (flag) {	/* if (flag == WRITE) */
	bind_ref((CPtr)op1, hreg);
	new_heap_free(hreg);
    }
    else {
	bld_copy0((CPtr)op1, *(sreg++));
    }
    goto contcase;

  case unipval: /* PPV */
    ppad; op1 = opvar;
    pad64;
    if (flag) { /* if (flag == WRITE) */
       nbldval(op1); 
      } 
    else {
      op2 = *(sreg++);
      goto nunify;
    } 
    goto contcase;

 case unitvar: /* PPR */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    if (flag) {	/* if (flag == WRITE) */
	bld_ref((CPtr)op1, hreg);
	new_heap_free(hreg);
    }
    else {
	bld_copy0((CPtr)op1, *(sreg++));
        /* * (CPtr) op1 = *(sreg++) */
    }
    goto contcase;

 case unitval: /* PPR */
    ppad; op1 = opreg;
    pad64;
    if (flag) { /* if (flag == WRITE) */
      nbldval(op1); 
      goto contcase;
    }
    else {
	op2 = *(sreg++);
	goto nunify;
    } 


 case unicon: /* PPP-C */
    pppad; pad64; op2word;
    if (flag) {	/* if (flag == WRITE) */
	new_heap_string(hreg, (char *)op2);
    }
    else {  /* op2 already set */
	op1 = *(sreg++);
	nunify_with_con(op1,op2);
    }
    goto contcase;

 case uninil: /* PPP */
    pppad;
    pad64;
    if (flag) {	/* if (flag == WRITE) */
	new_heap_nil(hreg);
    }
    else {
	op1 = *(sreg++);
	nunify_with_nil(op1);
    }
    goto contcase;

 case getnumcon: /* PPR-N */
    ppad; op1 = opreg; pad64; op2word;
    nunify_with_num(op1,op2);
    goto contcase;

 case getfloat: /* PPR-N */
    ppad; op1 = opreg; pad64; op2word;
    nunify_with_float(op1,op2);
    goto contcase;

 case putnumcon: /* PPR-N */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    op2 = *(pw)lpcreg; ADVANCE_PC;
    bld_int((CPtr)op1, op2);
    goto contcase;

 case putfloat: /* PPR-N */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_float((CPtr)op1, asfloat(*(pw)lpcreg));
    ADVANCE_PC;
    goto contcase;

 case putpvar: /* PVR */
    pad;
    op1 = (Cell)(opvaraddr);
    bld_free((CPtr)op1);
    op2 = (Cell)(opregaddr);
    pad64;
    bld_ref((CPtr)op2, (CPtr)op1);
    goto contcase;

 case putpval: /* PVR */
    pad; op1 = (Cell)(opvaraddr);
    bld_copy0(opregaddr, *((CPtr)op1));
    pad64;
    goto contcase;

 case puttvar: /* PRR */
    pad; op1 = (Cell)(opregaddr); op2 = (Cell)(opregaddr);
    pad64;
    bld_ref((CPtr)op1, hreg);
    bld_ref((CPtr)op2, hreg);
    new_heap_free(hreg); 
    goto contcase;

/* tls 12/8/92 */
 case putstrv: /*  PPV-S */
    ppad; op1 = (Cell)(opvaraddr);
    pad64;
    bind_cs((CPtr)op1, (Pair)hreg);
    new_heap_functor(hreg, *(Psc *)lpcreg); ADVANCE_PC;
    goto contcase;

 case putcon: /* PPR-C */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_string((CPtr)op1, *(char **)lpcreg); ADVANCE_PC;
    goto contcase;
    
 case putnil: /* PPR */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_nil((CPtr)op1);
    goto contcase;
    
/* doc tls -- differs from putstrv since it pulls from a register */
 case putstr: /* PPR-S */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_cs((CPtr)op1, (Pair)hreg);
    new_heap_functor(hreg, *(Psc *)lpcreg); ADVANCE_PC;
    goto contcase;

 case putlist: /* PPR */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_list((CPtr)op1, hreg);
    goto contcase;

 case bldpvar: /* PPV */
    ppad; op1 = (Cell)(opvaraddr);
    pad64;
    /* tls 12/8/92 */
    bind_ref((CPtr)op1, hreg);
    new_heap_free(hreg);
    goto contcase;
    
 case bldpval: /* PPV */
    ppad; op1 = opvar;
    pad64;
    nbldval(op1);
    goto contcase;
    
 case bldtvar: /* PPR */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_ref((CPtr)op1, hreg);
    new_heap_free(hreg);
    goto contcase;
    
 case bldtval: /* PPR */
    ppad; op1 = opreg;
    pad64;
    nbldval(op1);
    goto contcase;
    
 case bldcon: /* PPP-C */
    pppad;
    pad64;
    new_heap_string(hreg, *(char **)lpcreg);
    ADVANCE_PC;
    goto contcase;
    
 case bldnil: /* PPP */
    pppad;
    pad64;
    new_heap_nil(hreg);
    goto contcase;
    
 case getlist_tvar_tvar: /* RRR */
    /* op1 is FREE: change tls for value trail */
    op1 = opreg;
    deref(op1);
    if (isref(op1)) {
      bind_list((CPtr)(op1), hreg);
      op1 = (Cell)(opregaddr);
      bld_ref((CPtr)op1, hreg);
      new_heap_free(hreg);
      op1 = (Cell)(opregaddr);
      pad64;
      bld_ref((CPtr)op1, hreg);
      new_heap_free(hreg);
    } else if (islist(op1)) {
      sreg = clref_val(op1);
      op1 = (Cell)(opregaddr);
      bld_ref((CPtr)op1, *(sreg));
      op1 = (Cell)(opregaddr);
      pad64;
      bld_ref((CPtr)op1, *(sreg+1));
    }
    else Fail1;
    goto contcase;	/* end getlist_tvar_tvar */

/* In the following two instructions, I replaced `comma_psc' with a lookup
   of ','/2 in standard.  -EJJ 4/97 */
 case getcomma: /* PPR */
    ppad; op1 = opreg;
    pad64;
    psc = pair_psc(insert_module(0, "standard"));
    op2 = (Cell)pair_psc(insert(",", 2, psc, &xflag));
    nunify_with_str(op1,op2);
    goto contcase;

/* Old:
    ppad; op1 = opreg;
    op2 = (Cell)(comma_psc);
    goto nunify_with_str;
*/

 case getcomma_tvar_tvar: /* RRR */
    op1 = opreg;
    psc = pair_psc(insert_module(0, "standard"));
    psc = pair_psc(insert(",", 2, psc, &xflag));
    /* op1 is FREE: */
    deref(op1);
    if (isref(op1)) {
      bind_cs((CPtr)(op1), (Pair)hreg);
      new_heap_functor(hreg, psc);
      op1 = (Cell)(opregaddr);
      bld_ref((CPtr)op1, hreg);
      new_heap_free(hreg);
      op1 = (Cell)(opregaddr);
      pad64;
      bld_ref((CPtr)op1, hreg);
      new_heap_free(hreg);
    } else if (isconstr(op1)) {	/* or DELAY */
      /* tls: clref_val = constant list ref val. Untags the word */
      op2 = (Cell)(clref_val(op1));
      if (((Pair)(CPtr)op2)->psc_ptr == psc) {
	sreg = (CPtr)op2 + 1;
	op1 = (Cell)(opregaddr);
	bld_ref((CPtr)op1, *(sreg)); sreg++;
	op1 = (Cell)(opregaddr);
        pad64;
	bld_ref((CPtr)op1, *(sreg));
      }
    }
    else Fail1;
    goto contcase;	/* end getcomma_tvar_tvar */

 case uninumcon: /* PPP-N */
    pppad; pad64; op2word; /* num in op2 */
    if (flag) {	/* if (flag == WRITE) */
	new_heap_num(hreg, (Integer)op2);
    }
    else {  /* op2 set */
	op1 = *(sreg++);
	nunify_with_num(op1,op2);
    }
    goto contcase;

 case unifloat: /* PPPF */
    pppad; pad64; op2word; /* num in op2 */
    if (flag) {	/* if (flag == WRITE) */
	new_heap_float(hreg, asfloat(op2));
    }
    else {  /* op2 set */
	op1 = cell(sreg++);
        nunify_with_float(op1,op2);
    }
    goto contcase;
    
 case bldnumcon: /* PPP-N */
    pppad; pad64; op2word; /* num to op2 */
    new_heap_num(hreg, (Integer)op2);
    goto contcase;

 case bldfloat: /* PPP-F */
    pppad; pad64; op2word; /* num to op2 */
    new_heap_float(hreg, asfloat(op2));
    goto contcase;

 case trymeelse: /* PPA-L */
    ppad; op1byte; pad64; op2word;
    goto subtryme;

 case retrymeelse: /* PPA-L */
    ppad; op1byte;
    pad64;
    cp_pcreg(breg) = *(byte **)lpcreg;
    ADVANCE_PC;
    restore_type = 0;
    goto restore_sub;

 case trustmeelsefail: /* PPA */
    ppad; op1byte;
    pad64;
    restore_type = 1;
    goto restore_sub;

 case try: /* PPA-L */
    ppad; op1byte;
    pad64;
    op2 = (Cell)((Cell)lpcreg + sizeof(Cell));
    lpcreg = *(pb *)lpcreg; /* = *(pointer to byte pointer ) */
    goto subtryme;

 case retry: /* PPA-L */
    ppad; op1byte;
    pad64;
    cp_pcreg(breg) = lpcreg+sizeof(Cell);
    lpcreg = *(pb *)lpcreg;
    restore_type = 0;
    goto restore_sub;

 case trust: /* PPA-L */
    ppad; op1byte;
    pad64;
    lpcreg = *(pb *)lpcreg;
    restore_type = 1;
    goto restore_sub;

 case getpbreg: /* PPV */
    ppad; op1 = (Cell)(opvaraddr);
    pad64;
    bld_int((CPtr)op1, ((pb)tcpstack.high - (pb)breg));
    goto contcase;

 case gettbreg: /* PPR */
/* doc tls op1 = rreg+*lpcreg++ */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_int((CPtr)op1, ((pb)tcpstack.high - (pb)breg));
    goto contcase;

 case putpbreg: /* PPV */
    ppad; op1 = opvar;
    pad64;
    cut_code(op1);

 case puttbreg: /* PPR */
    ppad; op1 = opreg;
    pad64;
    cut_code(op1);

/* also see arith_exception in subp.c -- tls */

 case jumptbreg: /* PPR-L */	/* ??? */
    ppad; op1 = (Cell)(opregaddr);
    pad64;
    bld_int((CPtr)op1, ((pb)tcpstack.high - (pb)breg));
    lpcreg = *(byte **)lpcreg;
    goto contcase;

 case getarg_proceed: /* PPA */
    ppad; op1byte;
    pad64;
    lpcreg = cpreg;
    goto contcase;

 case switchonterm: /* PPR-L-L */
    ppad; 
    op1 = opreg;
    pad64;
    free_deref(op1);
    switch (cell_tag(op1)) {
    case FREE:
    case REF1: 
      lpcreg += 2 * sizeof(Cell);
      break;
      /* case DELAY: */
    case INT:
    case STRING:
    case FLOAT:
      lpcreg = *(pb *)lpcreg;	    
      break;
    case CS:
      if (get_arity(get_str_psc(op1)) == 0) {
	lpcreg = *(pb *)lpcreg;
	break;
      }
    case LIST:	/* include structure case here */
      lpcreg += sizeof(Cell); lpcreg = *(pb *)lpcreg; 
      break;
    }
    goto contcase;

    case switchonbound: /* PPR-L-L */
      /* op1 is register, op2 is hash table offset, op3 is modulus */
      ppad; 
      op1 = opreg;
      pad64;
      free_deref(op1);
      switch (cell_tag(op1)) {
      case FREE:
      case REF1: 
	lpcreg += 2 * sizeof(Cell);
	goto sotd2;
	/* case DELAY: */
      case INT: 
      case FLOAT:	/* Yes, use int_val to avoid conversion problem */
	op1 = (Cell)int_val(op1);
	break;
      case LIST:
	op1 = (Cell)(list_str); 
	break;
      case CS:
	op1 = (Cell)get_str_psc(op1);
	break;
      case STRING:	/* We should change the compiler to avoid this test */
	op1 = (Cell)(isnil(op1) ? 0 : string_val(op1));
	break;
      }
      op2 = (Cell)(*(byte **)(lpcreg)); lpcreg += sizeof(Cell);
      op3 = *(CPtr *)lpcreg;
      /* doc tls -- op2 + (op1%size)*4 */
      lpcreg =
	*(byte **)((byte *)op2 + ihash((Cell)op1, (Cell)op3) * sizeof(Cell));
    sotd2: goto contcase;
      
    case switchon3bound: /* RRR-L-L */
      /* op1 is register, op2 is hash table offset, op3 is modulus */
      if (*lpcreg == 0) { lpcreg++; opa[0] = 0; }
      else opa[0] = (Cell)opreg;
      opa[1] = (Cell)opreg;
      opa[2] = (Cell)opreg;
      pad64;
      op2 = (Cell)(*(byte **)(lpcreg)); lpcreg += sizeof(Cell);
      op3 = *(CPtr *)lpcreg; 
      /* This is not a good way to do this, but until we put retract into C,
	 or add new builtins, it will have to do. */
      j = 0;
      for (i = 0; i <= 2; i++) {
	if (opa[i] != 0) {
	  op1 = opa[i];
	  free_deref(op1);
	  switch (cell_tag(op1)) {
	  case FREE:
	  case REF1: 
	    lpcreg += sizeof(Cell);
	    goto sob3d2;
	  case INT: 
	  case FLOAT:	/* Yes, use int_val to avoid conversion problem */
	    op1 = (Cell)int_val(op1);
	    break;
	  case LIST:
	    op1 = (Cell)(list_str); 
	    break;
	  case CS:
	    op1 = (Cell)get_str_psc(op1);
	    break;
	  case STRING:
	    op1 = (Cell)string_val(op1);
	    break;
	  default:
	    fprintf(stderr,"Illegal operand in switchon3bound\n");
	    break;
	  }
	  j = (j<<1) + ihash((Cell)op1, (Cell)op3);
	}
      }
      lpcreg = *(byte **)((byte *)op2 + ((j % (Cell)op3) * sizeof(Cell)));
    sob3d2: goto contcase;

 case switchoncon: pppad; pad64; op2word;
    xsb_exit("Switchoncon not implemented");
    goto contcase;

 case switchonstr: pppad; op2word;
    xsb_exit("Switchonstr not implemented");
    goto contcase;

 case trymeorelse: /* PPA-L */
    pppad;
    pad64;
    op1 = 0;
    op2word;
    cpreg = lpcreg;
    goto subtryme;

 case retrymeorelse: /* PPA-L */
    pppad;
    pad64;
    op1 = 0;
    cp_pcreg(breg) = *(byte **)lpcreg;
    ADVANCE_PC;
    cpreg = lpcreg;
    restore_type = 0;
    goto restore_sub;

 case trustmeorelsefail: /* PPA */
    pppad;
    pad64;
    op1 = 0;
    cpreg = lpcreg+sizeof(Cell);
    restore_type = 1;
    goto restore_sub;

 case dyntrustmeelsefail: /* PPA-L, second word ignored */
    ppad; op1byte; 
    pad64;
    ADVANCE_PC;
    restore_type = 1;
    goto restore_sub;

/*----------------------------------------------------------------------*/

#include "slginsts.i"
#include "tc_insts.i"

/*----------------------------------------------------------------------*/

 case term_comp: /* RRR */
    op1 = (Cell)*(opregaddr);
    op2 = (Cell)*(opregaddr);
    bld_int(opregaddr, compare(op1, op2));
    pad64;
    goto contcase;

 case movreg: /* PRR */
    pad;
    op1 = (Cell)(opregaddr);
    bld_copy0(opregaddr, *((CPtr)op1));
    pad64;
    goto contcase;

#define ARITHPROC(OP) \
    pad;								\
    op1 = opreg;							\
    op3 = opregaddr;							\
    pad64;								\
    op2 = *(op3);							\
    deref(op1);								\
    deref(op2);								\
    if (isinteger(op1)) {						\
	if (isinteger(op2)) {						\
	    bld_int(op3, int_val(op2) OP int_val(op1));	}		\
	else if (isfloat(op2)) {					\
	    bld_float(op3, float_val(op2) OP (Float)int_val(op1)); }	\
	else {arithmetic_exception(lpcreg);}				\
    } else if (isfloat(op1)) {						\
	if (isfloat(op2)) {						\
	    bld_float(op3, float_val(op2) OP float_val(op1)); }		\
	else if (isinteger(op2)) {					\
	    bld_float(op3, (Float)int_val(op2) OP float_val(op1)); }	\
	else {arithmetic_exception(lpcreg);}			\
    } else {arithmetic_exception(lpcreg);}

 case addreg: /* PRR */
    ARITHPROC(+);
    goto contcase; 

 case subreg: /* PRR */
/*    ARITHPROC(-); */
    pad;							
    op1 = opreg;
    op3 = opregaddr;							
    pad64;
    op2 = *(op3);
    deref(op1);
    deref(op2);
    if (isinteger(op1)) {						
	if (isinteger(op2)) {
	    bld_int(op3, int_val(op2) - int_val(op1)); }
	else if (isfloat(op2)) {
	    bld_float(op3, float_val(op2) - (Float)int_val(op1)); }
	else {arithmetic_exception(lpcreg);}
    } else if (isfloat(op1)) {
	if (isfloat(op2)) {
	    bld_float(op3, float_val(op2) - float_val(op1)); }
	else if (isinteger(op2)) {
	    bld_float(op3, (Float)int_val(op2) - float_val(op1)); }
	else arithmetic_exception(lpcreg);
    } 
    else arithmetic_exception(lpcreg);
    goto contcase; 

 case mulreg: /* PRR */
    ARITHPROC(*);
    goto contcase; 

 case divreg: /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op1);
    deref(op2);
    if (isinteger(op1)) {
	if (isinteger(op2)) {
	    bld_float(op3, (Float)int_val(op2)/(Float)int_val(op1)); }
	else if (isfloat(op2)) {
	    bld_float(op3, float_val(op2)/(Float)int_val(op1)); }
	else {arithmetic_exception(lpcreg);}
    } else if (isfloat(op1)) {
	if (isfloat(op2)) {
	    bld_float(op3, float_val(op2)/float_val(op1)); }
	else if (isinteger(op2)) {
	    bld_float(op3, (Float)int_val(op2)/float_val(op1)); }
	else {arithmetic_exception(lpcreg);}
    } else {arithmetic_exception(lpcreg);}
    goto contcase; 

 case idivreg: /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op1);
    deref(op2);
    if (isinteger(op1) && isinteger(op2)) {
      if (int_val(op1) != 0) { bld_int(op3, int_val(op2) / int_val(op1)); }
      else {
	err_handle(ZERO_DIVIDE, 2,
		   "arithmetic expression involving is/2 or eval/2",
		   2, "non-zero number", op1);
	lpcreg = pcreg;
      }
    }
    else {arithmetic_exception(lpcreg);}
    goto contcase; 

 case int_test_z:   /* PPR-N-L */
    ppad;
    op1 = opreg; pad64;
    deref(op1); op2word;
    if (isnumber(op1)) {
	if ((int_val(op1) - (Integer)op2) == 0)
	    lpcreg = *(byte **)lpcreg;
        else ADVANCE_PC;
    }
    else {
	ADVANCE_PC;
	arithmetic_exception(lpcreg);
    }
    goto contcase;

 case int_test_nz:   /* PPR-N-L */
   ppad;
   op1 = opreg; pad64;
   deref(op1); op2word;
    if (isnumber(op1)) {
	if ((int_val(op1) - (Integer)op2) != 0)
	    lpcreg = *(byte **)lpcreg;
        else ADVANCE_PC;
    }
    else {
	ADVANCE_PC;
	arithmetic_exception(lpcreg);
    }
    goto contcase;

 case putdval: /* PVR */
    pad; 
    op1 = opvar;
    deref(op1);
    op2 = (Cell)(opregaddr);
    pad64;
    bld_copy0((CPtr)op2, op1);
    goto contcase;

 case putuval: /* PVR */
    pad;
    op1 = opvar;
    op2 = (Cell)(opregaddr);
    pad64;
    deref(op1);
    if (isnonvar(op1) || ((CPtr)(op1) < hreg) || ((CPtr)(op1) >= ereg)) {
	bld_copy0((CPtr)op2, op1);
    } else {
	bld_ref((CPtr)op2, hreg);
	bind_ref((CPtr)(op1), hreg);
	new_heap_free(hreg);
    } 
    goto contcase;

 case call: /* PPA-S */
    pppad; pad64; op2word;	/* the first arg is used later by alloc */
    cpreg = lpcreg;
    psc = (Psc)op2;
    call_sub(psc);
    goto contcase;

 case allocate: /* PPP */
    pppad; 
    pad64;
    if (efreg_on_top(ereg))
      op1 = (Cell) (efreg -1);
    else {
      if (ereg_on_top(ereg)) op1 = (Cell)(ereg - *(cpreg-2*sizeof(Cell)+3));
      else op1 = (Cell)(ebreg-1);
    }
    *(CPtr *)((CPtr) op1) = ereg;
    *((byte **) (CPtr)op1-1) = cpreg;
    ereg = (CPtr)op1; 
    goto contcase;

 case deallocate: /* PPP */
    pppad; 
    pad64;
    cpreg = *((byte **)ereg-1);
    ereg = *(CPtr *)ereg;
    goto contcase;

 case proceed:  /* PPP */
    lpcreg = cpreg;
    goto contcase;

 case execute:  /* PPP-S */
    pppad; pad64; op2word;
    psc = (Psc)op2;
    call_sub(psc);
    goto contcase;

 case jump:   /* PPP-L */
    pppad;
    pad64;
    check_glstack_overflow( MAX_ARITY, lpcreg, OVERFLOW_MARGIN ) ;
    lpcreg = *(byte **)lpcreg;
    goto contcase;

 case jumpz:   /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if (int_val(op1) == 0)
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase;

 case jumpnz:    /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if (int_val(op1) != 0)
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase;

 case jumplt:    /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if ((isinteger(op1) && int_val(op1) < 0) ||
	(isfloat(op1) && float_val(op1) < 0.0))
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase; 

 case jumple:    /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if ((isinteger(op1) && int_val(op1) <= 0) ||
	(isfloat(op1) && float_val(op1) <= 0.0))
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase; 

 case jumpgt:    /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if ((isinteger(op1) && int_val(op1) > 0) ||
	(isfloat(op1) && float_val(op1) > 0.0))
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase;

 case jumpge:    /* PPR-L */
    ppad; op1 = opreg;
    pad64;
    if ((isinteger(op1) && int_val(op1) >= 0) ||
	(isfloat(op1) && float_val(op1) >= 0.0))
	lpcreg = *(byte **)lpcreg;
    else ADVANCE_PC;
    goto contcase; 

 case fail:    /* PPP */
    Fail1; 
    goto contcase;

 case noop:  /* PPA */
    ppad; op1byte;
    pad64;
    lpcreg += (int)op1;
    lpcreg += (int)op1;
    goto contcase;

 case halt:  /* PPP */
    pppad;
    pad64;
    pcreg = lpcreg; 
    inst_begin = lpcreg;  /* hack for the moment to make this a ``creturn'' */
    return(0);	/* not "goto contcase"! */

 case builtin:   
    ppad; op1byte; pad64; pcreg=lpcreg; 
    if (builtin_call((int)(op1))) {lpcreg=pcreg;}
    else Fail1;
    goto contcase;

 case unifunc:   /* PAR */
    pad;
    op1byte;
    if (unifunc_call((int)(op1), opregaddr) == 0) {
	printf("Error in unary function call\n");
	Fail1;
    }
    pad64;
    goto contcase;

 case userfunc:	/* PPA-S */	/* The same as "call" now */
    pppad; pad64; op2word;	/* the first arg is used later by alloc */
    cpreg = lpcreg;
    psc = (Psc)op2;
    call_sub(psc);
    goto contcase;

 case calld:   /* PPA-L */
    pppad;
    pad64;
    check_glstack_overflow( MAX_ARITY, lpcreg, OVERFLOW_MARGIN ) ;
    cpreg = lpcreg+sizeof(Cell); 
    lpcreg = *(pb *)lpcreg;
    goto contcase;

 case lshiftr:  /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64; 
    op2 = *(op3);
    deref(op1); 
    deref(op2);
    if (!isinteger(op1) || !isinteger(op2)) { bitop_exception(lpcreg); }
    else { bld_int(op3, int_val(op2) >> int_val(op1)); }
    goto contcase; 

 case lshiftl:   /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op1); 
    deref(op2);
    if (!isinteger(op1) || !isinteger(op2)) { bitop_exception(lpcreg); }
    else { bld_int(op3, int_val(op2) << int_val(op1)); }
    goto contcase; 

 case or:   /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op1); 
    deref(op2);
    if (!isinteger(op1) || !isinteger(op2)) { bitop_exception(lpcreg); }
    else { bld_int(op3, int_val(op2) | int_val(op1)); }
    goto contcase; 

 case and:   /* PRR */
    pad;
    op1 = opreg;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op1); 
    deref(op2);
    if (!isinteger(op1) || !isinteger(op2)) { bitop_exception(lpcreg); }
    else { bld_int(op3, int_val(op2) & int_val(op1)); }
    goto contcase; 

 case negate:   /* PPR */
    ppad;
    op3 = opregaddr;
    pad64;
    op2 = *(op3);
    deref(op2);
    if (!isinteger(op2)) { bitop_exception(lpcreg); }
    else { bld_int(op3, ~(int_val(op2))); }
    goto contcase; 

 case endfile:	/*  */

 default: 
    sprintf(message, "Illegal opcode hex %x at %p", *--lpcreg, lpcreg); 
    xsb_exit(message);
} /* end of switch */


/*======================================================================*/
/* unification routines							*/
/*======================================================================*/

#define SUCCEED		break
#define IFTHEN_SUCCEED
#define FAILED		Fail1; break
#define IFTHEN_FAILED	Fail1

nunify: /* ( op1, op2 ) */
/* word op1, op2 */
#include "unify.i"

    goto contcase;  /* end of nunify */

/*======================================================================*/

subtryme:
{
  register CPtr cps_top;	/* cps_top only needed for efficiency */

  save_find_locx(ereg);		/* sets ebreg to the top of the E-stack	*/
  cps_top = top_of_cpstack;
  check_tcpstack_overflow(cps_top);
  save_registers(cps_top, (Cell)op1, i, rreg);
  save_choicepoint(cps_top, ereg, (byte *)op2, breg);
  breg = cps_top;
  hbreg = hreg;
  goto contcase;
} /* end of subtryme */

/*----------------------------------------------------------------------*/

restore_sub:
{
  register CPtr tbreg;

  tbreg = breg;
  switch_envs(tbreg);
  ptcpreg = cp_ptcp(tbreg);
  delayreg = cp_pdreg(tbreg);
  restore_some_wamregs(tbreg, ereg);
  restore_registers(tbreg, (int)op1, i, rreg);
  if (restore_type == 1) { /* trust */
    breg = cp_prevbreg(breg); 
    restore_trail_condition_registers(breg);
  }
  goto contcase;
}

/*----------------------------------------------------------------------*/

table_restore_sub:
{
  register CPtr tbreg;

  tbreg = breg;
  switch_envs(tbreg);
  ptcpreg = tbreg;	/* This CP should be used for the dependency graph */
  delayreg = NULL;
  restore_some_wamregs(tbreg, ereg);
  table_restore_registers(tbreg, (int)op1, i, rreg);
  if (restore_type == 1) { 
    xtemp1 = tcp_prevbreg(breg); 
    restore_trail_condition_registers(xtemp1);
  }
  goto contcase;
}

/*----------------------------------------------------------------------*/

} /* end emuloop */
