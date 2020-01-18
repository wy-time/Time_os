#include "bootpack.h" 
/*关于图形处理的函数*/
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

