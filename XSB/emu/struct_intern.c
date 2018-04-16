/* File:      struct_intern.c
** Author(s): Warren
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
** $Id: struct_intern.c,v 1.3 2013-05-06 21:10:25 dwarren Exp $
** 
*/

/* to do: in no particular order
1. automatic expansion of hashtables
2. C program to do interning of full term
3. Garbage collection of interned terms
4. Fix tabling to take advantage of interned terms.

*/


#include "xsb_config.h"
#include "xsb_debug.h"

/* Special debug includes */
#include "debugs/debug_biassert.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "setjmp_xsb.h"
#include "auxlry.h"
#include "context.h"
#include "cell_xsb.h"
#include "psc_xsb.h"
#include "error_xsb.h"
#include "cinterf.h"
#include "memory_xsb.h"
#include "deref.h"
#include "register.h"
#include "heap_xsb.h"
#include "flags_xsb.h"
#include "struct_intern.h"

extern int is_cyclic(CTXTdeclc Cell);
extern int ground(Cell);
extern void printterm(FILE *, Cell, long);
extern int compare(CTXTdeclc const void *, const void *);

#define clean_addr(ptr) ((void *)((UInteger)(ptr) & 0xfffffffffffffffe))

/*  Finds the size of noninterned part of a term prior to interning.
Argument term must have been dereferenced. */

Integer intern_term_size(CTXTdeclc Cell term)
{
  Integer size = 0 ;

 recur:
  switch(cell_tag(term)) {
  case XSB_FREE:
  case XSB_REF1:
  case XSB_INT:
  case XSB_STRING:
  case XSB_FLOAT:
    return size ;
  case XSB_LIST: {
    if (isinternstr(term)) {return size;}
    else {
      CPtr pfirstel ;
      pfirstel = clref_val(term) ;
      term = *pfirstel ; 
      XSB_Deref(term) ;
      size += 2 + intern_term_size(CTXTc term) ;
      term = *(pfirstel+1) ; XSB_Deref(term) ;
      goto recur;
    }
  }
  case XSB_STRUCT: {
    if (isinternstr(term)) return size;
    else {
      int a ;
      CPtr pfirstel ;
      pfirstel = (CPtr)cs_val(term) ;
      a = get_arity((Psc)(*pfirstel)) ;
      size += a + 1 ;
      if (a) {  
	while( --a ) {
	  term = *++pfirstel ; 
	  XSB_Deref(term) ;
	  size += intern_term_size( CTXTc term ) ;
	}
      }
      term = *++pfirstel ; XSB_Deref(term) ;
      goto recur;
    }
  }
  case XSB_ATTV:
    return size;
  }
  return FALSE;
}


/* block headers, 1 for each arity; 256 for lists */
#define LIST_INDEX 256
#define BIG_ARITY_INDEX_BASE 257
#define MAX_INTERN_BIG_ARITIES 140

struct hc_block_rec *hc_block[400] = {0};
int big_arities[140] = {0};

UInteger it_hash(Integer ht_size, int reclen, CPtr termrec) {
  UInteger hsh = 0;
  int i;
  // termrec is untagged address!
  for (i = 0; i<reclen; i++) {
    hsh = (hsh << 2*(i % 16)) + cell(termrec+i); 
  }
  return hsh % ht_size;
}

int get_big_arity_index(int big_arity) {
  int i;

  if (big_arity < BIG_ARITY_INDEX_BASE)
    xsb_abort("Internal error: Should be big arity!");
  for (i=0; i<MAX_INTERN_BIG_ARITIES; i++) {
    if (big_arities[i] == 0) {
      big_arities[i] = big_arity;
      break;
    } else if (big_arities[i] == big_arity)
      break;
  }
  if (i >= MAX_INTERN_BIG_ARITIES)
    xsb_abort("[intern_term] Too many big arity functors\n");
  return i + BIG_ARITY_INDEX_BASE;
}

#define get_big_arity_from_index(index) big_arities[index-BIG_ARITY_INDEX_BASE]

/* Called from gc_mark.h marking for garbage collection. */
/* Must be called with interned term (isinternstr(term)is true). */
int is_interned_rec(Cell term) {
  int areaindex, reclen;
  struct intterm_rec *recptr;
  CPtr term_rec;
  UInteger hashindex; 
  struct hc_block_rec *hc_blk_ptr;

  if (islist(term)) {areaindex = LIST_INDEX; reclen = 2; }
  else {areaindex = get_arity(get_str_psc(term)); reclen = areaindex + 1; }

  if (areaindex > 256) areaindex = get_big_arity_index(areaindex);
  hc_blk_ptr = hc_block[areaindex];

  if (!hc_blk_ptr) return FALSE;
  term_rec = (CPtr)cs_val(term);

  hashindex = it_hash(hc_blk_ptr->hashtab_size,reclen,term_rec);
  recptr = hc_blk_ptr->hashtab[hashindex];
  while (recptr) {
    if (term_rec == (CPtr)&(recptr->intterm_psc)) {return TRUE;}
    recptr = clean_addr(recptr->next);
  }
  return FALSE;
}


#define it_hashtab_size 1048573
CPtr insert_interned_rec(int reclen, struct hc_block_rec *hc_blk_ptr, CPtr termrec) {
  struct intterm_rec *recptr, *prev;
  Integer hashindex; 
  int i, found;
  CPtr hc_term;
  
  if (!hc_blk_ptr->base) { /* allocate first block */
    hc_blk_ptr->base = 
      mem_calloc(sizeof(Cell),(1+hc_num_in_block*(1+reclen)),OTHER_SPACE); /* for now, make own space*/
    if (!hc_blk_ptr->base) {
      xsb_error("No memory for interned terms\n");
    }
    hc_blk_ptr->hashtab = 0;
    hc_blk_ptr->hashtab_size = 0;
    hc_blk_ptr->freechain = 0;
    hc_blk_ptr->freedisp = &(hc_blk_ptr->base->recs);
  }
  if (!hc_blk_ptr->hashtab) {
    hc_blk_ptr->hashtab = mem_calloc(sizeof(Cell),it_hashtab_size,OTHER_SPACE);
    if (!hc_blk_ptr->hashtab) xsb_abort("No memory for interned terms\n");
    hc_blk_ptr->hashtab_size = it_hashtab_size;
  }

  hashindex = it_hash(hc_blk_ptr->hashtab_size,reclen,termrec);
  prev = recptr = clean_addr(hc_blk_ptr->hashtab[hashindex]);
  while (recptr) {
    found = 1;
    hc_term = (CPtr)&(recptr->intterm_psc);
    for (i=0; i<reclen; i++) {
      if (cell(hc_term+i) != cell(termrec+i)) {
	found = 0; break;
      }
    }
    if (found) {/*printf("old %p\n",hc_term);*/ return hc_term;}
    prev = recptr;
    recptr = clean_addr(recptr->next);
  }
  if ((recptr = hc_blk_ptr->freechain)) { // take available rec from freechain if is one
    hc_blk_ptr->freechain = recptr->next;
  } else {
    recptr = hc_blk_ptr->freedisp;
    if ((CPtr)recptr >= (CPtr)(&(hc_blk_ptr->base->recs)) + (hc_num_in_block*(1+reclen))) { 
      struct intterm_block *newblock;
      newblock = mem_calloc(sizeof(Cell),(1+hc_num_in_block*(1+reclen)),OTHER_SPACE);
      if (!newblock) {
	xsb_error("No memory for interned terms\n");
      }
      newblock->nextblock =  hc_blk_ptr->base;
      hc_blk_ptr->base = newblock;
      hc_blk_ptr->freedisp = &(newblock->recs);
      recptr = &(newblock->recs);
    }
    hc_blk_ptr->freedisp = (struct intterm_rec *)((CPtr)(hc_blk_ptr->freedisp) + reclen+1);
  }
  if (prev) prev->next = recptr; else hc_blk_ptr->hashtab[hashindex] = recptr;
  recptr->next = 0;
  hc_term = (CPtr)&(recptr->intterm_psc);
  for (i=0; i<reclen; i++) {
    cell(hc_term+i) = cell(termrec+i);
  }
  return hc_term;
}

/* should be passed a term which is dereffed for which isinternstr is true! */
int isinternstr_really(prolog_term term) {
  return is_interned_rec(term);
}

/* intern_rec takes a reference to struct record with all subfields
   pointing either to atoms, integers, or other interned term records,
   and returns a str-tagged pointer to the interned version of that
   struct record.  It returns 0 if the struct record isn't of this
   form */

Cell intern_rec(CTXTdeclc prolog_term term) {

  int areaindex, reclen, i, j;
  CPtr hc_term;
  static Cell *dterm = NULL;
  static int dterm_len = 0;
  Cell arg;
  struct hc_block_rec *hc_blk_ptr;

  // create term-record with all fields dereffed in dterm
  XSB_Deref(term);
  if (isinternstr(term)) {
    return term;
  }
  if (isconstr(term)) {
    areaindex = get_arity(get_str_psc(term)); 
    reclen = areaindex + 1;
    if (areaindex > 255) areaindex = get_big_arity_index(areaindex);
    if (reclen > dterm_len) {
      dterm = realloc(dterm,reclen*sizeof(Cell));
      dterm_len = reclen;
    }
    cell(dterm) = (Cell)get_str_psc(term); // copy psc ptr
    j=1;
  } else if (islist(term)) {
    areaindex = LIST_INDEX; 
    reclen = 2;
    if (reclen > dterm_len) {
      dterm = realloc(dterm,reclen*sizeof(Cell));
      dterm_len = reclen;
    }
    j=0;
  } else return 0;
  hc_blk_ptr = hc_block[areaindex];
  if (!hc_blk_ptr) {
    hc_blk_ptr = (struct hc_block_rec *)mem_calloc(sizeof(struct hc_block_rec),1,OTHER_SPACE);
    hc_block[areaindex] = hc_blk_ptr;
  }
  if (!hc_blk_ptr) xsb_abort("[intern_term] No space for hc_block\n");
  for (i=j; i<reclen; i++) {
    arg = get_str_arg(term,i);  // works for lists and strs
    XSB_Deref(arg);
    if (isref(arg) || (isstr(arg) && !isinternstr(arg)) || isattv(arg)) {
      return 0;
    }
    cell(dterm+i) = arg;
  }

  hc_term = insert_interned_rec(reclen, hc_blk_ptr, dterm);
  if (islist(term)) return makelist(hc_term); else return makecs(hc_term);
}


struct term_subterm *ts_array = 0;
Integer ts_array_len = 0;
#define init_ts_array_len 5000  
#define check_ts_array_overflow \
  if (ti >= ts_array_len) {						\
    ts_array = mem_realloc(ts_array,ts_array_len*sizeof(*ts_array),	\
			   ts_array_len*2*sizeof(*ts_array),OTHER_SPACE); \
    ts_array_len = ts_array_len*2;					\
  }


/* caller must ensure enough heap space (term_size(term)*sizeof(Cell)) */
prolog_term intern_term(CTXTdeclc prolog_term term) {
  Integer ti = 0;
  Cell arg, newterm, interned_term;
  unsigned int subterm_index;

  XSB_Deref(term);
  if (!(islist(term) || isconstr(term))) {return term;}
  if (isinternstr(term)) {return term;}
  if (is_cyclic(CTXTc term)) {xsb_abort("[intern_term/2] Cannot intern a cyclic term\n");}

  if (!ts_array) {
    ts_array = mem_alloc(init_ts_array_len*sizeof(*ts_array),OTHER_SPACE);
    if (!ts_array) xsb_abort("No space for interning term\n");
    ts_array_len = init_ts_array_len;
  }
  
  ts_array[0].term = term;
  if (islist(term)) {
    ts_array[0].subterm_index = 0;
    ts_array[0].newterm = makelist(hreg);
    hreg += 2;
  }
  else {
    ts_array[0].subterm_index = 1;
    ts_array[0].newterm = makecs(hreg);
    new_heap_functor(hreg, get_str_psc(term));
    hreg += get_arity(get_str_psc(term));
  }
  ts_array[ti].ground = 1;

  while (ti >= 0) {
    term = ts_array[ti].term;
    newterm = ts_array[ti].newterm;
    subterm_index = ts_array[ti].subterm_index;
    if ((islist(term) && subterm_index >= 2) ||
	(isconstr(term) && subterm_index > get_arity(get_str_psc(term)))) {
      /* all subterms of this term are interned processed */
      if (ts_array[ti].ground) {
	interned_term = intern_rec(CTXTc newterm);
	hreg = clref_val(newterm);  // reclaim used stack space
	if (!ti) {
	  return interned_term;
	}
	ti--;
	get_str_arg(ts_array[ti].newterm,ts_array[ti].subterm_index-1) = interned_term;
      } else {
	if (!ti) {
	  return newterm;
	}
	ti--;
	get_str_arg(ts_array[ti].newterm,ts_array[ti].subterm_index-1) = newterm;
	ts_array[ti].ground = 0;
      }
    } else { /* process next subterm of this term */
      arg = get_str_arg(term, (ts_array[ti].subterm_index)++);
      XSB_Deref(arg);
      switch (cell_tag(arg)) {
      case XSB_FREE:
      case XSB_REF1:
      case XSB_ATTV:
	ts_array[ti].ground = 0;
	get_str_arg(newterm,subterm_index) = arg;
	break;
      case XSB_STRING:
	if (string_find_safe(string_val(arg)) != string_val(arg))
	  printf("uninterned string? %s\n",string_val(arg));
      case XSB_INT:
      case XSB_FLOAT:
	get_str_arg(newterm,subterm_index) = arg;
	break;
      case XSB_LIST:
	if (isinternstr(arg)) get_str_arg(newterm,subterm_index) = arg;
	else {
	  ti++;
	  check_ts_array_overflow;
	  ts_array[ti].term = arg;
	  ts_array[ti].subterm_index = 0;
	  ts_array[ti].ground = 1;
	  ts_array[ti].newterm = makelist(hreg);
	  hreg += 2;
	}
	break;
      case XSB_STRUCT:
	if (isinternstr(arg)) get_str_arg(newterm,subterm_index) = arg;
	else {
	  ti++;
	  check_ts_array_overflow;
	  ts_array[ti].term = arg;
	  ts_array[ti].subterm_index = 1;
	  ts_array[ti].ground = 1;
	  ts_array[ti].newterm = makecs(hreg);
	  new_heap_functor(hreg,get_str_psc(arg));
	  hreg += get_arity(get_str_psc(arg));
	}
      }
    }
  }
  printf("intern_term: shouldn't happen\n");
  return 0;
}

void reclaim_internstr_recs() {
  struct intterm_block *block_ptr;
  struct intterm_rec *rec_ptr, *lrec_ptr;
  struct intterm_rec *prec_ptr = NULL;
  int areaindex, reclen;
  UInteger hashindex; 
  struct hc_block_rec *hc_blk_ptr;

  for (areaindex = 0; areaindex < 400; areaindex++) {
    if (areaindex == LIST_INDEX) reclen = 3; // including next pointer
    else if (areaindex > 255) reclen = get_big_arity_from_index(areaindex)+2;
    else reclen = areaindex+2;
    
    /* mark freechain recs so don't have to go thru hashtable to not find them */
    hc_blk_ptr = hc_block[areaindex];
    if (hc_blk_ptr) {
      rec_ptr = hc_blk_ptr->freechain;
      while (rec_ptr) {
	lrec_ptr = rec_ptr->next;
	rec_ptr->next = (struct intterm_rec *)((UInteger)rec_ptr->next | intern_mark_bit);
	rec_ptr = lrec_ptr;
      }

      block_ptr = hc_blk_ptr->base;
      while (block_ptr) {
	rec_ptr = &(block_ptr->recs);
	while (((CPtr)rec_ptr < (CPtr)block_ptr+1+hc_num_in_block*reclen)
	       && IsNonNULL(rec_ptr->intterm_psc)) {
	  if (!((UInteger)rec_ptr->next & intern_mark_bit)) { // unmarked, so free
	    hashindex = it_hash(hc_blk_ptr->hashtab_size,
				reclen-1,(CPtr)&(rec_ptr->intterm_psc));
	    lrec_ptr = (struct intterm_rec *)&hc_blk_ptr->hashtab[hashindex];
	    while (lrec_ptr) {
	      if (lrec_ptr == rec_ptr) {
		if ((UInteger)(prec_ptr->next) & intern_mark_bit) {
		  prec_ptr->next = (struct intterm_rec *)
		    ((UInteger)clean_addr(lrec_ptr->next) | intern_mark_bit);
		} else prec_ptr->next = clean_addr(lrec_ptr->next);
		lrec_ptr->next = hc_blk_ptr->freechain;
		hc_blk_ptr->freechain = lrec_ptr;
		break;
	      }
	      prec_ptr = lrec_ptr;
	      lrec_ptr = clean_addr(lrec_ptr->next);
	    }
	  }
	  rec_ptr->next = clean_addr(rec_ptr->next);
	  rec_ptr = (struct intterm_rec *)(((CPtr)rec_ptr) + reclen);  // to next rec in this block
	}
	block_ptr = block_ptr->nextblock;  // to next block
      }
    }
  }
    return;
}

/* hashstring is string that begins with }]]}, followed by hex that is
   the address of a prolog term. */

prolog_term stringhash_to_term(char *hashstring) {
  UInteger val;
  int i, h, hexlen;
  
  if (hashstring[0] != '}' || hashstring[1] != ']' || hashstring[2] != ']'
      || hashstring[3] != '}') {return 0;}
  hexlen = 2*sizeof(void *);

  val = 0;
  for (i = 4; i < 4 + hexlen; i++) {
    h = hashstring[i];
    if (h >= 48 && h <= 57) val = 16*val+h-48;
    else if (h >= 65 && h <= 70) val = 16*val+h-55;
    else {return 0;}
  }
  if (!isinternstr((Cell)val)) xsb_abort("ERROR: term from stringhash_to_term is not interned!!!");
  return (prolog_term)val;
}

char hexcode[16] = "0123456789ABCDEF";

prolog_term term_to_stringhash(prolog_term term) {
  char hashstring[21];
  int i, hexlen;

  if (!isinternstr(term)) xsb_abort("ERROR: term to term_to_stringhash is not interned!!!");

  hashstring[0] = '}';
  hashstring[1] = ']';
  hashstring[2] = ']';
  hashstring[3] = '}';

  hexlen = 2*sizeof(void *);
  for (i = 3 + hexlen; i >= 4; i--) {
    hashstring[i] = hexcode[(UInteger)term & 0xf];
    term = (prolog_term)((UInteger)term >> 4);
  }
  hashstring[4+hexlen] = '\0';
  return makestring(string_find(hashstring,1));
}

