#include <stdio.h>
#include "bootpack.h" 
void HariMain(void)//主函数
{
	unsigned char *vram;
	int xsize, ysize;
	struct BOOTINFO *bootinfo;
	bootinfo = (struct BOOTINFO *)ADR_BOOTINFO;
	xsize = bootinfo->scrnx;
	ysize = bootinfo->scrny;
	vram = bootinfo->vram;
    init_gdtidt();//初始化GDT与IDT
    init_pic();//初始化PIC(可编程中断控制器)
    io_sti();//cpu接受外部中断
	init_palette();//初始化调色板
	init_screen(vram, xsize, ysize);//背景显示
	char mouse[16 * 16];
	init_mouse_cursor(mouse, COL8_008484);//初始化鼠标指针
	int mid_x = (xsize - 16) / 2;
	int mid_y = (ysize - 28 - 16) / 2;
	putblock8_8(vram, xsize, 16, 16, mid_x, mid_y, mouse, 16);//绘制指针图像
    io_out8(PIC0_IMR,0xf9);//修改PIC0的中断屏蔽寄存器为11111001 接受IRQ2和1
    io_out8(PIC1_IMR,0xef);//修改PIC1的中断屏蔽寄存器为11101111 接受IRQ12
	while (1)
		io_hlt();
}

