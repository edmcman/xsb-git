#! /bin/csh

set XEMU = $1

echo "-------------------------------------------------------"
echo " running testptq.csh "
echo "-------------------------------------------------------"
csh genlredprof.csh "tran([john,talks,about,a,unicorn])" jtaulredemu $XEMU
csh genprof.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiemu  $XEMU
csh genlredprof.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheilredemu  $XEMU
csh genprof.csh "tran([john,talks,about,a,unicorn])" jtauemu $XEMU
csh genprof.csh "tran([john,dates,mary])" jdmemu $XEMU
csh genprof.csh "tran([a,price,rises])" apremu $XEMU
csh genprof.csh "tran([john,dates,him])" jdhemu $XEMU
csh genprof.csh "tran([john,walks,in,a,park])" jwiapemu $XEMU
csh genprof.csh "tran([john,finds,a,unicorn])" jfauemu $XEMU
csh genprof.csh "tran([john,seeks,a,unicorn])" jsauemu $XEMU
csh genprof.csh "tran([a,man,tries_to,walk])" amtwemu $XEMU
csh genprof.csh "tran([a,woman,tries_to,run,allegedly])" awtraemu $XEMU
csh genprof.csh "tran([every,man,loves,a,woman])" emlawemu $XEMU
csh genprof.csh "tran([a,woman,such_that,she,walks,runs])" awsswremu $XEMU
csh genprof.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
csh genlredprof.csh "tran([john,dates,mary])" jdmlredemu $XEMU
csh genlredprof.csh "tran([a,price,rises])" aprlredemu $XEMU
csh genlredprof.csh "tran([john,dates,him])" jdhlredemu $XEMU
csh genlredprof.csh "tran([john,walks,in,a,park])" jwiaplredemu $XEMU
csh genlredprof.csh "tran([john,finds,a,unicorn])" jfaulredemu $XEMU
csh genlredprof.csh "tran([john,seeks,a,unicorn])" jsaulredemu $XEMU
csh genlredprof.csh "tran([a,man,tries_to,walk])" amtwlredemu $XEMU
csh genlredprof.csh "tran([a,woman,tries_to,run,allegedly])" awtralredemu $XEMU
csh genlredprof.csh "tran([every,man,loves,a,woman])" emlawlredemu $XEMU
csh genlredprof.csh "tran([a,woman,such_that,she,walks,runs])" awsswrlredemu $XEMU
csh genlredprof.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
csh genlredprof.csh "tran([john,believes_that,mary,walks,in,a,park])" jbmwiaplredemu $XEMU
csh genlredprof.csh "tran([john,believes_that,mary,wishes_to,walk,in,a,park])" jbmwwaiplredemu $XEMU
csh genlredprof.csh "tran([john,wishes_to,find,a,unicorn,and,eat,it])" jwfauaeilredemu $XEMU
csh genlredprof.csh "tran([john,tries_to,find,a,unicorn,and,wishes_to,eat,it])" jtfauaweilredemu $XEMU
csh genlredprof.csh "tran([every,man,loves,a,woman,such_that,she,loves,him])" emlawsslhlredemu $XEMU
csh genlredprof.csh "tran([every,man,loves,a,woman,and,she,loves,him])" emlawaslhlredemu $XEMU

