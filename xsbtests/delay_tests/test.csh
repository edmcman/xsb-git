echo "-------------------------------------------------------"
echo "--- Running delay_tests/test.csh                    ---"
echo "-------------------------------------------------------"
#============================================================================
set XEMU = $argv[1]
#============================================================================
#----------------------------------------------------------------------
# Tests of dynamically stratified negation.
#----------------------------------------------------------------------
gentest.csh $XEMU dynstrat1 "test."	# Needs n simplification (OK)
gentest.csh $XEMU dynstrat2 "test."	# No simplification (OK)
gentest.csh $XEMU dynstrat3 "test."	# Needs p+n simplification (OK)
gentest.csh $XEMU dynstrat4 "test."	# Needs n simplification (OK)
gentest.csh $XEMU dynstrat5 "test."	# Needs n simplification (OK)
gentest.csh $XEMU dynstrat6 "test."	# Needs n simplification (OK)
gentest.csh $XEMU dynstrat7 "test."	# Needs n simplification (OK)
gentest.csh $XEMU ross1 "test."		# No simplification (OK)
gentest.csh $XEMU sel_unsusp "test."	# Tests selective unsuspension
					# Needs n simplification (OK)
gentest.csh $XEMU dl_dupl "test."	# Needs p+n simplification (OK)
gentest.csh $XEMU asl_dupl "test."	# Needs n simplification (OK)
gentest.csh $XEMU gfp "test."		# Needs cascading simplifications (OK)
#----------------------------------------------------------------------
# Tests that at some early point in the development gave false results.
#----------------------------------------------------------------------
gentest.csh $XEMU fr1 "test."		#
gentest.csh $XEMU fr2 "test."		#
gentest.csh $XEMU fr3 "test."		#
gentest.csh $XEMU fr4 "test."		#
gentest.csh $XEMU fr5 "test."		#
gentest.csh $XEMU fr6 "test."		#
gentest.csh $XEMU fr7 "test."		#
gentest.csh $XEMU fr8 "test."		#
gentest.csh $XEMU fr9 "test."		#
gentest.csh $XEMU fr19 "test."		#
gentest.csh $XEMU fr20 "test."		#
gentest.csh $XEMU fr21 "test."		#
gentest.csh $XEMU fr22 "test."		#
gentest.csh $XEMU fr23 "test."		#
gentest.csh $XEMU fr25 "test."		#
gentest.csh $XEMU fr26 "test."		#
gentest.csh $XEMU fr27 "test."		#
gentest.csh $XEMU fr28 "test."		#
gentest.csh $XEMU fr29 "test."		# shows need for undeleting answers (OK)
gentest.csh $XEMU fr30 "test."		# shows need for junking answers (OK)
#----------------------------------------------------------------------
# Tests that at some early point in the development caused
# segmentation faults.
#----------------------------------------------------------------------
gentest.csh $XEMU seg1 "test."		# (OK)
gentest.csh $XEMU seg2 "test."		# (OK)
gentest.csh $XEMU seg3 "test."		# (OK)
gentest.csh $XEMU seg4 "test."		# (OK)
gentest.csh $XEMU seg5 "test."		# Only when interpreted (OK)
gentest.csh $XEMU fr24 "test."		# Needed restoration of ptcpreg (OK)
#----------------------------------------------------------------------
# Tests from David's automatic generation meta-interpreter that at
# some early point in the development caused segmentation faults.
#----------------------------------------------------------------------
gentest.csh $XEMU interp0 "test."	#
gentest.csh $XEMU interp1 "test."	#
gentest.csh $XEMU interp2 "test."	# was giving wrong results (OK)
#gentest.csh $XEMU interp3 "test."	# was causing an infinite loop
gentest.csh $XEMU interp4 "test."	# showed need for ptcpreg in Prolog CPs
gentest.csh $XEMU interp5 "test."	#
gentest.csh $XEMU interp6 "test."	#
gentest.csh $XEMU interp7 "test."	#
gentest.csh $XEMU interp8 "test."	#
gentest.csh $XEMU interp9 "test."	#
gentest.csh $XEMU interp10 "test."	#
gentest.csh $XEMU interp11 "test."	# was giving wrong results (OK)
#----------------------------------------------------------------------
# Genome tests involving unclassified negation.
#----------------------------------------------------------------------
#	gentest.csh $XEMU q6 "test."
#----------------------------------------------------------------------
# Tests of non-stratified negation.
#----------------------------------------------------------------------
gentest.csh $XEMU two_ary "test."
gentest.csh $XEMU abol_susp1 "test."	# Tests abolishing suspensions (OK)
gentest.csh $XEMU abol_susp2 "test."	# Tests abolishing suspensions (OK)
gentest.csh $XEMU przy1 "test."		# No simplification (OK)
gentest.csh $XEMU przy1_simp "test."	# Needs n simplification (OK)
gentest.csh $XEMU nonstrat1 "test."	# No simplification (OK)
gentest.csh $XEMU nonstrat2 "test."	# Needs n simplification (OK)
#----------------------------------------------------------------------
# Tests of well-founded negation.
#----------------------------------------------------------------------
gentest.csh $XEMU p1 "test."		# No simplification (OK)
gentest.csh $XEMU p2 "test."		# No simplification (OK)
gentest.csh $XEMU p3 "test."		# No simplification (OK)
gentest.csh $XEMU p4 "test."		# No simplification (OK)
gentest.csh $XEMU p5 "test."		# Needs n simplification (OK)
gentest.csh $XEMU simpl_win "test."	# No simplification (OK)
gentest.csh $XEMU win "test."		# Tests cond. vs uncond. answers
					# No simplification (OK)
gentest.csh $XEMU cond_uncond "test."	# Tests cond. vs uncond. answers
					# and early simplification (OK)
gentest.csh $XEMU ullman3 "test."	# Requires answers to be returned
					# from completed tables (OK)
gentest.csh $XEMU undef1 "test."	# To test printing of delay lists
gentest.csh $XEMU undef2 "test."	# To test printing of delay lists
#----------------------------------------------------------------------
# Tests of positive simplification.
#----------------------------------------------------------------------
gentest.csh $XEMU pos_simpl1 "test."	# To test printing of delay lists
#----------------------------------------------------------------------
# A mixture of negation tests.
#----------------------------------------------------------------------
#	gentest.csh $XEMU pot_pouri "test."
#----------------------------------------------------------------------
# Tests that may require answer completion.
#----------------------------------------------------------------------
#	gentest.csh $XEMU weidong1 "test."
gentest.csh $XEMU weidong2 "test."	# Requires simpl. + ans. compl. (OK)
gentest.csh $XEMU weidong3 "test."	# OK
gentest.csh $XEMU weidong4 "test."	# Requires simpl. + ans. compl. (WRONG)
gentest.csh $XEMU weidong5 "test."	# Requires answer completion (OK)
gentest.csh $XEMU weidong6 "test."	# Requires answer completion (OK)
gentest.csh $XEMU weidong7 "test."	# Requires answer completion (WRONG)
gentest.csh $XEMU weidong8 "test."	# Requires answer completion (WRONG)
#----------------------------------------------------------------------
# Tests that do require answer completion.
#----------------------------------------------------------------------
gentest.csh $XEMU interp12 "test."	# Requires answer completion (WRONG)
gentest.csh $XEMU interp13 "test."	# Requires answer completion (WRONG)
gentest.csh $XEMU interp14 "test."	# Requires answer completion (WRONG)
gentest.csh $XEMU interp15 "test."	# Requires answer completion (WRONG)
#----------------------------------------------------------------------
# Tests of floundering behaviour.
#----------------------------------------------------------------------
gentest.csh $XEMU avoid_flounder "test."
#----------------------------------------------------------------------
# Tests of residual program.
#----------------------------------------------------------------------
gentest.csh $XEMU residual1 "test."
#----------------------------------------------------------------------
# Tests of residual program and findall/3.
#----------------------------------------------------------------------
gentest.csh $XEMU fa "test."
