#! /bin/sh

echo "-------------------------------------------------------"
echo "--- Running delay_tests/test.sh                     ---"
echo "-------------------------------------------------------"

XEMU=$1

#----------------------------------------------------------------------
# Tests of dynamically stratified negation.
#----------------------------------------------------------------------
../gentest.sh $XEMU dynstrat1 "test."	# Needs n simplification (OK)
../gentest.sh $XEMU dynstrat2 "test."	# No simplification (OK)
../gentest.sh $XEMU dynstrat3 "test."	# Needs p+n simplification (OK)
../gentest.sh $XEMU dynstrat4 "test."	# Needs n simplification (OK)
../gentest.sh $XEMU dynstrat5 "test."	# Needs n simplification (OK)
../gentest.sh $XEMU dynstrat6 "test."	# Needs n simplification (OK)
../gentest.sh $XEMU dynstrat7 "test."	# Needs n simplification (OK)
../gentest.sh $XEMU ross1 "test."		# No simplification (OK)
../gentest.sh $XEMU sel_unsusp "test."	# Tests selective unsuspension
					# Needs n simplification (OK)
../gentest.sh $XEMU dl_dupl "test."	# Needs p+n simplification (OK)
../gentest.sh $XEMU asl_dupl "test."	# Needs n simplification (OK)
../gentest.sh $XEMU gfp "test."		# Needs cascading simplifications (OK)
#----------------------------------------------------------------------
# Tests that at some early point in the development gave false results.
#----------------------------------------------------------------------
../gentest.sh $XEMU fr1 "test."		#
../gentest.sh $XEMU fr2 "test."		#
../gentest.sh $XEMU fr3 "test."		#
../gentest.sh $XEMU fr4 "test."		#
../gentest.sh $XEMU fr5 "test."		#
../gentest.sh $XEMU fr6 "test."		#
../gentest.sh $XEMU fr7 "test."		#
../gentest.sh $XEMU fr8 "test."		#
../gentest.sh $XEMU fr9 "test."		#
../gentest.sh $XEMU fr19 "test."	#
../gentest.sh $XEMU fr20 "test."	#
../gentest.sh $XEMU fr21 "test."	#
../gentest.sh $XEMU fr22 "test."	#
../gentest.sh $XEMU fr23 "test."	#
../gentest.sh $XEMU fr25 "test."	#
../gentest.sh $XEMU fr26 "test."	#
../gentest.sh $XEMU fr27 "test."	#
../gentest.sh $XEMU fr28 "test."	#
../gentest.sh $XEMU fr29 "test."	#shows need for undeleting answers (OK)
../gentest.sh $XEMU fr30 "test."	#shows need for junking answers (OK)
#----------------------------------------------------------------------
# Tests that at some early point in the development caused
# segmentation faults.
#----------------------------------------------------------------------
../gentest.sh $XEMU seg1 "test."		# (OK)
../gentest.sh $XEMU seg2 "test."		# (OK)
../gentest.sh $XEMU seg3 "test."		# (OK)
../gentest.sh $XEMU seg4 "test."		# (OK)
../gentest.sh $XEMU seg5 "test."		# Only when interpreted (OK)
../gentest.sh $XEMU fr24 "test."		# Needed restoration of ptcpreg (OK)
#----------------------------------------------------------------------
# Tests from David's automatic generation meta-interpreter that at
# some early point in the development caused segmentation faults.
#----------------------------------------------------------------------
../gentest.sh $XEMU interp0 "test."	#
../gentest.sh $XEMU interp1 "test."	#
../gentest.sh $XEMU interp2 "test."	# was giving wrong results (OK)
#../gentest.sh $XEMU interp3 "test."	# was causing an infinite loop
../gentest.sh $XEMU interp4 "test."	# showed need for ptcpreg in Prolog CPs
../gentest.sh $XEMU interp5 "test."	#
../gentest.sh $XEMU interp6 "test."	#
../gentest.sh $XEMU interp7 "test."	#
../gentest.sh $XEMU interp8 "test."	#
../gentest.sh $XEMU interp9 "test."	#
../gentest.sh $XEMU interp10 "test."	#
../gentest.sh $XEMU interp11 "test."	# was giving wrong results (OK)
#----------------------------------------------------------------------
# Genome tests involving unclassified negation.
#----------------------------------------------------------------------
#	gentest.csh $XEMU q6 "test."
#----------------------------------------------------------------------
# Tests of non-stratified negation.
#----------------------------------------------------------------------
../gentest.sh $XEMU two_ary "test."
../gentest.sh $XEMU abol_susp1 "test."	# Tests abolishing suspensions (OK)
../gentest.sh $XEMU abol_susp2 "test."	# Tests abolishing suspensions (OK)
../gentest.sh $XEMU przy1 "test."		# No simplification (OK)
../gentest.sh $XEMU przy1_simp "test."	# Needs n simplification (OK)
../gentest.sh $XEMU nonstrat1 "test."	# No simplification (OK)
../gentest.sh $XEMU nonstrat2 "test."	# Needs n simplification (OK)
#----------------------------------------------------------------------
# Tests of well-founded negation.
#----------------------------------------------------------------------
../gentest.sh $XEMU p1 "test."		# No simplification (OK)
../gentest.sh $XEMU p2 "test."		# No simplification (OK)
../gentest.sh $XEMU p3 "test."		# No simplification (OK)
../gentest.sh $XEMU p4 "test."		# No simplification (OK)
../gentest.sh $XEMU p5 "test."		# Needs n simplification (OK)
../gentest.sh $XEMU simpl_win "test."	# No simplification (OK)
../gentest.sh $XEMU win "test."		# Tests cond. vs uncond. answers
					# No simplification (OK)
../gentest.sh $XEMU cond_uncond "test."	# Tests cond. vs uncond. answers
					# and early simplification (OK)
../gentest.sh $XEMU ullman3 "test."	# Requires answers to be returned
					# from completed tables (OK)
../gentest.sh $XEMU undef1 "test."	# To test printing of delay lists
../gentest.sh $XEMU undef2 "test."	# To test printing of delay lists
#----------------------------------------------------------------------
# Tests of positive simplification.
#----------------------------------------------------------------------
../gentest.sh $XEMU pos_simpl1 "test."	# To test printing of delay lists
#----------------------------------------------------------------------
# A mixture of negation tests.
#----------------------------------------------------------------------
#	gentest.csh $XEMU pot_pouri "test."
#----------------------------------------------------------------------
# Tests that may require answer completion.
#----------------------------------------------------------------------
#	gentest.csh $XEMU weidong1 "test."
../gentest.sh $XEMU weidong2 "test."	# Requires simpl. + ans. compl. (OK)
../gentest.sh $XEMU weidong3 "test."	# OK
../gentest.sh $XEMU weidong4 "test."	# Requires simpl. + ans. compl. (WRONG)
../gentest.sh $XEMU weidong5 "test."	# Requires answer completion (OK)
../gentest.sh $XEMU weidong6 "test."	# Requires answer completion (OK)
../gentest.sh $XEMU weidong7 "test."	# Requires answer completion (WRONG)
../gentest.sh $XEMU weidong8 "test."	# Requires answer completion (WRONG)
#----------------------------------------------------------------------
# Tests that do require answer completion.
#----------------------------------------------------------------------
../gentest.sh $XEMU interp12 "test."	# Requires answer completion (WRONG)
../gentest.sh $XEMU interp13 "test."	# Requires answer completion (WRONG)
../gentest.sh $XEMU interp14 "test."	# Requires answer completion (WRONG)
../gentest.sh $XEMU interp15 "test."	# Requires answer completion (WRONG)
#----------------------------------------------------------------------
# Tests of floundering behaviour.
#----------------------------------------------------------------------
../gentest.sh $XEMU avoid_flounder "test."
#----------------------------------------------------------------------
# Tests of residual program.
#----------------------------------------------------------------------
../gentest.sh $XEMU residual1 "test."
#----------------------------------------------------------------------
# Tests of residual program and findall/3.
#----------------------------------------------------------------------
../gentest.sh $XEMU fa "test."
#----------------------------------------------------------------------
# Tests of variables in delay list.
#----------------------------------------------------------------------
#../gentest.sh $XEMU delay_var "test."
