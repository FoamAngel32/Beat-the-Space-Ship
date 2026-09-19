@echo off

set name="BeatTheSpaceShip"

set path=%path%;..\bin\

set CC65_HOME=..\

cc65 -Oirs src/main.c --add-source
ca65 src/crt0.s
ca65 src/main.s -g

ld65 -C src/nrom_32k_vert.cfg -o %name%.nes src/crt0.o src/main.o nes.lib -Ln labels.txt
move /Y main.s ..\build
move /Y *.o ..\build
move /Y labels.txt build\ 
move /Y %name%.nes build\ 

cd build
%name%.nes
