#set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
set XEMU = $argv[1]
echo "-------------------------------------------------------"
echo " running testptq.csh "
echo "-------------------------------------------------------"
csh genscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiemu  $XEMU
csh diffall.csh mbjfauaheiemu mbjfauaheiint
