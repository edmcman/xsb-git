:- export empty_db/1, is_db/1, assert_in_db/3, assert1_in_db/3, assert_in_db/2,
   retractall_in_db/3, clause_in_db/3, fact_in_db/2, call_in_db/2, load_in_db/3,
   load_in_db/2, subset_db/2, subsumed_db/2, union_db/3, intersect_db/3, difference_db/3,
   sym_diff_db/3, disjoint_db/2, dump_db/2, pure_call_in_db/2.

:- export empty_dbi/1, is_dbi/1, assert_in_dbi/3, assert1_in_dbi/3, assert_in_dbi/2,
   retractall_in_dbi/3, clause_in_dbi/3, fact_in_dbi/2, call_in_dbi/2, load_in_dbi/3,
   load_in_dbi/2, subset_dbi/2, subsumed_dbi/2, union_dbi/3, intersect_dbi/3, difference_dbi/3,
   sym_diff_dbi/3, disjoint_dbi/2, dump_dbi/2, pure_call_in_dbi/2.

:- export apply_op_in_db/6, apply_op_in_dbi/6.
:- export apply_rel_op_in_db/4, apply_rel_op_in_dbi/4.
:- export union_in_db/5, union_in_dbi/5.
:- export intersect_in_db/5, intersect_in_dbi/5.
:- export difference_in_db/5, difference_in_dbi/5.
:- export sym_diff_in_db/5, sym_diff_in_dbi/5.
:- export subset_in_db/3, subset_in_dbi/3.
:- export subsumed_in_db/3, subsumed_in_dbi/3.
:- export disjoint_in_db/3, disjoint_in_dbi/3.
:- export equal_in_db/3, equal_in_dbi/3.
:- export copy_in_db/4, copy_in_dbi/4.
:- export materialize_in_db/4, materialize_in_dbi/4.

:- import close/1, writeln/2, write_canonical/2, '=..'/2,
   functor/3, open/3, throw/1, call/1, read/2
   from standard.
:- import ','/2, ';'/2, (:)/2, is_most_general_term/1 from standard.

:- import findall/3, term_variables/2 from setof.
:- import (tnot)/1 from tables.
:- import numbervars/3 from num_vars.
:- import copy_term/2, length/2 from basics.

:- import subsumes_chk/2 from subsumes.
:- import intern_termhash/2 from machine.
:- import phrase/3 from dcg.
   
:- import misc_error/1 from error_handler.
