#include <stdio.h>
#define wfor(i,j,k) for(i=j;i<k;++i)
#define mfor(i,j,k) for(i=j;i>=k;--i)
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15
void io_hlt(void);
void io_in8(int port);
void io_out8(int port, int data);
void io_cli(void);
void io_sti(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void boxfill8(unsigned char*vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void set_palette(int start, int end, unsigned char *rgb);
void init_palette();
void init_screen(unsigned char *vram, int x, int y);
void putfont8(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *font);
void putfonts8_asc(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *str);
void init_mouse_cursor(char *mouse, char bc);
void putblock8_8(unsigned char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char*buf, int bxsize);
struct BOOTINFO//启动区信息
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	unsigned char *vram;
};
struct SEGMENT_DESCRIPTOR//GDT(全局段号记录表)存储的信息，包括段的起始地址，大小，和访问权限--8字节
{
    short limit_low,base_low;
    char base_mid,access_right;
    char limit_high,base_high;
};
struct GATE_DESCRIPTOR//IDT(中断记录表)存储的信息--8字节
{
    short offset_low,selector;
    char dw_count,access_right;
    short offset_high;
};
void init_gdtidt(void);//初始化GDT和IDT
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd,unsigned int limit,int base,int ar);//设置GDT
void set_gatedesc(struct GATE_DESCRIPTOR* gd,int offset, int selector,int ar);//设置IDT
void load_gdtr(int limit,int addr);
void load_idtr(int limit,int addr);
void HariMain(void)//主函数
{
	unsigned char *vram;
	int xsize, ysize;
	struct BOOTINFO *bootinfo;
	bootinfo = (struct BOOTINFO *)0x0ff0;
	xsize = bootinfo->scrnx;
	ysize = bootinfo->scrny;
	vram = bootinfo->vram;
    init_gdtidt();//初始化GDT与IDT
	init_palette();//初始化调色板
	init_screen(vram, xsize, ysize);//背景显示
	char s[100];
	sprintf(s, "vram=%d", (int)vram);
	putfonts8_asc(vram, xsize, 10, 58, COL8_FFFFFF, (unsigned char*)s);
	char mouse[16 * 16];
	init_mouse_cursor(mouse, COL8_008484);//初始化鼠标指针
	int mid_x = (xsize - 16) / 2;
	int mid_y = (ysize - 28 - 16) / 2;
	putblock8_8(vram, xsize, 16, 16, mid_x, mid_y, mouse, 16);//绘制指针图像
	while (1)
		io_hlt();
}
void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR *gdt=(struct SEGMENT_DESCRIPTOR *)0x00270000;//段号记录表的内存位置
    struct GATE_DESCRIPTOR* idt=(struct GATE_DESCRIPTOR*)0x0026f800;//中断记录表的内存地址
    int i;
    //初始化GDT
    wfor(i,0,8192)//一共8192个段
    {
        set_segmdesc(gdt+i,0,0,0);//设置段上限字节数-1,基址,访问权限为0;
    }
    set_segmdesc(gdt+1,0xffffffff,0x00000000,0x4092);
    set_segmdesc(gdt+2,0x0007ffff,0x00280000,0x409a);
    load_gdtr(0xffff,0x00270000);//给全局段表寄存器赋值(汇编)
    //初始化IDT
    wfor(i,0,256)//256个中断记录
    {
        set_gatedesc(idt+i,0,0,0);
    }
    load_idtr(0x7ff,0x0026f800);//给中断记录表寄存器赋值(汇编)
}
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd,unsigned int limit,int base,int ar)
{
    if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
}
void set_gatedesc(struct GATE_DESCRIPTOR *gd,int offset,int selector,int ar)
{
    gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
}
void set_palette(int start, int end, unsigned char *rgb)//设置颜色
{
	int i, eflags;
	eflags = io_load_eflags();
	io_cli();//关中断
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++)
	{
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);//还原中断
}
void init_palette()//初始化调色板
{
	unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,
		0xff, 0x00, 0x00,
		0x00, 0xff, 0x00,
		0xff, 0xff, 0x00,
		0x00, 0x00, 0xff,
		0xff, 0x00, 0xff,
		0x00, 0xff, 0xff,
		0xff, 0xff, 0xff,
		0xc6, 0xc6, 0xc6,
		0x84, 0x00, 0x00,
		0x00, 0x84, 0x00,
		0x84, 0x84, 0x00,
		0x00, 0x00, 0x84,
		0x84, 0x00, 0x84,
		0x00, 0x84, 0x84,
		0x84, 0x84, 0x84//rgb颜色代码
	};
	set_palette(0, 15, table_rgb);
}
void boxfill8(unsigned char*vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)//画矩形
{
	int i, j;
	for (i = x0; i <= x1; i++)
	{
		for (j = y0; j <= y1; j++)
			vram[i + xsize * j] = c;
	}
}
void init_screen(unsigned char *vram, int x, int y)//初始化屏幕背景
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}
void putfont8(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *font)//显示字符(16*8)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		int temp = font[i];
		unsigned char*p = vram + (y + i) * xsize + x;
		if ((temp & 0x80) != 0)p[0] = c;
		if ((temp & 0x40) != 0)p[1] = c;
		if ((temp & 0x20) != 0)p[2] = c;
		if ((temp & 0x10) != 0)p[3] = c;
		if ((temp & 0x08) != 0)p[4] = c;
		if ((temp & 0x04) != 0)p[5] = c;
		if ((temp & 0x02) != 0)p[6] = c;
		if ((temp & 0x01) != 0)p[7] = c;
	}
}
void putfonts8_asc(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *str)//显示字符串
{
	extern char font[4096];
	for (; *str != 0x00; str++)
	{
		putfont8(vram, xsize, x, y, c, (unsigned char*)font + *str * 16);
		x += 8;
	}
}
void init_mouse_cursor(char *mouse, char bc) //初始化鼠标指针
{
	char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int i, j;
	wfor(i, 0, 16)
	{
		wfor(j, 0, 16)
		{
			if (cursor[i][j] == '*')
				mouse[i * 16 + j] = COL8_000000;
			else if (cursor[i][j] == 'O')
				mouse[i * 16 + j] = COL8_FFFFFF;
			else
				mouse[i * 16 + j] = bc;
		}
	}
}
/*绘制图形的函数
 *vxsize是屏幕x轴大小
 *vxsize是屏幕x轴大小
 *vram是显存地址
 *pxsize和pysieze是显示图像的大小
 *px0和py0是图形显示的位置
 *buf是图形的颜色填充的数组
 *bxsize是图形一行的像素数量
*/
void putblock8_8(unsigned char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char*buf, int bxsize)
{
	int i, j;
	wfor(i, 0, pxsize)
	{
		wfor(j, 0, pysize)
		{
			vram[(i + py0)*vxsize + px0 + j] = buf[i * bxsize + j];
		}
	}
}

