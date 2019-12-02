PATH=../../tolset_h/z_tools/
MAKE= $(PATH)make.exe -r
NASK= $(PATH)nask.exe
EDIMG=$(PATH)edimg.exe

default :
	$(MAKE) img
ipl.bin : ipl.nas makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
Timeos.img : ipl.bin makefile
	$(EDIMG)   imgin:../../tolset_h/z_tools/fdimg0at.tek\
	 	wbinimg src:ipl.bin len:512 from:0 to:0   imgout:Timeos.img
img :
	$(MAKE) Timeos.img
asm :
	$(MAKE) -r ipl.bin
run :
	$(MAKE) img
	copy Timeos.img ..\..\tolset_h\z_tools\qemu\fdimage0.bin
	$(MAKE)	-C ../../tolset_h/z_tools/qemu
clean :
	-del ipl.bin
	-del ipl.lst
src_onley:
	$(MAKE) clean
	-del Timeos.img
