:- export empty_db/1, is_db/1, assert_in_db/3, assert1_in_db/3, assert_in_db/2,
   retractall_in_db/3, clause_in_db/3, call_in_db/2, load_in_db/3, load_in_db/2,
   subset_db/2, union_db/3, intersect_db/3, difference_db/3, sym_diff_db/3,
   disjoint_db/2, dump_db/2, pure_call_in_db/2.

:- export empty_dbi/1, is_dbi/1, assert_in_dbi/3, assert1_in_dbi/3, assert_in_dbi/2,
   retractall_in_dbi/3, clause_in_dbi/3, call_in_dbi/2, load_in_dbi/3, load_in_dbi/2,
   subset_dbi/2, union_dbi/3, intersect_dbi/3, difference_dbi/3, sym_diff_dbi/3,
   disjoint_dbi/2, dump_dbi/2, pure_call_in_dbi/2.

:- import close/1, writeln/2, write_canonical/2, '=..'/2,
   functor/3, open/3, throw/1, call/1, read/2
   from standard.

:- import findall/3 from setof.
   :- import ','/2, ';'/2 from standard.
:- import numbervars/3 from num_vars.
:- import copy_term/2, length/2 from basics.

:- import (:)/2 from standard.  %??

:- import intern_termhash/2 from machine.
