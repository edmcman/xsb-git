#! /bin/csh

#set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
set XEMU = $argv[1]
echo "-------------------------------------------------------"
echo " running testptq.csh "
echo "-------------------------------------------------------"
csh genlredscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheilredemu  $XEMU
csh diffall.csh mbjfauaheilredemu mbjfauaheilredint



