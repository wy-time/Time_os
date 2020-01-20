OBJS_BOOTPACK = bootpack.obj naskfunc.obj font.obj graphic.obj dsctbl.obj \
				int.obj

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
MAKEFONT=$(TOOLPATH)makefont.exe
BIN2OBJ=$(TOOLPATH)bin2obj.exe

default :
	$(MAKE) img
ipl.bin : ipl.nas makefile
	$(NASK) ipl.nas ipl.bin ipl.lst
asmhead.bin : asmhead.nas makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst
font.bin: font.txt makefile
	$(MAKEFONT) font.txt font.bin
font.obj: font.bin makefile
	$(BIN2OBJ) font.bin font.obj _font
%.gas : %.c makefile
	$(CC1) -o $*.gas $*.c
%.nas: %.gas makefile
	$(GAS2NASK) $*.gas $*.nas
%.obj: %.nas makefile
	$(NASK) $*.nas $*.obj $*.lst

bootpack.bim : $(OBJS_BOOTPACK) makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
bootpack.hrb :bootpack.bim makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0
Timeos.sys : asmhead.bin bootpack.hrb makefile
	cat asmhead.bin bootpack.hrb>>Timeos.sys
Timeos.img : ipl.bin Timeos.sys makefile 
	$(EDIMG)   imgin:../../tolset_h/z_tools/fdimg0at.tek \
	 	wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:Timeos.sys to:@: \
	  	imgout:Timeos.img
img :
	$(MAKE) Timeos.img
run : 
	$(MAKE) Timeos.img
	cp Timeos.img ../../tolset_h/z_tools/qemu/fdimage0.bin
	$(MAKE)	-C ../../tolset_h/z_tools/qemu
clean :
	-rm *.bin
	-rm *.lst
	-rm *.obj
	-rm bootpack.map
	-rm bootpack.bim
	-rm bootpack.hrb
	-rm Timeos.sys
src_onley:
	$(MAKE) clean
	-rm Timeos.img
