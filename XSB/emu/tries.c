/* File:      tries.c
** Author(s): Prasad Rao, David S. Warren, Kostis Sagonas,
**    	      Juliana Freire
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
** $Id: tries.c,v 1.1.1.1 1998-11-05 16:55:25 sbprolog Exp $
** 
*/



#include <stdio.h>
#include <stdlib.h>

#include "configs/config.h"
#include "debugs/debug.h"

/* Special debug includes */
#include "debugs/debug_tries.h"

#include "auxlry.h"
#include "cell.h"
#include "inst.h"
#include "psc.h"
#include "deref.h"
#include "memory.h"
#include "register.h"
#include "binding.h"
#include "tries.h"
#include "xmacro.h"
#include "choice.h"
#include "cinterf.h"
#include "xsberror.h"
#include "tr_utils.h"

struct NODE dummy_ans_node = {0,0,1,0,NULL,NULL,NULL,0};


/*======================================================================*/

extern tab_inf_ptr get_tip(Psc);
extern Psc term_psc(Cell);

/*======================================================================*/

#define set_min(a,b,c)	if (b < c) a = b; else a = c



struct HASHhdr HASHroot = {0,0,0,0};
struct HASHhdr *HASHrootptr;


struct HASHhdr tra_HASHroot = {0,0,0,0};

/*********Simpler trails ********/
#define simple_dbind_ref(addr,val) bld_ref(addr,val);\
	*(++temp_trreg) = addr;


#define simple_table_undo_bindings while(temp_trreg > trie_tr_base){\
	bld_ref(*temp_trreg,(Cell)*temp_trreg);\
	temp_trreg--;\
	}
	
	
/*******NEW DEFINES******/

#define FirstInstOfVar(x) (Integer)int_val(x) & 0x10000

#define make_no_cp(where,X) switch(cell_tag(X)) {\
	case CS:\
		Instr(where) = (byte) trie_no_cp_str;\
		break;\
	case INT:\
	case STRING:\
	case FLOAT:\
		Instr(where) = (byte) trie_no_cp_numcon;\
		break;\
	case TrieVar:\
		if (FirstInstOfVar(X))\
		  Instr(where) = (byte) trie_no_cp_var;\
		else\
		  Instr(where) = (byte) trie_no_cp_val;\
		break;\
	case LIST:\
		Instr(where) = (byte) trie_no_cp_list;\
		break;\
	default:\
		fprintf(stderr, "Bad tag in make no cp in atom: %x ",(int)X);\
		xsb_abort("Bye");\
	}

#define make_try_opcode(where,X) switch(cell_tag(X)) {\
	case CS:\
		Instr(where) = (byte) trie_try_str;\
		break;\
	case INT:\
	case STRING:\
  	case FLOAT:\
		Instr(where) = (byte) trie_try_numcon;\
		break;\
	case TrieVar:\
		if (FirstInstOfVar(X))\
		  Instr(where) = (byte) trie_try_var;\
		else\
		  Instr(where) = (byte) trie_try_val;\
		break;\
	case LIST:\
		Instr(where) = (byte) trie_try_list;\
		break;\
	default:\
		fprintf(stderr, "Bad tag in make try opcode in atom: %x ",(int)X);\
		xsb_abort("Bye");\
	}
		
/* can be changed to Instr(Sibl(X)) = Instr(Sibl(X))|RETRY_MASK 
But for now */

#define make_current_try(X)	Instr(X) = ((Instr(X)) & ~3) | 0x2
#define make_current_retry(X)	X->instr++
#define change_to_success(X)	Instr(X) += 4 /* 4 Cells */
/* Just zero out the last two bits */
#define lay_no_cp_in_bucket(X)	Instr(X) = ((Instr(X)) & ~3 )
	
/*****************Addr Stack************/
int addr_stack_pointer = 0;
int addr_stack_size   = DEFAULT_ARRAYSIZ;

CPtr *Addr_Stack;
#define push_addr(X) {\
    if(addr_stack_pointer == addr_stack_size){\
       xpand_array(CPtr, Addr_Stack ,addr_stack_size,"Addr_Stack");\
    }\
    Addr_Stack[addr_stack_pointer++] = ((CPtr) X);\
}


#define pop_addr Addr_Stack[--addr_stack_pointer]
/*****************Term Stack*************/
int term_stackptr = -1;
long term_stacksize = DEFAULT_ARRAYSIZ;
/**********************************/

int ctr;

long subg_chk_ins, subg_inserts, ans_chk_ins, ans_inserts;

int  num_heap_term_vars;
CPtr *var_addr;
int  var_addr_arraysz = DEFAULT_ARRAYSIZ;
Cell VarEnumerator[NUM_TRIEVARS];

struct NODE *TrieArray;
NODEptr	Paren, *GNodePtrPtr;
tab_inf_ptr  UglyHackForTip;

/*----------------------------------------------------------------------*/

int trie_chunk_size = 2048 * sizeof(struct NODE);

char *trie_node_chunk_ptr = 0;

NODEptr free_trie_nodes = 0;
NODEptr free_trie_space = 0;
NODEptr top_trie_space = 0;
SGFrame subg_structure_list = NULL;

NODEptr tra_free_trie_nodes = 0;
NODEptr tra_free_trie_space = 0;
NODEptr tra_top_trie_space = 0;
char    *tra_trie_node_chunk_ptr = 0;

NODEptr bak_free_trie_nodes = 0;
NODEptr bak_free_trie_space = 0;
NODEptr bak_top_trie_space = 0;
char    *bak_trie_node_chunk_ptr = 0;



int answer_list_chunk_size = 512 * sizeof(struct answer_list_node);

char *answer_list_node_chunk_ptr = 0;

ALPtr free_answer_list_nodes = 0;
ALPtr free_answer_list_space = 0;
ALPtr top_answer_list_space = 0;

/*----------------------------------------------------------------------*/

static NODEptr alloc_more_trie_space()
{
  char *t;
  t = (char *)mem_alloc(trie_chunk_size+sizeof(Cell));
  if (!t) xsb_abort("No room to expand Trie space");
  *(char **)t = trie_node_chunk_ptr;
  trie_node_chunk_ptr = t;
  free_trie_space = (NODEptr)(t+sizeof(Cell));
  top_trie_space = (NODEptr)(t+trie_chunk_size+sizeof(Cell));
  return free_trie_space++;
}

/*----------------------------------------------------------------------*/

static ALPtr alloc_more_answer_list_space()
{
  char *t;
  t = (char *)mem_alloc(answer_list_chunk_size+sizeof(Cell));
  if (!t) xsb_abort("No room to expand Answer_List space");
  *(char **)t = answer_list_node_chunk_ptr;
  answer_list_node_chunk_ptr = t;
  free_answer_list_space = (ALPtr)(t+sizeof(Cell));
  top_answer_list_space = (ALPtr)(t+answer_list_chunk_size+sizeof(Cell));
  return free_answer_list_space++;
}
 
/*----------------------------------------------------------------------*/
#ifdef PVPROF
int num_newnodes = 0;
int freenodes = 0;
#endif

#ifdef PVPROF
#define NewNode(t,item,chil,sibl,nparen) {\
  if (free_trie_nodes) {\
    t = free_trie_nodes;\
    free_trie_nodes = Sibl(free_trie_nodes);\
  }\
  else if (free_trie_space < top_trie_space) \
    {t = free_trie_space++;}\
  else {t = alloc_more_trie_space();}\
  Sibl(t) = (NODEptr)sibl;\
  Child(t)= (NODEptr)chil;\
  Parent(t) =(NODEptr)nparen;\
  DelFlag(t) = (byte)0;\
  NodeType(t) = NODE_TYPE_UNKNOWN;\
  Atom(t) = (Cell)item;\
  if (sibl != NULL) make_try_opcode(t,item) else make_no_cp(t,item);\
  num_newnodes++;\
}
#else

#define NewNode(t,item,chil,sibl,nparen) {\
  if (free_trie_nodes) {\
    t = free_trie_nodes;\
    free_trie_nodes = Sibl(free_trie_nodes);\
  }\
  else if (free_trie_space < top_trie_space) \
    {t = free_trie_space++;}\
  else {t = alloc_more_trie_space();}\
  Sibl(t) = (NODEptr)sibl;\
  Child(t)= (NODEptr)chil;\
  Parent(t) =(NODEptr)nparen;\
  DelFlag(t) = (byte)0;\
  NodeType(t) = NODE_TYPE_UNKNOWN;\
  Atom(t) = (Cell)item;\
  if (sibl != NULL) make_try_opcode(t,item) else make_no_cp(t,item);\
}


#endif
/*----------------------------------------------------------------------*/
 
#define NewAnsListNode(t) {\
  if (free_answer_list_nodes) {\
    t = free_answer_list_nodes;\
    free_answer_list_nodes = aln_next_aln(free_answer_list_nodes);\
  }\
  else if (free_answer_list_space < top_answer_list_space) \
    {t = free_answer_list_space++;}\
  else {t = alloc_more_answer_list_space();}\
  aln_answer_ptr(t) = NULL;\
  aln_next_aln(t) = NULL;\
  }

/*----------------------------------------------------------------------*/

void abolish_trie(void)
{
  char *t;
  struct HASHhdr *th, *tht;
  SGFrame temp;

  while (trie_node_chunk_ptr) {
    t = *(char **)trie_node_chunk_ptr;
/*    printf("Freeing Trie chunk %x\n",trie_node_chunk_ptr);*/
    mem_dealloc((byte *)trie_node_chunk_ptr,trie_chunk_size+sizeof(Cell));
    trie_node_chunk_ptr = t;
  }
  free_trie_nodes = 0;
  free_trie_space = 0;
  top_trie_space = 0;

  th = HASHrootptr->next;
  while (th) {
    tht = th->next;
/*    printf("freeing hashtab: %x, size: %d, nument: %d\n",th,th->HASHmask+1,th->numInHash);*/
    free(th);
    th = tht;
  }

  while(answer_list_node_chunk_ptr){
    t = *(char **)answer_list_node_chunk_ptr;
    mem_dealloc((byte *)answer_list_node_chunk_ptr,answer_list_chunk_size+sizeof(Cell));
    answer_list_node_chunk_ptr = t;
  }

  free_answer_list_nodes = 0;
  free_answer_list_space = 0;
  top_answer_list_space = 0;
  

  while(subg_structure_list != NULL){
    temp = subg_structure_list;
    subg_structure_list = subg_next_subgoal(subg_structure_list);
    free(temp);
  }

  HASHrootptr->next = 0;
  HASHrootptr->prev = 0;
  HASHrootptr->numInHash = 0;
  HASHrootptr->HASHmask = 0;

 /* clear_interned_tries();*/
}

/*----------------------------------------------------------------------*/
/* The following exported routines are mainly used for statistics.	*/
/*----------------------------------------------------------------------*/

int allocated_trie_size(void)
{
  int size = 0;
  char *t = trie_node_chunk_ptr;

  while (t) {
    size = size + (trie_chunk_size+sizeof(Cell));
    t = *(char **)t;
  }
  return size;
}

/*--------------------------------------------------
Include the number of "free nodes" in the list in 
the free trie space
--------------------------------------------------*/
int num_free_nodes()
{
  int i = 0;
  NODEptr p;

  p = free_trie_nodes;
  while(p != NULL){
    i++;
    p = Sibl(p);
  }
  return(i);
}

int free_trie_size(void)
{
  return (top_trie_space - free_trie_space +num_free_nodes()) * sizeof(struct NODE);
}

int allocated_trie_hash_size(void)
{
  int size = 0;
  struct HASHhdr *th = HASHrootptr->next;

  while (th) {
    size = size + (th->HASHmask+1);
    th = th->next;
  }
  return size;
}

/*----------------------------------------------------------------------*/

#define IsInsibling(wherefrom,count,Found,item)\
{\
  LocalNodePtr = wherefrom;\
  while (LocalNodePtr && (Atom(LocalNodePtr) != item)) {\
    LocalNodePtr = Sibl(LocalNodePtr);\
    count++;\
  }\
  if (!LocalNodePtr) {\
    NewNode(LocalNodePtr,item,NULL,wherefrom,Paren);\
    Paren = LocalNodePtr;\
    wherefrom = LocalNodePtr;\
    Found = 0;\
    if(Sibl(LocalNodePtr))\
    	make_current_retry(Sibl(LocalNodePtr));\
  } else Paren = LocalNodePtr;\
}

/*************************************************/
#define one_node_chk_ins(Found,item) \
{\
  int count = 0;\
  NODEptr LocalNodePtr, *Cnodeptr;\
  struct HASHhdr *hh;\
\
  if (!(*GNodePtrPtr)) {\
      NewNode(LocalNodePtr,item,NULL,NULL,Paren);\
      *GNodePtrPtr = Paren = LocalNodePtr;\
      Found = 0;\
  } else {\
      if (Sibl(*GNodePtrPtr) == (NODEptr)-1) {\
          Cnodeptr = (NODEptr *)Child(*GNodePtrPtr);\
          hh = (struct HASHhdr *)Cnodeptr - 1;\
          if (hh->numInHash > hh->HASHmask) {\
            expand_hash(GNodePtrPtr,hh);\
            Cnodeptr = (NODEptr *)Child(*GNodePtrPtr);\
            hh = (struct HASHhdr *)Cnodeptr - 1;\
          }\
          GNodePtrPtr = Cnodeptr + HASH(item,hh->HASHmask);\
          IsInsibling(*GNodePtrPtr,count,Found,item);\
          if (!Found) {hh->numInHash++;};\
      } else {\
          IsInsibling(*GNodePtrPtr,count,Found,item);\
	  if (count>LENGTHLIMIT) {reorganize(GNodePtrPtr);}\
      }\
  }\
  GNodePtrPtr = &(Child(LocalNodePtr));\
}

/*******************************/

static void reorganize(NODEptr *locnPtrPtr)
{
  int num;
  NODEptr ProcessPtr,ArrayPtr,TempPtr;
  NODEptr *Array;
  struct HASHhdr *th;

  NewNode(ArrayPtr,(Cell)FREE,(NODEptr)NULL,(NODEptr)-1,(NODEptr)NULL);
  Instr(ArrayPtr) = (byte)hash_opcode;
	/* need calloc so table is initialized to 0 */
  th = (struct HASHhdr *) 
	calloc(1,HASHLENGTH*sizeof(NODEptr)+sizeof(struct HASHhdr));
/*  printf("Allocating initial Hash array: %x (%d)\n",th,HASHLENGTH);*/
  if (!th) {
     xsb_abort("No more memory in Reorganize\n");
  }
  th->next = HASHrootptr->next;
  th->prev = HASHrootptr;
  th->HASHmask = HASHLENGTH-1;
  th->numInHash = LENGTHLIMIT+1;
  if (HASHrootptr->next) HASHrootptr->next->prev = th;
  HASHrootptr->next = th;
  Array = (NODEptr *)(th + 1);

  ProcessPtr = *locnPtrPtr;
  *locnPtrPtr = (NODEptr) ArrayPtr;
  Child(ArrayPtr) = (NODEptr) Array;
  while(ProcessPtr) {
      num = HASH(Atom(ProcessPtr),th->HASHmask);
      TempPtr = Sibl(ProcessPtr);
      Sibl(ProcessPtr) = Array[num]; 
      if (Array[num] != NULL) {
	make_current_try(ProcessPtr);
	make_current_retry(Array[num]);
      }
      else {
	lay_no_cp_in_bucket(ProcessPtr);
      }
      Array[num] = ProcessPtr;
      ProcessPtr = TempPtr;
  }
}

/***********************************************/

void expand_hash(NODEptr *locnPtrPtr, struct HASHhdr *oldHash)
{
  struct HASHhdr *newHash;
  Cell newHashMask;
  Integer num;
  NODEptr *oldArray, *newArray, *bucketptr, ProcessPtr, TempPtr;

  /* remove old hash table from from global chain */
  oldHash->prev->next = oldHash->next;
  if (oldHash->next) oldHash->next->prev = oldHash->prev;

  newHashMask = (oldHash->HASHmask<<1) + 1;
	/* need calloc so table is initialized to 0 */
  newHash = (struct HASHhdr *) 
	calloc(1,(newHashMask+1)* sizeof(NODEptr)+sizeof(struct HASHhdr));
/*  printf("Allocating Hash array: %x(%d) to expand: %x\n",newHash,newHashMask+1,oldHash);*/
  if (!newHash) {
     xsb_abort("No more memory to expand hash table.\n");
  }
  newHash->next = HASHrootptr->next;
  newHash->prev = HASHrootptr;
  newHash->HASHmask = newHashMask;
  newHash->numInHash = oldHash->numInHash;
  if (HASHrootptr->next) HASHrootptr->next->prev = newHash;
  HASHrootptr->next = newHash;

  newArray = (NODEptr *)(newHash + 1);
  oldArray = (NODEptr *)(oldHash + 1);
  for (bucketptr = oldArray; 
       bucketptr <= oldArray+oldHash->HASHmask;
       bucketptr++) {
    ProcessPtr = *bucketptr;
    while (ProcessPtr) {
      num = HASH(Atom(ProcessPtr),newHash->HASHmask);
      TempPtr = Sibl(ProcessPtr);
      Sibl(ProcessPtr) = newArray[num];
      if (newArray[num] != NULL) {
	make_current_try(ProcessPtr);
	make_current_retry(newArray[num]);
      }
      else {
	lay_no_cp_in_bucket(ProcessPtr);
      }
      newArray[num] = ProcessPtr;
      ProcessPtr = TempPtr;
    }
  }

  Child(*locnPtrPtr) = (NODEptr)newArray;
  free(oldHash);
}

/*******************/

static void follow_par_chain(NODEptr SolnPtr)
{
  term_stackptr = -1; /* Forcibly Empty term_stack */
  while (SolnPtr != NOPAR) {
    push_term((Atom(SolnPtr)));
    SolnPtr = unftag(Parent(SolnPtr));
  }
}

/************************************/

NODEptr get_next_trie_solution(ALPtr *NextPtrPtr)
{
  NODEptr TempPtr;

  if (!*NextPtrPtr){
    return(NULL);
  }

  if (ftagged(Parent(aln_answer_ptr(*NextPtrPtr))))
    TempPtr = aln_answer_ptr(*NextPtrPtr);
  else 
    TempPtr = NULL;
  *NextPtrPtr = aln_next_aln(*NextPtrPtr);

  /* printf("next returning %d, saving %d\n",TempPtr,*NextPtrPtr); */
  return(TempPtr);
}

/************************************/

#define rec_macro_make_heap_term(Macro_addr)\
{int rj,rArity;\
   while(addr_stack_pointer)\
   {\
     Macro_addr = (CPtr)pop_addr;\
     switch(cell_tag(xtemp2 = (CPtr)pop_term))\
     {\
      case TrieVar: \
	if (FirstInstOfVar(xtemp2)) {\
          safe_assign(var_addr,((Integer)int_val(xtemp2) & 0xffff),Macro_addr,var_addr_arraysz);\
	  *Macro_addr = (Cell)var_addr[(Integer)int_val(xtemp2) & 0xffff];\
	  num_heap_term_vars++;\
	}\
	else *Macro_addr = (Cell) var_addr[int_val(xtemp2)];\
	break;\
      case STRING:\
      case INT:\
      case FLOAT:\
	*Macro_addr = (Cell) xtemp2;\
	break;\
      case LIST: \
	*Macro_addr = (Cell) makelist(hreg);\
	hreg += 2;\
	push_addr(hreg-1); /* changed */\
	push_addr(hreg-2);\
	break;\
      case CS:\
	*Macro_addr = (Cell) makecs(hreg);\
	xtemp2 = (CPtr) cs_val(xtemp2);\
	*hreg = (Cell) xtemp2;\
	rArity = (int) get_arity((Psc) xtemp2);\
	for (rj= rArity; rj >= 1; rj --) {\
	  push_addr(hreg+rj);\
	}\
	hreg += rArity;\
	hreg++;\
	break;\
      default:\
        xsb_abort("Bad tag in macro_make_heap_term");\
	return;\
   }\
  }\
}
/***********************************************/
#define macro_make_heap_term(ataddr,ret_val,dummy_addr)\
{ int mArity,mj;\
  switch(cell_tag(xtemp2 = ((CPtr) pop_term)))\
   {\
    case TrieVar: \
	if (FirstInstOfVar(xtemp2)) {\
          safe_assign(var_addr,((Integer)int_val(xtemp2) & 0xffff),ataddr,var_addr_arraysz);\
	  ret_val = (Cell)var_addr[(Integer)int_val(xtemp2) & 0xffff];\
	  num_heap_term_vars++;\
	}\
	else {ret_val = (Cell) var_addr[(Integer)int_val(xtemp2)];}\
	break;\
    case STRING:\
    case INT:\
    case FLOAT:\
	ret_val = (Cell) xtemp2;\
	break;\
    case LIST: \
	ret_val = (Cell) makelist(hreg) ;\
	hreg += 2;\
	push_addr(hreg-1);/* changed */\
	push_addr(hreg-2);\
	rec_macro_make_heap_term(dummy_addr);\
	break;\
    case CS:\
	ret_val = (Cell) makecs(hreg);\
	xtemp2 = (CPtr) cs_val(xtemp2);\
	*hreg = (Cell) xtemp2;\
	mArity = (int) get_arity((Psc) xtemp2);\
	for (mj= mArity; mj >= 1; mj--) {\
	  push_addr(hreg+mj);\
	}\
	hreg += mArity;\
	hreg++;\
	rec_macro_make_heap_term(dummy_addr);\
	break;\
     default: \
        xsb_abort("Bad tag in macro_make_heap_term");\
	return;\
   }\
}
/***************************************************************************/
#define recvariant_trie(flag)\
{\
	int  j;\
\
	while (!pdlempty ) {\
	  xtemp1 = (CPtr) pdlpop;\
	  cptr_deref(xtemp1);\
	  tag = cell_tag(xtemp1);\
	  switch (tag) {\
	    case FREE: case REF1: \
	    if(is_VarEnumerator(xtemp1)){\
	      simple_dbind_ref_nth_var(xtemp1,ctr);\
	      one_node_chk_ins(flag, maketrievar((ctr | 0x10000)));\
	      ctr++;\
					}\
            else{\
              one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));\
	       }\
              break;\
	    case STRING: case INT: case FLOAT:\
	      one_node_chk_ins(flag, (Cell)xtemp1);\
	      break;\
	    case LIST:\
	      one_node_chk_ins(flag, (Cell)LIST);\
	      pdlpush(cell(clref_val(xtemp1)+1));/* changed */\
              pdlpush(cell(clref_val(xtemp1)));\
	      break;\
	    case CS:\
	      one_node_chk_ins(flag,makecs(follow(cs_val(xtemp1)))); /*put root in trie */\
	      for (j = get_arity((Psc)follow(cs_val(xtemp1))); j>=1 ; j--) \
  	         {pdlpush(cell(clref_val(xtemp1) +j));}\
	      break;\
	    default: \
              xsb_abort("Bad type tag in recvariant_trie...\n");\
	  }\
	}\
        resetpdl;\
}

/************************************************************************/
/* variant_trie_search(NumVarsInCall, VarsInCall, SubgoalPtr, Ptrflag)	*/
/************************************************************************/

NODEptr variant_trie_search(int arity, CPtr cptr,
			    CPtr subgoal_ptr, int *flagptr)
{
    CPtr *trie_tr_base, *temp_trreg;
    CPtr xtemp1;
    int i, j, flag = 1;
    Cell tag = 0 ;
    ALPtr answer_node;

    ans_chk_ins++;

    if(trfreg > trreg)
	trie_tr_base = temp_trreg = trfreg;
    else
	trie_tr_base = temp_trreg = trreg;
    ctr = 0;
    Paren = NOPAR;
    GNodePtrPtr = (NODEptr *) &subg_ans_root_ptr(subgoal_ptr);
    for (i = 0; i<arity; i++) {
      xtemp1 = (CPtr) (cptr - i);
      cptr_deref(xtemp1);
      tag = cell_tag(xtemp1);
      switch (tag) {
      case FREE: case REF1:
	/*one_node_chk_ins(flag,FREE);*/
	if(is_VarEnumerator(xtemp1)){
	  simple_dbind_ref_nth_var(xtemp1,ctr);
	  one_node_chk_ins(flag, maketrievar((ctr | 0x10000)));
	  ctr++;
	}
	else{
          one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));\
	}
	break;
      case STRING: case INT:  case FLOAT:
	one_node_chk_ins(flag, (Cell)xtemp1);
	break;
      case LIST:
	one_node_chk_ins(flag, (Cell)LIST);
	pdlpush(cell(clref_val(xtemp1)+1)); /* changed */
	pdlpush(cell(clref_val(xtemp1)));
	recvariant_trie(flag);
	break;
      case CS:
	/* put root in trie */
	one_node_chk_ins(flag, makecs(follow(cs_val(xtemp1))));
	for (j = get_arity((Psc)follow(cs_val(xtemp1))); j >= 1 ; j--) {
	  pdlpush(cell(clref_val(xtemp1)+j));
	}
	recvariant_trie(flag);
	break;
	default:
	  xsb_abort(" Bad type tag :variant_trie_search\n");
	}                                                       
    }                
    resetpdl;                                                   

    simple_table_undo_bindings;

    /* if there is no parent node an ESCAPE node has to be created;	*/
    /* ow the parent node is "ftagged" to indicate that it is a leaf.	*/
    if (Paren == NOPAR) {
      one_node_chk_ins(flag, (Cell)0);
      Instr(Paren) = trie_proceed;
    } else {
      Parent(Paren) = makeftag(Parent(Paren));
      /* NOTE: for structures and lists tag can be set by recvariant_trie */
      if ((flag == 0) && (tag == STRING || tag == INT || tag == FLOAT)) {
	change_to_success(Paren);
      }
    }


    if (!flag) {
      NewAnsListNode(answer_node);
      aln_answer_ptr(answer_node) = Paren;
	
      NodeType(Paren) = NODE_TYPE_ANSWER_LEAF;
      if(subg_ans_list_tail(subgoal_ptr) == NULL) { 
	/* add a dummy node and the new answer*/ 
	subg_answers(subgoal_ptr) = answer_node;
	subg_ans_list_tail(subgoal_ptr) = answer_node;
      }
      else { 
	aln_next_aln(subg_ans_list_tail(subgoal_ptr)) = answer_node; 
        aln_next_aln(answer_node) = NULL; 
	subg_ans_list_tail(subgoal_ptr) = answer_node;	/* update tail */
      }
    }
    *flagptr = flag;	

    if (!flag) ans_inserts++;

    return Paren;
}

/*----------------------------------------------------------------------*/
/* for each variable in call, builds its binding on the heap.		*/
/*----------------------------------------------------------------------*/

static void load_solution_from_trie(int arity, CPtr cptr)
{
   int i;
   CPtr xtemp1, xtemp2, Dummy_Addr;
   Cell returned_val;

   for (i=0; i<arity; i++) {
     xtemp1 = (CPtr) (cptr-i);
     cptr_deref(xtemp1);
     macro_make_heap_term(xtemp1,returned_val,Dummy_Addr);
     if( xtemp1 != (CPtr)returned_val )
     { dpushtrail(xtemp1,returned_val);
       bld_ref(xtemp1,returned_val);
     }
   }
   resetpdl;
}

/*----------------------------------------------------------------------*/
void bottomupunify(Cell term, NODEptr Root, NODEptr Leaf){
  NODEptr SolnPtr;
  CPtr Dummy_Addr, xtemp2;
  Cell returned_val;
  CPtr gen;
  int  i;

  term_stackptr = -1;
  SolnPtr = Leaf;
#ifdef DEBUG_INTERN
  printf("var to be bound = _%d\n",(int)term);
#endif

  num_heap_term_vars = 0;     
  while(SolnPtr!= NULL){
    push_term((Atom(SolnPtr)));
#ifdef DEBUG_INTERN
    printf("node = ");
    print_trie_node(SolnPtr);
#endif
    SolnPtr = unftag(Parent(SolnPtr));
  }

  deref(term);
  gen = (CPtr) term;
  macro_make_heap_term(gen,returned_val,Dummy_Addr);
  bld_ref(gen,returned_val);

  for(i = 0; i < num_heap_term_vars; i++){
    var_regs[i] = var_addr[i];
  }
 num_vars_in_var_regs =  num_heap_term_vars -1;
  

#ifdef DEBUG_INTERN
  deref(returned_val);
  printf("Retd val =");
  printterm((Cell)returned_val,1,25);
  printf(" ");
  printf(" *gen = %d\n",(int)*gen);
#endif
}
/*----------------------------------------------------------------------*/
void     bottom_up_unify(){
  Cell term;
  NODEptr root;
  NODEptr leaf;
  int     rootidx;
  extern  NODEptr *Set_ArrayPtr;

  term    = ptoc_tag(1);
  rootidx = ptoc_int(2);
  root    = Set_ArrayPtr[rootidx];  
  leaf    = (NODEptr) ptoc_int(3);   
  bottomupunify(term, root, leaf);

}
/*----------------------------------------------------------------------*/

void load_solution_trie(int arity, CPtr cptr, NODEptr TriePtr)
{
   if (arity) {
     num_heap_term_vars = 0;
     follow_par_chain(unftag(TriePtr));
     load_solution_from_trie(arity,cptr);
   }
}

/*----------------------------------------------------------------------*/

#define recvariant_call(flag)\
{\
  int  j;\
\
  while (!pdlempty) {\
    xtemp1 = (CPtr) pdlpop;\
    cptr_deref(xtemp1);\
    switch(tag = cell_tag(xtemp1)) {\
      case FREE: case REF1: \
        if(is_VarEnumerator(xtemp1)){\
	*(--VarPosReg) = (Cell) xtemp1;\
	/* printf("var[%lx] at %lx is %d\n", ctr, VarPosReg-1, xtemp1); */\
	bld_nth_var(xtemp1,ctr);\
	one_node_chk_ins(flag,maketrievar((ctr | 0x10000)));\
	ctr++;\
				    }\
      else{\
	one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));\
	 }\
        break;\
      case STRING: case INT: case FLOAT:\
	one_node_chk_ins(flag, (Cell)xtemp1);\
	break;\
      case LIST:\
	one_node_chk_ins(flag, (Cell)LIST);\
	pdlpush( cell(clref_val(xtemp1)+1) ); /* changed */\
        pdlpush( cell(clref_val(xtemp1)) );\
	break;\
      case CS:\
	one_node_chk_ins(flag,makecs(follow(cs_val(xtemp1)))); /*put root in trie */\
	for(j=get_arity((Psc)follow(cs_val(xtemp1))); j>=1; j--) {\
	  pdlpush(cell(clref_val(xtemp1)+j));\
	}\
	break;\
      default: \
        xsb_abort("Bad type tag in recvariant_call...\n");\
    }\
  }\
  resetpdl;\
}
/******************************************************/

void variant_call_search(int arity, CPtr cptr, CPtr *curcallptr, int *flagptr)
{
	CPtr xtemp1;
	int i,j,flag = 1;
	Cell tag=0;
	CPtr tVarPosReg;

	subg_chk_ins++;

	tVarPosReg = VarPosReg;
        ctr = 0;
	Paren = NOPAR;
	UglyHackForTip = (tab_inf_ptr)*curcallptr;
	GNodePtrPtr = (NODEptr *)&(ti_call_trie_root(*curcallptr));

	for (i=1; i<=arity; i++) {
	  xtemp1 = (CPtr) (cptr + i);            /* Note! */
	  cptr_deref(xtemp1);
	  tag =cell_tag(xtemp1);
	  switch (tag) {
	    case FREE: case REF1:
	        if(is_VarEnumerator(xtemp1)){
		  *(--VarPosReg) = (Cell) xtemp1;
		  bld_nth_var(xtemp1,ctr);
		  one_node_chk_ins(flag,maketrievar((ctr | 0x10000)));
		  ctr++;
		}
		else{
		  one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));
		}
                break;
	      case STRING: case INT: case FLOAT:
		one_node_chk_ins(flag, (Cell)xtemp1);
		break;
	      case LIST:
		one_node_chk_ins(flag, (Cell)LIST);
	        pdlpush(cell(clref_val(xtemp1)+1));/* changed */
	        pdlpush(cell(clref_val(xtemp1)));
		recvariant_call(flag);
		break;
	    case CS:
		one_node_chk_ins(flag,makecs(follow(cs_val(xtemp1))));
                /*put root in trie */
		for (j=get_arity((Psc)follow(cs_val(xtemp1))); j>=1 ; j--) {
		  pdlpush(cell(clref_val(xtemp1)+j));
		}
		recvariant_call(flag);
		break;
  	    default:
		xsb_abort("Bad type tag in variant_call_search...\n");
	    }
        }
        resetpdl;
	*(--VarPosReg) = ctr;
	while(--tVarPosReg > VarPosReg) {
		bld_ref(((CPtr)(*tVarPosReg)),(Cell)*tVarPosReg);
	}
	/* Simplified Untrailing of Variables */
	if(arity == 0){
		one_node_chk_ins(flag, (Cell)0);
		Instr(Paren) = trie_proceed;
	}
	else{
	      Parent(Paren) = makeftag(Parent(Paren));
	      if ((flag == 0) && (tag == STRING || tag == INT || tag == FLOAT)) {
		change_to_success(Paren);
              }    
	}

	if (!flag) subg_inserts++;
        *curcallptr = (CPtr) (&(Child(Paren)));
	*flagptr = flag;
}

/*----------------------------------------------------------------------*/

static void remove_calls_and_returns(SGFrame CallStrPtr)
{
  tab_inf_ptr TipPtr;
  ALPtr AListPtr, TAlistPtr;

  TipPtr = subg_tip_ptr(CallStrPtr);
  delete_branch(subg_leaf_ptr(CallStrPtr),(CPtr)&ti_call_trie_root(TipPtr));
  AListPtr = subg_answers(CallStrPtr);
  while(AListPtr != NULL){
    TAlistPtr = AListPtr;
    AListPtr = aln_next_aln(AListPtr);
    delete_branch(aln_answer_ptr(TAlistPtr),(CPtr)&subg_ans_root_ptr(CallStrPtr));
  }
  free_subgoal_frame(CallStrPtr);
}

/*----------------------------------------------------------------------*/

void remove_open_tries(CPtr bottom_parameter)
{
  bool warned = FALSE;
  SGFrame CallStrPtr;

  while (openreg < bottom_parameter) {
    CallStrPtr =(SGFrame) compl_subgoal_ptr(openreg);
    if (!is_completed(CallStrPtr)) {
      if (warned == FALSE) {
	xsb_warn("Removing incomplete tables...");
	warned = TRUE;
      }
      remove_calls_and_returns(CallStrPtr);
    }
    openreg += COMPLFRAMESIZE;
  }
}
/*----------------------------------------------------------------------*/

NODEptr whole_term_chk_ins(Cell term, CPtr hook, int *flagptr)
{
  CPtr xtemp1;
  int j, flag = 1;
  Cell tag = 0;
  CPtr *trie_tr_base, *temp_trreg;

  xtemp1 = (CPtr) term;
  cptr_deref(xtemp1);
  tag = cell_tag(xtemp1);
  Paren = NOPAR;
  GNodePtrPtr = (NODEptr *) hook;
    if(trfreg > trreg)
	trie_tr_base = temp_trreg = trfreg;
    else
	trie_tr_base = temp_trreg = trreg;
  ctr = 0;

  switch (tag) {
  case FREE: case REF1:
    if(is_VarEnumerator(xtemp1)){
      /*one_node_chk_ins(flag,FREE);*/
      simple_dbind_ref_nth_var(xtemp1,ctr);
      one_node_chk_ins(flag, maketrievar((ctr | 0x10000)));
      ctr++;
    }
    else{
      one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));
    }
    break;
  case STRING: case INT:  case FLOAT:
    one_node_chk_ins(flag, (Cell)xtemp1);
    break;
  case LIST:
    one_node_chk_ins(flag, (Cell)LIST);
    pdlpush(cell(clref_val(xtemp1)+1)); /* changed */
    pdlpush(cell(clref_val(xtemp1)));
    recvariant_trie(flag);
    break;
  case CS:
    /* put root in trie */
    one_node_chk_ins(flag, makecs(follow(cs_val(xtemp1))));
    for (j = get_arity((Psc)follow(cs_val(xtemp1))); j >= 1 ; j--) {
      pdlpush(cell(clref_val(xtemp1)+j));
    }
    recvariant_trie(flag);
    break;
  default:
    xsb_abort("Bad type tag: one_term_check_ins\n");
  }            

    /* if there is no parent node an ESCAPE node has to be created;	*/
    /* ow the parent node is "ftagged" to indicate that it is a leaf.	*/
    if (Paren == NOPAR) {
      one_node_chk_ins(flag, (Cell)0);
      Instr(Paren) = trie_proceed;
    } else {
      Parent(Paren) = makeftag(Parent(Paren));
      /* NOTE: for structures and lists tag can be set by recvariant_trie */
      if ((flag == 0) && (tag == STRING || tag == INT || tag == FLOAT)) {
	change_to_success(Paren);
      }
    }
    *flagptr = flag;	


  for(j = 0; j < ctr; j++){
    var_regs[j] = (CPtr)trie_tr_base [j + 1];
  }
 num_vars_in_var_regs = ctr -1;
  simple_table_undo_bindings;


    return(Paren);
}


/**************************************************************************
* one_term_chk_ins(termptr,hook,flag)
**************************************************************************/

NODEptr one_term_chk_ins(CPtr termptr,CPtr hook,int *flagptr)
{
    int  arity;
    CPtr cptr;
    CPtr *trie_tr_base, *temp_trreg;
    CPtr xtemp1;
    int i, j, flag = 1;
    Cell tag = 0;
    Psc psc;

    psc = term_psc((prolog_term)termptr);
    arity = get_arity(psc);
    cptr = (CPtr)cs_val(termptr);
    if(trfreg > trreg)
	trie_tr_base = temp_trreg = trfreg;
    else
	trie_tr_base = temp_trreg = trreg;
    ctr = 0;
    Paren = NOPAR;
    GNodePtrPtr = (NODEptr *) hook;
    for (i = 1; i<=arity; i++) {
      xtemp1 = (CPtr) (cptr + i);
      cptr_deref(xtemp1);
      tag = cell_tag(xtemp1);
      switch (tag) {
	case FREE: case REF1:
	if(is_VarEnumerator(xtemp1)){
	  /*one_node_chk_ins(flag,FREE);*/
	  simple_dbind_ref_nth_var(xtemp1,ctr);
	  one_node_chk_ins(flag, maketrievar((ctr | 0x10000)));
	  ctr++;
	}
	else{
          one_node_chk_ins(flag,maketrievar(trie_var_num(xtemp1)));
	}
          break;
	case STRING: case INT:  case FLOAT:
	  one_node_chk_ins(flag, (Cell)xtemp1);
	  break;
	case LIST:
	  one_node_chk_ins(flag, (Cell)LIST);
	  pdlpush(cell(clref_val(xtemp1)+1)); /* changed */
	  pdlpush(cell(clref_val(xtemp1)));
	  recvariant_trie(flag);
	  break;
	case CS:
          /* put root in trie */
	  one_node_chk_ins(flag, makecs(follow(cs_val(xtemp1))));
	  for (j = get_arity((Psc)follow(cs_val(xtemp1))); j >= 1 ; j--) {
	    pdlpush(cell(clref_val(xtemp1)+j));
	  }
	  recvariant_trie(flag);
	  break;
	default:
	  xsb_abort("Bad type tag: one_term_check_ins\n");
	}                                                       
    }                
    resetpdl;                                                   

    simple_table_undo_bindings;

    /* if there is no parent node an ESCAPE node has to be created;	*/
    /* ow the parent node is "ftagged" to indicate that it is a leaf.	*/
    if (Paren == NOPAR) {
      one_node_chk_ins(flag, (Cell)0);
      Instr(Paren) = trie_proceed;
    } else {
      Parent(Paren) = makeftag(Parent(Paren));
      /* NOTE: for structures and lists tag can be set by recvariant_trie */
      if ((flag == 0) && (tag == STRING || tag == INT || tag == FLOAT)) {
	change_to_success(Paren);
      }
    }
    *flagptr = flag;	
    return(Paren);
}
/*************************************************/

int trie_get_returns_for_call(void)
{
  SGFrame call_str_ptr;
  CPtr retskel,term1;
  int i;
  Psc psc_ptr;
  NODEptr ans_root_ptr;
  CPtr cptr;
  

  call_str_ptr = (SGFrame) ptoc_int(1);
  retskel = (CPtr)ptoc_tag(2);

  if((ans_root_ptr = subg_ans_root_ptr(call_str_ptr))== NULL)
	return FALSE;
  else{ 
    pcreg = (byte *)ans_root_ptr;
    term1 = retskel;
    cptr_deref(term1);
    if(cell_tag(term1) == CS){
      psc_ptr = get_str_psc(retskel);
      reg_arrayptr = reg_array -1;
      num_vars_in_var_regs = -1;
      cptr = (CPtr)cs_val(retskel);
      for(i = get_arity(psc_ptr); i >=1; i--){
	pushreg(cell(cptr+i));
      }
    }
    return TRUE;
  }
}
/****************************************************/

int trie_get_calls(void)
{
   Cell call_term;
   Psc psc_ptr;
   int i;
   tab_inf_ptr tip_ptr;
   CPtr cptr, call_trie_root;

   call_term =  ptoc_tag(1);
   if((  psc_ptr = term_psc(call_term)) != NULL){
     tip_ptr = get_tip(psc_ptr);
     if(tip_ptr == NULL){
       xsb_abort("get_calls/3 called with non-tabled predicate");
       return FALSE; 
     }
     call_trie_root =ti_call_trie_root(tip_ptr);
     if(call_trie_root == NULL)
       return FALSE;
     else{
       cptr = (CPtr)cs_val(call_term);
       reg_arrayptr = reg_array  -1;
       num_vars_in_var_regs = -1;
       for (i = get_arity(psc_ptr); i >=1; i--) {
	 pushreg(cell(cptr+i));
       }
       pcreg = (byte *)call_trie_root;
       return TRUE;
     }
   }
   else
     return FALSE;

}
/*************************************************/
void aux_call_info(void)
{
    ctop_int(1, (Integer)Last_Nod_Sav);
    ctop_int(2, num_vars_in_var_regs + 1);
    ctop_int(3, (Integer)&var_regs[0]);
}
/*************************************/
void make_cell_as_desired(void)
{
  int i;
  i = ptoc_int(1);
  ctop_tag(2, (Cell)var_regs[i]);
}
/************************************/
void prolog_newnode(void) 
{
  NODEptr i;
  
  /* Used only in one context hence this abuse */
  NewNode(i,(Cell)0,NULL,NULL,NULL);
  ctop_int(1, (Integer)i);
}

/* jf: 102695 create an empty (dummy) answer */
ALPtr empty_return() 
{
  ALPtr i;
  
  /* Used only in one context hence this abuse */
  NewAnsListNode(i);
  aln_answer_ptr(i) = &dummy_ans_node;
  aln_next_aln(i) = NULL;
  return i;
}