; naskfucn
; TAB=4
[FORMAT "WCOFF"] ;制作目标文件的格式
[INSTRSET "i486p"]  ;使用486
[BITS 32]   ;制作32位的机器语言

;制作目标文件的信息

[FILE "naskfunc.nas"]           ;源文件名信息
    GLOBAL  _io_hlt,_write_mem8         ; 程序中包含的函数名

;以下是实际函数
[SECTION .text] ;目标文件中写了这些之后再写程序
_io_hlt:     ;void io_hlt(void);
    HLT
    RET

;_write_mem8:    ; void write_mem8(int addr,int data);
;    MOV ECX,[ESP+4]     ;[ESP+4]中存放的是第一份参数也就是地址，将其读入ECX
;    MOV AL,[ESP+8]      ;[ESP+8]中存放的是第二个参数也就是数据，将其读入AL
;    MOV [ECX],AL        ;将数据写入地址，即将data写入addr的位置
;    RET
