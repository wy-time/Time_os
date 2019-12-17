TOOLPATH=../../tolset_h/z_tools/
INCPATH=../../tolset_h/z_tools/haribote/
MAKE= $(TOOLPATH)make.exe -r
NASK= $(TOOLPATH)nask.exe
EDIMG=$(TOOLPATH)edimg.exe
CC1=$(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK=$(TOOLPATH)gas2nask.exe -a
OBJ2BIM =$(TOOLPATH)obj2bim.exe
BIM2HRB=$(TOOLPATH)bim2hrb.exe
IMGTOL=$(TOOLPATH)imgtol.com
RULEFILE=$(TOOLPATH)haribote/haribote.rul

default :
	$(MAKE) img
ipl.bin : ipl.nas makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
asmhead.bin : asmhead.nas makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst
bootpack.gas : bootpack.c makefile
	$(CC1) -o bootpack.gas bootpack.c
bootpack.nas : bootpack.gas makefile
	$(GAS2NASK) bootpack.gas bootpack.nas
bootpack.obj : bootpack.nas makefile
	$(NASK) bootpack.nas bootpack.obj bootpack.lst
bootpack.bim : bootpack.obj makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj
bootpack.hrb :bootpack.bim makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0
Timeos.sys : asmhead.bin bootpack.hrb makefile
	copy /B asmhead.bin+bootpack.hrb Timeos.sys
Timeos.img : ipl.bin Timeos.sys makefile 
	$(EDIMG)   imgin:../../tolset_h/z_tools/fdimg0at.tek\
	 	wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:Timeos.sys to:@: \
	  	imgout:Timeos.img
img :
	$(MAKE) Timeos.img
run :
	$(MAKE) img
	copy Timeos.img ..\..\tolset_h\z_tools\qemu\fdimage0.bin
	$(MAKE)	-C ../../tolset_h/z_tools/qemu
clean :
	-del *.bin
	-del *.lst
	-del *.gas
	-del *.obj
	-del bootpack.nas
	-del bootpack.map
	-del bootpack.bim
	-del bootpack.hrb
	-del Timeos.sys
src_onley:
	$(MAKE) clean
	-del Timeos.img
