#define wfor(i,j,k) for(i=j;i<k;++i)
#define mfor(i,j,k) for(i=j;i>=k;--i)
/*asmhead.nas*/
struct BOOTINFO//启动区信息 0x0ff0-0x0fff
{
	char cyls; //启动区读硬盘的柱面数
    char leds;//启动时键盘的led状态
    char vmode;//显卡模式为多少位色
    char reserve;
	short scrnx, scrny;//分辨率
	unsigned char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

/*naskfunc.nas*/
void io_hlt(void);
int io_in8(int port);
void io_out8(int port, int data);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit,int addr);
void load_idtr(int limit,int addr);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
/*graphic.c*/
void boxfill8(unsigned char*vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void set_palette(int start, int end, unsigned char *rgb);
void init_palette();
void init_screen(unsigned char *vram, int x, int y);
void putfont8(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *font);
void putfonts8_asc(unsigned char*vram, int xsize, int x, int y, char c, unsigned char *str);
void init_mouse_cursor(char *mouse, char bc);
void putblock8_8(unsigned char* vram, int vxsize, int pxsize, int pysize, int px0, int py0, char*buf, int bxsize);
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

/*dsctbl.c*/
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
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e
/* int.c */
void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
//端口号
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
/*fifo.c*/
struct FIFO8 
{
    unsigned char *buf;//缓冲区首地址
    int next_w;//下一个写入地址
    int next_r;//下一个读出地址
    int size;//缓冲区大小
    int free;//缓冲区剩余空闲大小
    int flag;//是否溢出标志
};
void fifo8_init(struct FIFO8* fifo,int size,unsigned char*buf);
int fifo8_put(struct FIFO8*fifo,unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo_status(struct FIFO8 *fifo);
