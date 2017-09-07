
REM Arg 1 is the path where InstallXSB.jar is located
cd %1%
@ shift

@ set setvcvar=%1%
@ set vsdisk=%2%
@ set xsbdisk=%3%
@ set currpath=%4%
@ set makexsb=%5%
@ %vsdisk%
@ call %setvcvar%
@ %xsbdisk%
cd %currpath%
@ cd build
@ call %makexsb% clean
@ call %makexsb%
cd ..
echo "======= done ======="
