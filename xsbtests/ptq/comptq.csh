#! /bin/csh

csh genscaff.csh "$argv[1]" ${argv[2]}emu
csh genxscaff.csh "$argv[1]" ${argv[2]}int
csh diffall.csh ${argv[2]}emu ${argv[2]}int
