#! /bin/csh
set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
echo "-------------------------------------------------------"
echo " running testptq.csh "
echo "-------------------------------------------------------"
csh genscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiemu  $XEMU
csh diffall.csh mbjfauaheiemu mbjfauaheiint
csh genscaff.csh "tran([john,dates,mary])" jdmemu $XEMU
csh diffall.csh jdmemu jdmint
csh genscaff.csh "tran([a,price,rises])" apremu $XEMU
csh diffall.csh apremu aprint
csh genscaff.csh "tran([john,dates,him])" jdhemu $XEMU
csh diffall.csh jdhemu jdhint
csh genscaff.csh "tran([john,walks,in,a,park])" jwiapemu $XEMU
csh diffall.csh jwiapemu jwiapint
csh genscaff.csh "tran([john,finds,a,unicorn])" jfauemu $XEMU
csh diffall.csh jfauemu jfauint
csh genscaff.csh "tran([john,seeks,a,unicorn])" jsauemu $XEMU
csh diffall.csh jsauemu jsauint
csh genscaff.csh "tran([a,man,tries_to,walk])" amtwemu $XEMU
csh diffall.csh amtwemu amtwint
csh genscaff.csh "tran([a,woman,tries_to,run,allegedly])" awtraemu $XEMU
csh diffall.csh awtraemu awtraint
csh genscaff.csh "tran([john,talks,about,a,unicorn])" jtauemu $XEMU
csh diffall.csh jtauemu jtauint
csh genscaff.csh "tran([every,man,loves,a,woman])" emlawemu $XEMU
csh diffall.csh emlawemu emlawint
csh genscaff.csh "tran([a,woman,such_that,she,walks,runs])" awsswremu $XEMU
csh diffall.csh awsswremu awsswrint
csh genscaff.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
csh diffall.csh jsauamsiemu jsauamsiint
echo "************************************************************"
echo "Testing Lreds"
csh genlredscaff.csh "tran([john,dates,mary])" jdmlredemu $XEMU
csh diffall.csh jdmlredemu jdmlredint
csh genlredscaff.csh "tran([a,price,rises])" aprlredemu $XEMU
csh diffall.csh aprlredemu aprlredint
csh genlredscaff.csh "tran([john,dates,him])" jdhlredemu $XEMU
csh diffall.csh jdhlredemu jdhlredint
csh genlredscaff.csh "tran([john,walks,in,a,park])" jwiaplredemu $XEMU
csh diffall.csh jwiaplredemu jwiaplredint
csh genlredscaff.csh "tran([john,finds,a,unicorn])" jfaulredemu $XEMU
csh diffall.csh jfaulredemu jfaulredint
csh genlredscaff.csh "tran([john,seeks,a,unicorn])" jsaulredemu $XEMU
csh diffall.csh jsaulredemu jsaulredint
csh genlredscaff.csh "tran([a,man,tries_to,walk])" amtwlredemu $XEMU
csh diffall.csh amtwlredemu amtwlredint
csh genlredscaff.csh "tran([a,woman,tries_to,run,allegedly])" awtralredemu $XEMU
csh diffall.csh awtralredemu awtralredint
csh genlredscaff.csh "tran([john,talks,about,a,unicorn])" jtaulredemu $XEMU
csh diffall.csh jtaulredemu jtaulredint
csh genlredscaff.csh "tran([every,man,loves,a,woman])" emlawlredemu $XEMU
csh diffall.csh emlawlredemu emlawlredint
csh genlredscaff.csh "tran([a,woman,such_that,she,walks,runs])" awsswrlredemu $XEMU
csh diffall.csh awsswrlredemu awsswrlredint
csh genlredscaff.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
csh diffall.csh jsauamsiemu jsauamsilredint
csh genlredscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheilredemu  $XEMU
csh diffall.csh mbjfauaheilredemu mbjfauaheilredint
csh genlredscaff.csh "tran([john,believes_that,mary,walks,in,a,park])" jbmwiaplredemu $XEMU
csh diffall.csh jbmwiaplredemu jbmwiaplredint
csh genlredscaff.csh "tran([john,believes_that,mary,wishes_to,walk,in,a,park])" jbmwwaiplredemu $XEMU
csh diffall.csh jbmwwaiplredemu jbmwwaiplredint
csh genlredscaff.csh "tran([john,wishes_to,find,a,unicorn,and,eat,it])" jwfauaeilredemu $XEMU
csh diffall.csh jwfauaeilredemu jwfauaeilredint
csh genlredscaff.csh "tran([john,tries_to,find,a,unicorn,and,wishes_to,eat,it])" jtfauaweilredemu $XEMU
csh diffall.csh jtfauaweilredemu jtfauaweilredint
csh genlredscaff.csh "tran([every,man,loves,a,woman,such_that,she,loves,him])" emlawsslhlredemu $XEMU
csh diffall.csh emlawsslhlredemu emlawsslhlredint
csh genlredscaff.csh "tran([every,man,loves,a,woman,and,she,loves,him])" emlawaslhlredemu $XEMU
csh diffall.csh emlawaslhlredemu emlawaslhlredint

#csh genlredscaff.csh "tran([every,man,loves,a,woman,in,a,park,voluntarily,and,every,fish,eats,a,pen,slowly])" emlawiapvaefeapslredemu
#csh diffall.csh emlawiapvaefeapslredemu emlawiapvaefeapslredint
