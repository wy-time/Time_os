/* int.c*/

#include "bootpack.h"

void init_pic(void)
/* PIC初始化*/
{
	io_out8(PIC0_IMR,  0xff  ); /* 屏蔽所有中断*/
	io_out8(PIC1_IMR,  0xff  ); /* 屏蔽所有中断*/

	io_out8(PIC0_ICW1, 0x11  ); /* 边沿触发模式(edge trigger mode)*/
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT20-27接收*/
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接*/
	io_out8(PIC0_ICW4, 0x01  ); /* 无缓冲区模式*/

	io_out8(PIC1_ICW1, 0x11  ); /* 边沿触发模式(edge trigger mode)*/
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15由INT28-2f接收*/
	io_out8(PIC1_ICW3, 2     ); /* PIC1由IRQ2连接*/
	io_out8(PIC1_ICW4, 0x01  ); /* 无缓冲区模式*/

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC0屏蔽除了PIC1之外的所有中断请求*/
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 PIC1屏蔽所有中断请求*/

	return;
}
void inthandler21 (int *emp) //0x21号中断函数，键盘的中断 IRQ1
{
    struct BOOTINFO *bootinfo;
	bootinfo = (struct BOOTINFO *)ADR_BOOTINFO;
    boxfill8(bootinfo->vram,bootinfo->scrnx,COL8_000000,0,0,32*8-1,15);
    putfonts8_asc(bootinfo->vram,bootinfo->scrnx,0,0,COL8_FFFFFF,(unsigned char*)"*INT 21 keyboard");
    for (;;)
        io_hlt();
}
void inthandler2c (int *emp) //0x2c号中断函数，鼠标的中断 IRQ12
{
    struct BOOTINFO *bootinfo;
	bootinfo = (struct BOOTINFO *)ADR_BOOTINFO;
    boxfill8(bootinfo->vram,bootinfo->scrnx,COL8_000000,0,0,32*8-1,15);
    putfonts8_asc(bootinfo->vram,bootinfo->scrnx,0,0,COL8_FFFFFF,(unsigned char*)"*INT 2c mouse");
    for (;;)
        io_hlt();
}
void inthandler27 (int *emp) //0x27号中断函数
/*操作系统启动会产生异常IRQ7的中断，如果不处理会启动失败*/
{
    io_out8(PIC0_OCW2,0x67);
}
