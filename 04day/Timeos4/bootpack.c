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
void set_palette(int start, int end, unsigned char *rgb)
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
void init_palette()
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
void box(unsigned char*vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int i, j;
	for (i = x0; i <= x1; i++)
	{
		for (j = y0; j <= y1; j++)
			vram[i + xsize * j] = c;
	}
}
void HariMain(void)
{
	unsigned char *p = (unsigned char*)0xa0000;
	init_palette();//初始化调色板
	box(p, 320, COL8_FFFFFF, 20, 20, 120, 120);
	box(p, 320, COL8_00FFFF, 50, 50, 150, 150);
	box(p, 320, COL8_008400, 100, 100, 220, 220);
	while (1)
		io_hlt();
}
