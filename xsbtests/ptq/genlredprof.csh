#! /bin/csh

set XEMU = $argv[3]
$XEMU -x 1024 -i << EOF
[ptq,tables].
set_prof_on(2).
zero_out_profile_table,$argv[1].
write_out_profile_table.
halt.
EOF
