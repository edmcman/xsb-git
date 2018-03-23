:- export new_db/1, is_db/1, assert_in_db/3, assert1_in_db/3,
   assert_in_db/2, retractall_in_db/3, clause_in_db/3, fact_in_db/2,
   call_in_db/2, load_in_db/3, load_in_db/2, subset_db/2, subsumed_db/2, size_db/2,
   union_db/3, intersect_db/3, difference_db/3, sym_diff_db/3, disjoint_db/2,
   is_empty_db/1, dump_db/2, pure_call_in_db/2, load_in_db/4.

:- export cvt_db_to_i/2, cvt_db_from_i/2.

:- export new_dbi/1, load_in_dbi/2, assert_in_dbi/2, new_db_like/2.

:- export union_in_db/5,
   intersect_in_db/5, difference_in_db/5, sym_diff_in_db/5,
   subset_in_db/3, subsumed_in_db/3, disjoint_in_db/3, equal_in_db/3,
   copy_in_db/4, materialize_in_db/4, xprod_in_db/5, join_in_db/6,
   reorder_in_db/4, project_in_db/5, is_empty_in_db/2, move_in_db/4.
:- export count_in_db/3.

/* The following naive-* predicates are for testing only and should in
      general not be used, as they are less efficient than the other
      predicates. */
:- comment(hide,[naive_subset_db/2,naive_union_db/3,naive_intersect_db/3, naive_difference_db/3, naive_disjoint_db/2, naive_sym_diff_db/3]).

:- export naive_subset_db/2, naive_union_db/3, naive_intersect_db/3.
:- export naive_difference_db/3.
:- export naive_disjoint_db/2.
:- export naive_sym_diff_db/3.

:- import gensym/2, conset/2, coninc/1, conget/2 from gensym.
:- import close/1, writeln/2, '=..'/2,
   functor/3, functor/4, open/3, call/1, read/2
   from standard.
:- import ','/2, ';'/2, (:)/2, is_most_general_term/1 from standard.
:- import clause/2, assert/1, retractall/1 from assert.

:- import findall/3, term_variables/2 from setof.
:- import (tnot)/1 from tables.
:- import numbervars/3 from num_vars.
:- import copy_term/2, length/2 from basics.

:- import subsumes_chk/2 from subsumes.
:- import phrase/3 from dcg.
   
:- import misc_error/1 from error_handler.

:- import write_canonical_lettervar/2 from xsb_writ.
