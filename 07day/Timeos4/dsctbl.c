#include "bootpack.h" 
/*关于GDT与IDT处理的函数*/
void init_gdtidt(void)//初始化GDT，IDT
{
    struct SEGMENT_DESCRIPTOR *gdt=(struct SEGMENT_DESCRIPTOR *)ADR_GDT;//段号记录表的内存位置
    struct GATE_DESCRIPTOR* idt=(struct GATE_DESCRIPTOR*)ADR_IDT;//中断记录表的内存地址
    int i;
    //初始化GDT
    wfor(i,0,LIMIT_GDT/8)//一共8192个段
    {
        set_segmdesc(gdt+i,0,0,0);//设置段上限字节数-1,基址,访问权限为0;
    }
    set_segmdesc(gdt+1,0xffffffff,0x00000000,AR_DATA32_RW);
    set_segmdesc(gdt+2,LIMIT_BOTPAK,ADR_BOTPAK,AR_CODE32_ER);
    load_gdtr(LIMIT_GDT,ADR_GDT);//给全局段表寄存器赋值(汇编)
    //初始化IDT
    wfor(i,0,LIMIT_IDT/8)//256个中断记录
    {
        set_gatedesc(idt+i,0,0,0);
    }
    load_idtr(LIMIT_IDT,ADR_IDT);//给中断记录表寄存器赋值(汇编)
    set_gatedesc(idt+0x21,(int)asm_inthandler21,2<<3,AR_INTGATE32);//注册中断函数
    set_gatedesc(idt+0x27,(int)asm_inthandler27,2<<3,AR_INTGATE32);
    set_gatedesc(idt+0x2c,(int)asm_inthandler2c,2<<3,AR_INTGATE32);
}
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd,unsigned int limit,int base,int ar)//设置GDT
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
void set_gatedesc(struct GATE_DESCRIPTOR *gd,int offset,int selector,int ar)//设置IDT
{
    gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
}

