echo "-------------------------------------------------------"
echo "--- Running testall.csh                             ---"
echo "-------------------------------------------------------"

if -e uniq_lock then
   echo uniq_lock exists
   echo Probably somebody else is running the testsuite
   echo If not then remove uniq_lock
   echo and continue
else

echo " " > uniq_lock

#=============================================================================
set XEMU = $argv[1]
#=============================================================================
#------------------------------------
cd basic_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd prolog_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd float_tests
#/bin/rm -f core 2> /dev/null
#csh test.csh $XEMU
cd ..
#------------------------------------
cd retract_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd table_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd ptq
/bin/rm -f core 2> /dev/null
csh testptq.csh $XEMU
cd ..
#------------------------------------
cd neg_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd sem_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd delay_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------
cd wfs_tests
/bin/rm -f core 2> /dev/null
csh sanity.csh $XEMU
cd ..
#------------------------------------
cd ai_tests
/bin/rm -f core 2> /dev/null
csh test.csh $XEMU
cd ..
#------------------------------------

/bin/rm -f uniq_lock

endif
