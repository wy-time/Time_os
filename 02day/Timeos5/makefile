default :
	../../tolset_h/z_tools/make.exe img
ipl.bin : ipl.nas makefile
	../../tolset_h/z_tools/nask.exe ipl.nas ipl.bin ipl.lst
Timeos.img : ipl.bin makefile
	../../tolset_h/z_tools/edimg.exe   imgin:../../tolset_h/z_tools/fdimg0at.tek\
	 	wbinimg src:ipl.bin len:512 from:0 to:0   imgout:Timeos.img
img :
	../../tolset_h/z_tools/make.exe -r Timeos.img
asm :
	../../tolset_h/z_tools/make.exe -r ipl.bin
run :
	../../tolset_h/z_tools/make.exe img
	copy Timeos.img ..\..\tolset_h\z_tools\qemu\fdimage0.bin
	..\..\tolset_h\z_tools\make.exe	-C ../../tolset_h/z_tools/qemu
clean :
	-del ipl.bin
	-del ipl.lst
src_onley:
	../../tolset_h/z_tools/make.exe clean
	-del Timeos.img
