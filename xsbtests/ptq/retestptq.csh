set XEMU = /home/u21/sbprolog/PSB-terry/PSB-terry3/emu/xemu
echo "-------------------------------------------------------"
echo " running retestptq.csh "
echo "-------------------------------------------------------"int

csh remove.csh amtwretemu 
csh remove.csh aprretemu 
csh remove.csh awsswrretemu 
csh remove.csh awtraretemu 
csh remove.csh emlawaslhretemu 
csh remove.csh emlawsslhretemu 
csh remove.csh jbmwiapretemu 
csh remove.csh jbmwwaipretemu 
csh remove.csh jdhretemu 
csh remove.csh jsauamsiemu 
csh remove.csh jsauretemu 
csh remove.csh jtauretemu 
csh remove.csh jtfauaweiretemu 
csh remove.csh jwiapretemu 
csh remove.csh jwfauaeiretemu 
csh remove.csh mbjfauaheiretemu  

$XEMU -x 1024 -i << EOF
[ptqlred,tables].
 tran([a,man,tries_to,walk]). 
output_table(s(_,_,_,_,_),amtwretemu_s).
output_table(te1(_,_,_,_,_,_,_),amtwretemu_te1).
output_table(cn(_,_,_,_,_,_),amtwretemu_cn).
output_table(iv(_,_,_,_,_,_),amtwretemu_iv).
 tran([a,price,rises]). 
output_table(s(_,_,_,_,_),aprretemu_s).
output_table(te1(_,_,_,_,_,_,_),aprretemu_te1).
output_table(cn(_,_,_,_,_,_),aprretemu_cn).
output_table(iv(_,_,_,_,_,_),aprretemu_iv).
 tran([a,woman,such_that,she,walks,runs]). 
output_table(s(_,_,_,_,_),awsswrretemu_s).
output_table(te1(_,_,_,_,_,_,_),awsswrretemu_te1).
output_table(cn(_,_,_,_,_,_),awsswrretemu_cn).
output_table(iv(_,_,_,_,_,_),awsswrretemu_iv).
 tran([a,woman,tries_to,run,allegedly]).
output_table(s(_,_,_,_,_),awtraretemu_s).
output_table(te1(_,_,_,_,_,_,_),awtraretemu_te1).
output_table(cn(_,_,_,_,_,_),awtraretemu_cn).
output_table(iv(_,_,_,_,_,_),awtraretemu_iv).
 tran([every,man,loves,a,woman,and,she,loves,him]). 
output_table(s(_,_,_,_,_),emlawaslhretemu_s).
output_table(te1(_,_,_,_,_,_,_),emlawaslhretemu_te1).
output_table(cn(_,_,_,_,_,_),emlawaslhretemu_cn).
output_table(iv(_,_,_,_,_,_),emlawaslhretemu_iv).
tran([every,man,loves,a,woman,such_that,she,loves,him]).
output_table(s(_,_,_,_,_),emlawsslhretemu_s).
output_table(te1(_,_,_,_,_,_,_),emlawsslhretemu_te1).
output_table(cn(_,_,_,_,_,_),emlawsslhretemu_cn).
output_table(iv(_,_,_,_,_,_),emlawsslhretemu_iv).
tran([john,believes_that,mary,walks,in,a,park]).
output_table(s(_,_,_,_,_),jbmwiapretemu_s).
output_table(te1(_,_,_,_,_,_,_),jbmwiapretemu_te1).
output_table(cn(_,_,_,_,_,_),jbmwiapretemu_cn).
output_table(iv(_,_,_,_,_,_),jbmwiapretemu_iv).
tran([john,believes_that,mary,wishes_to,walk,in,a,park]).
output_table(s(_,_,_,_,_),jbmwwiapretemu_s).
output_table(te1(_,_,_,_,_,_,_),jbmwwiapretemu_te1).
output_table(cn(_,_,_,_,_,_),jbmwwiapretemu_cn).
output_table(iv(_,_,_,_,_,_),jbmwwiapretemu_iv).
tran([john,dates,him]).
output_table(s(_,_,_,_,_),jdhretemu_s).
output_table(te1(_,_,_,_,_,_,_),jdhretemu_te1).
output_table(cn(_,_,_,_,_,_),jdhretemu_cn).
output_table(iv(_,_,_,_,_,_),jdhretemu_iv).
tran([john,seeks,a,unicorn,and,mary,seeks,it]).
output_table(s(_,_,_,_,_),jsauamsiretemu_s).
output_table(te1(_,_,_,_,_,_,_),jsauamsiretemu_te1).
output_table(cn(_,_,_,_,_,_),jsauamsiretemu_cn).
output_table(iv(_,_,_,_,_,_),jsauamsiretemu_iv).
tran([john,seeks,a,unicorn]).
output_table(s(_,_,_,_,_),jsauretemu_s).
output_table(te1(_,_,_,_,_,_,_),jsauretemu_te1).
output_table(cn(_,_,_,_,_,_),jsauretemu_cn).
output_table(iv(_,_,_,_,_,_),retemu_iv).
tran([john,talks,about,a,unicorn]).
output_table(s(_,_,_,_,_),retemu_s).
output_table(te1(_,_,_,_,_,_,_),jtauretemu_te1).
output_table(cn(_,_,_,_,_,_),jtauretemu_cn).
output_table(iv(_,_,_,_,_,_),jtauretemu_iv).
tran([john,tries_to,find,a,unicorn,and,wishes_to,eat,it]).
output_table(s(_,_,_,_,_),retemu_s).
output_table(te1(_,_,_,_,_,_,_),retemu_te1).
output_table(cn(_,_,_,_,_,_),retemu_cn).
output_table(iv(_,_,_,_,_,_),retemu_iv).
tran([john,walks,in,a,park]).
output_table(s(_,_,_,_,_),retemu_s).
output_table(te1(_,_,_,_,_,_,_),retemu_te1).
output_table(cn(_,_,_,_,_,_),retemu_cn).
output_table(iv(_,_,_,_,_,_),retemu_iv).
tran([john,wishes_to,find,a,unicorn,and,eat,it]).
output_table(s(_,_,_,_,_),retemu_s).
output_table(te1(_,_,_,_,_,_,_),retemu_te1).
output_table(cn(_,_,_,_,_,_),retemu_cn).
output_table(iv(_,_,_,_,_,_),retemu_iv).
tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it]).
output_table(s(_,_,_,_,_),retemu_s).
output_table(te1(_,_,_,_,_,_,_),retemu_te1).
output_table(cn(_,_,_,_,_,_),retemu_cn).
output_table(iv(_,_,_,_,_,_),retemu_iv).
halt.
EOF

csh genretscaff.csh "tran([a,man,tries_to,walk])" amtwretemu $XEMU
csh genretscaff.csh "tran([a,price,rises])" aprretemu $XEMU
csh genretscaff.csh "tran([a,woman,such_that,she,walks,runs])" awsswrretemu $XEMU
csh genretscaff.csh "tran([a,woman,tries_to,run,allegedly])" awtraretemu $XEMU
csh genretscaff.csh "tran([every,man,loves,a,woman,and,she,loves,him])" emlawaslhretemu $XEMU
csh genretscaff.csh "tran([every,man,loves,a,woman,such_that,she,loves,him])" emlawsslhretemu $XEMU
csh genretscaff.csh "tran([every,man,loves,a,woman])" emlawretemu $XEMU
csh genretscaff.csh "tran([john,believes_that,mary,walks,in,a,park])" jbmwiapretemu $XEMU
csh genretscaff.csh "tran([john,believes_that,mary,wishes_to,walk,in,a,park])" jbmwwaipretemu $XEMU
csh genretscaff.csh "tran([john,dates,him])" jdhretemu $XEMU
csh genretscaff.csh "tran([john,dates,mary])" jdmretemu $XEMU
csh genretscaff.csh "tran([john,finds,a,unicorn])" jfauretemu $XEMU
csh genretscaff.csh "tran([john,seeks,a,unicorn,and,mary,seeks,it])" jsauamsiemu $XEMU
csh genretscaff.csh "tran([john,seeks,a,unicorn])" jsauretemu $XEMU
csh genretscaff.csh "tran([john,talks,about,a,unicorn])" jtauretemu $XEMU
csh genretscaff.csh "tran([john,tries_to,find,a,unicorn,and,wishes_to,eat,it])" jtfauaweiretemu $XEMU
csh genretscaff.csh "tran([john,walks,in,a,park])" jwiapretemu $XEMU
csh genretscaff.csh "tran([john,wishes_to,find,a,unicorn,and,eat,it])" jwfauaeiretemu $XEMU
csh genretscaff.csh "tran([mary,believes_that,john,finds,a,unicorn,and,he,eats,it])" mbjfauaheiretemu  $XEMU

csh diffall.csh amtwretemu amtwretint
csh diffall.csh aprretemu aprretint
csh diffall.csh awsswrretemu awsswrretint
csh diffall.csh awtraretemu awtraretint
csh diffall.csh emlawaslhretemu emlawaslhretint
csh diffall.csh emlawretemu emlawretint
csh diffall.csh emlawsslhretemu emlawsslhretint
csh diffall.csh jbmwiapretemu jbmwiapretint
csh diffall.csh jbmwwaipretemu jbmwwaipretint
csh diffall.csh jdhretemu jdhretint
csh diffall.csh jdmretemu jdmretint
csh diffall.csh jfauretemu jfauretint
csh diffall.csh jsauamsiemu jsauamsiretint
csh diffall.csh jsauretemu jsauretint
csh diffall.csh jtauretemu jtauretint
csh diffall.csh jtfauaweiretemu jtfauaweiretint
csh diffall.csh jwfauaeiretemu jwfauaeiretint
csh diffall.csh jwiapretemu jwiapretint
csh diffall.csh mbjfauaheiretemu mbjfauaheiretint




