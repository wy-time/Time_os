#include <stdio.h>
#include "bootpack.h" 
extern struct FIFO8 keyfifo;
void enable_mouse(void);
void init_keyboard(void);
void HariMain(void)//主函数
{
	unsigned char *vram;
	int xsize, ysize;
	struct BOOTINFO *bootinfo;
	bootinfo = (struct BOOTINFO *)ADR_BOOTINFO;
	xsize = bootinfo->scrnx;
	ysize = bootinfo->scrny;
	vram = bootinfo->vram;
    unsigned char buf[32];//fifo的缓冲区
    fifo8_init(&keyfifo,32,buf);//初始化缓冲区
    init_gdtidt();//初始化GDT与IDT
    init_pic();//初始化PIC(可编程中断控制器)
    io_sti();//cpu接受外部中断
    init_keyboard();
	init_palette();//初始化调色板
	init_screen(vram, xsize, ysize);//背景显示
	char mouse[16 * 16];
	init_mouse_cursor(mouse, COL8_008484);//初始化鼠标指针
	int mid_x = (xsize - 16) / 2;
	int mid_y = (ysize - 28 - 16) / 2;
	putblock8_8(vram, xsize, 16, 16, mid_x, mid_y, mouse, 16);//绘制指针图像
    io_out8(PIC0_IMR,0xf9);//修改PIC0的中断屏蔽寄存器为11111001 接受IRQ2和1
    io_out8(PIC1_IMR,0xef);//修改PIC1的中断屏蔽寄存器为11101111 接受IRQ12
    enable_mouse();
	while (1)
	{
        io_cli();//屏蔽所有中断
        if(keyfifo.free==keyfifo.size)//缓冲区无内容
            io_stihlt();//开中断并等待
        else
        {
            unsigned char data=fifo8_get(&keyfifo);
            io_sti();//开中断
            char s[40];
            sprintf(s,"%02X",data);
            boxfill8(bootinfo->vram,bootinfo->scrnx,COL8_008484,0,16,15,31);
            putfonts8_asc(bootinfo->vram,bootinfo->scrnx,0,16,COL8_FFFFFF,(unsigned char*)s);
        }
	}
}
#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
void wait_KBC_sendready(void)
{
    //等待键盘控制电路准备完成
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}
void init_keyboard(void)
{
    //初始化键盘控制电路
    wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
void enable_mouse(void)
{
    //激活鼠标 
    wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return;
}
