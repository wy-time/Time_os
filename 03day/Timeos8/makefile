PATH=../../tolset_h/z_tools/
MAKE= $(PATH)make.exe -r
NASK= $(PATH)nask.exe
EDIMG=$(PATH)edimg.exe

default :
	$(MAKE) img
ipl.bin : ipl.nas makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
Timeos.sys :
	$(NASK) Timeos.nas Timeos.sys Timeos.lst
Timeos.img : ipl.bin Timeos.sys makefile 
	$(EDIMG)   imgin:../../tolset_h/z_tools/fdimg0at.tek\
	 	wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:Timeos.sys to:@: \
	  	imgout:Timeos.img
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
	-del Timeos.sys
	-del Timeos.lst
src_onley:
	$(MAKE) clean
	-del Timeos.img
