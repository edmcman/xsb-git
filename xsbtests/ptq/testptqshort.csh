#set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
set XEMU = $argv[1]
echo "-------------------------------------------------------"
echo " running testptq.csh "
echo "-------------------------------------------------------"
csh genscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiemu  $XEMU
csh diffall.csh mbjfauaheiemu mbjfauaheiint
csh genlredscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheilredemu  $XEMU
csh diffall.csh mbjfauaheilredemu mbjfauaheilredint
csh genlredscaff.csh "tran([john,believes_that,mary,walks,in,a,park])" jbmwiaplredemu $XEMU
csh diffall.csh jbmwiaplredemu jbmwiaplredint
csh genlredscaff.csh "tran([john,talks,about,a,unicorn])" jtaulredemu $XEMU
csh diffall.csh jtaulredemu jtaulredint
echo "end short *********************************************************"
