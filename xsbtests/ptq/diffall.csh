echo "--------------------------------------------------------"
if (-e $argv[2]_s1) then 
else
	touch $argv[2]_s1
endif
diff ${argv[1]}_s1 ${argv[2]}_s1
if ( $status == 0 ) then 
	echo "$argv[1]_s1 tested"
else
	echo "$argv[1]_s1 different \!\!\!"
endif
echo "--------------------------------------------------------"
if (-e $argv[2]_iv1) then 
else
	touch $argv[2]_iv1
endif
diff ${argv[1]}_iv1 ${argv[2]}_iv1
if ( $status == 0 ) then 
	echo "$argv[1]_iv1 tested"
else
	echo "$argv[1]_iv1 different\!\!\!"
endif
echo "--------------------------------------------------------"
if (-e $argv[2]_cn1) then 
else
	touch $argv[2]_cn1
endif
diff ${argv[1]}_cn1 ${argv[2]}_cn1
if ( $status == 0 ) then 
	echo "$argv[1]_cn1 tested"
else
	echo "$argv[1]_cn1 different\!\!\!"
endif
echo "--------------------------------------------------------"
if (-e $argv[2]_te11) then 
else
	touch $argv[2]_te11
endif
diff ${argv[1]}_te11 ${argv[2]}_te11
if ( $status == 0 ) then 
	echo "$argv[1]_te11 tested"
else
	echo "$argv[1]_te11 different\!\!\!"
endif
