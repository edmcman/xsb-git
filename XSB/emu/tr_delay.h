/* File:      tr_delay.h
** Author(s): Kostis Sagonas, Baoqiu Cui
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
** $Id: tr_delay.h,v 1.2 1998-12-09 03:22:57 cbaoqiu Exp $
** 
*/


/* special debug includes */
#include "debugs/debug_delay.h"

/*----- Stuff for trie instructions ------------------------------------*/

/*
 * In the execution of trie code, handle_conditional_answers is called
 * (in proceed_lpcreg) when NodePtr is an answer leaf.  If the answer is
 * a conditional one, then call delay_positively() to put it into the
 * delay list of the parent predicate.
 *
 * After the execution of trie code, the substitution factor of the
 * _answer_ is stored in array var_regs[], and the number of variables is
 * saved in num_vars_in_var_regs (-1 means there is no variable, 0 means
 * there is one variable, ...)
 *
 * Instead of saving the substitution factor of the call, we can save
 * the substitution factor of the answer in the delay element.
 */

#ifdef DELAY_DEBUG
#define handle_conditional_answers {					\
    CPtr temp_hreg;							\
    if (is_conditional_answer(NodePtr)) {				\
      fprintf(stderr, "Trie-Code returning a conditional answer for ");	\
      SUBGOAL = (CPtr) asi_subgoal((ASI) Delay(NodePtr));		\
      print_subgoal(stderr, (SGFrame) SUBGOAL);				\
      fprintf(stderr, " (positively delaying)\n");			\
      fprintf(stderr, ">>>> (in handle_conditional_answers)\
num_vars_in_var_regs = %d\n", num_vars_in_var_regs);			\
      /* create the answer subsf ret/n */				\
      temp_hreg = hreg;							\
      new_heap_functor(hreg, get_ret_psc(num_vars_in_var_regs + 1));	\
      {									\
	int i;								\
	for (i = 0; i < num_vars_in_var_regs + 1; i++) {		\
	  cell(hreg++) = (Cell) var_regs[i]; /* new */			\
	  fprintf(stderr, ">>>> var_regs[%d] = ", i);			\
	  printterm(cell(var_regs[i]), 1, 25);				\
	  printf("\n");							\
	}								\
      }									\
      delay_positively(SUBGOAL, NodePtr, temp_hreg)			\
    }									\
  }
#else
#define handle_conditional_answers {					\
    CPtr temp_hreg;							\
    if (is_conditional_answer(NodePtr)) {				\
      SUBGOAL = (CPtr) asi_subgoal((ASI) Delay(NodePtr));		\
      temp_hreg = hreg;							\
      new_heap_functor(hreg, get_ret_psc(num_vars_in_var_regs + 1));	\
      {									\
	int i;								\
	for (i = 0; i < num_vars_in_var_regs + 1; i++) {		\
	  cell(hreg++) = (Cell) var_regs[i]; /* new */			\
	}								\
      }									\
      delay_positively(SUBGOAL, NodePtr, temp_hreg)			\
    }									\
  }
#endif

/*---------------------- end of file tr_delay.h ------------------------*/
