#! /bin/sh

XEMU=$argv[1]
echo "-------------------------------------------------------"
echo "---   Running ptq/test.sh                           ---"
echo "-------------------------------------------------------"

#ensure parser.P is compiled
$XEMU -m 4000 -c 2000 -i << EOF
[parser].
halt.
EOF

./genlredscaff.csh "tran([john,talks,about,a,unicorn])" jtaulredemu $XEMU
./diffall.csh jtaulredemu jtaulredint
./genscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiemu  $XEMU
./diffall.csh mbjfauaheiemu mbjfauaheiint
./genlredscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheilredemu  $XEMU
./diffall.csh mbjfauaheilredemu mbjfauaheilredint
echo "************************************************************"
echo "End Short"
./genscaff.csh "tran([john,talks,about,a,unicorn])" jtauemu $XEMU
./diffall.csh jtauemu jtauint
./genscaff.csh "tran([john,dates,mary])" jdmemu $XEMU
./diffall.csh jdmemu jdmint
./genscaff.csh "tran([a,price,rises])" apremu $XEMU
./diffall.csh apremu aprint
./genscaff.csh "tran([john,dates,him])" jdhemu $XEMU
./diffall.csh jdhemu jdhint
#./genscaff.csh "tran([john,walks,in,a,park])" jwiapemu $XEMU
#./diffall.csh jwiapemu jwiapint
#./genscaff.csh "tran([john,finds,a,unicorn])" jfauemu $XEMU
#./diffall.csh jfauemu jfauint
./genscaff.csh "tran([john,seeks,a,unicorn])" jsauemu $XEMU
./diffall.csh jsauemu jsauint
#./genscaff.csh "tran([a,man,tries_to,walk])" amtwemu $XEMU
#./diffall.csh amtwemu amtwint
./genscaff.csh "tran([a,woman,tries_to,run,allegedly])" awtraemu $XEMU
./diffall.csh awtraemu awtraint
./genscaff.csh "tran([every,man,loves,a,woman])" emlawemu $XEMU
./diffall.csh emlawemu emlawint
./genscaff.csh "tran([a,woman,such_that,she,walks,runs])" awsswremu $XEMU
./diffall.csh awsswremu awsswrint
./genscaff.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
./diffall.csh jsauamsiemu jsauamsiint
echo "************************************************************"
echo "Testing Lreds"
./genlredscaff.csh "tran([john,dates,mary])" jdmlredemu $XEMU
./diffall.csh jdmlredemu jdmlredint
#./genlredscaff.csh "tran([a,price,rises])" aprlredemu $XEMU
#./diffall.csh aprlredemu aprlredint
./genlredscaff.csh "tran([john,dates,him])" jdhlredemu $XEMU
./diffall.csh jdhlredemu jdhlredint
./genlredscaff.csh "tran([john,walks,in,a,park])" jwiaplredemu $XEMU
./diffall.csh jwiaplredemu jwiaplredint
./genlredscaff.csh "tran([john,finds,a,unicorn])" jfaulredemu $XEMU
./diffall.csh jfaulredemu jfaulredint
./genlredscaff.csh "tran([john,seeks,a,unicorn])" jsaulredemu $XEMU
./diffall.csh jsaulredemu jsaulredint
#./genlredscaff.csh "tran([a,man,tries_to,walk])" amtwlredemu $XEMU
#./diffall.csh amtwlredemu amtwlredint
./genlredscaff.csh "tran([a,woman,tries_to,run,allegedly])" awtralredemu $XEMU
./diffall.csh awtralredemu awtralredint
./genlredscaff.csh "tran([every,man,loves,a,woman])" emlawlredemu $XEMU
./diffall.csh emlawlredemu emlawlredint
./genlredscaff.csh "tran([a,woman,such_that,she,walks,runs])" awsswrlredemu $XEMU
./diffall.csh awsswrlredemu awsswrlredint
./genlredscaff.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
./diffall.csh jsauamsiemu jsauamsilredint
#./genlredscaff.csh "tran([john,believes_that,mary,walks,in,a,park])" jbmwiaplredemu $XEMU
#./diffall.csh jbmwiaplredemu jbmwiaplredint
./genlredscaff.csh "tran([john,believes_that,mary,wishes_to,walk,in,a,park])" jbmwwaiplredemu $XEMU
./diffall.csh jbmwwaiplredemu jbmwwaiplredint
./genlredscaff.csh "tran([john,wishes_to,find,a,unicorn,and,eat,it])" jwfauaeilredemu $XEMU
./diffall.csh jwfauaeilredemu jwfauaeilredint
./genlredscaff.csh "tran([john,tries_to,find,a,unicorn,and,wishes_to,eat,it])" jtfauaweilredemu $XEMU
./diffall.csh jtfauaweilredemu jtfauaweilredint
./genlredscaff.csh "tran([every,man,loves,a,woman,such_that,she,loves,him])" emlawsslhlredemu $XEMU
./diffall.csh emlawsslhlredemu emlawsslhlredint
./genlredscaff.csh "tran([every,man,loves,a,woman,and,she,loves,him])" emlawaslhlredemu $XEMU
./diffall.csh emlawaslhlredemu emlawaslhlredint

#./genlredscaff.csh "tran([every,man,loves,a,woman,in,a,park,voluntarily,and,every,fish,eats,a,pen,slowly])" emlawiapvaefeapslredemu
#./diffall.csh emlawiapvaefeapslredemu emlawiapvaefeapslredint

