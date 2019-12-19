; naskfucn
; TAB=4
[FORMAT "WCOFF"] ;制作目标文件的格式
[INSTRSET "i486p"]  ;使用486
[BITS 32]   ;制作32位的机器语言

;制作目标文件的信息

[FILE "naskfunc.nas"]           ;源文件名信息
    GLOBAL  _io_hlt,    _io_cli,    _io_sti,    _io_stihlt ; 程序中包含的函数名
    GLOBAL  _io_in8,    _io_in16,   _io_in32
    GLOBAL  _io_out8,   _io_out16,  _io_out32
    GLOBAL  _io_load_eflags,    _io_store_eflags

;以下是实际函数
[SECTION .text] ;目标文件中写了这些之后再写程序
_io_hlt:     ;void io_hlt(void);
    HLT
    RET
_io_cli:    ;void io_cli(void);
    CLI
    RET
_io_sti:    ;void io_sti(void);
    STI
    RET
_io_stihlt: ;void io_stihlt (void);
    STI
    HLT
    RET
_io_in8:    ;void io_in8(port);
    MOV EDX,[ESP+4]
    MOV EAX,0
    IN  AL,DX
    RET
_io_in16:   ;void io_in16(port);
    MOV EDX,[ESP+4]
    MOV EAX,0
    IN  AX,DX
    RET
_io_in32:   ;void io_in32(port);
    MOV EDX,[ESP+4]
    IN EAX,DX
    RET
_io_out8:   ;void io_out8(port,data);
    MOV EDX,[ESP+4]
    MOV AL,[ESP+8]
    OUT DX,AL
    RET
_io_out16:  ;void io_out16(port,data);
    MOV EDX,[ESP+4]
    MOV EAX,[ESP+8]
    OUT DX,AX
    RET
_io_out32:  ;void io_out32(port,data);
    MOV EDX,[ESP+4]
    MOV EAX,[ESP+8]
    OUT DX,EAX
    RET
_io_load_eflags:    ;int io_load_eflags(void);
    PUSHFD  ; push eflags
    POP EAX
    RET
_io_store_eflags:   ;void io_store_eflags(data);
    MOV EAX,[ESP+4]
    PUSH EAX
    POPFD   ;pop eflags
    RET

; C语言可以用指针实现，所有注释了
;_write_mem8:    ; void write_mem8(int addr,int data);
;    MOV ECX,[ESP+4]     ;[ESP+4]中存放的是第一份参数也就是地址，将其读入ECX
;    MOV AL,[ESP+8]      ;[ESP+8]中存放的是第二个参数也就是数据，将其读入AL
;    MOV [ECX],AL        ;将数据写入地址，即将data写入addr的位置
;    RET

