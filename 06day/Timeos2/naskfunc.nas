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
    GLOBAL  _load_gdtr, _load_idtr
    GLOBAL  _asm_inthandler21,_asm_inthandler27,_asm_inthandler2c     ;中断处理保护现场与返回
    EXTERN  _inthandler21,_inthandler27,_inthandler2c
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
_load_gdtr:     ;void load_gdtr(int limit,int addr);
    MOV     AX,[ESP+4]      ;limit
    MOV     [ESP+6],AX
    LGDT    [ESP+6]
    RET
_load_idtr:     ;void load_idtr(int limit,int addr);
    MOV     AX,[ESP+4]      ;limit
    MOV     [ESP+6],AX
    LIDT    [ESP+6]
    RET
_asm_inthandler21: 
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    _inthandler21
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD

_asm_inthandler27:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    _inthandler27
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD
_asm_inthandler2c:
    PUSH    ES
    PUSH    DS
    PUSHAD
    MOV     EAX,ESP
    PUSH    EAX
    MOV     AX,SS
    MOV     DS,AX
    MOV     ES,AX
    CALL    _inthandler2c
    POP     EAX
    POPAD
    POP     DS
    POP     ES
    IRETD
; C语言可以用指针实现，所以注释了
;_write_mem8:    ; void write_mem8(int addr,int data);
;    MOV ECX,[ESP+4]     ;[ESP+4]中存放的是第一份参数也就是地址，将其读入ECX
;    MOV AL,[ESP+8]      ;[ESP+8]中存放的是第二个参数也就是数据，将其读入AL
;    MOV [ECX],AL        ;将数据写入地址，即将data写入addr的位置
;    RET

