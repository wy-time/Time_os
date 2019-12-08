; Time-os
; TAB=4
CYLS    EQU     10              ;常量赋值

		ORG		0x7c00			; 指明程序的加载地址
; 以下这段代码是标准FAT12格式软盘的专用代码
		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; 启动区的名称(8字节)
		DW		512				; 每个扇区的大小(必须是512字节)
		DB		1				; 簇的大小(必须为1个扇区)
		DW		1				; FAT的起始位置(一般从第一个扇区开始)
		DB		2				; FAT的个数 (必须为2)
		DW		224				; 根目录的大小(一般设成224项)
		DW		2880			; 该磁盘的大小(必须是2880扇区)
		DB		0xf0			; 磁盘的种类(必须是0xf0)
		DW		9				; FAT的长度 必须是9扇区
		DW		18				; 1个磁道有几个扇区 必须是18
		DW		2				; 磁头数 必须是2
		DD		0				; 不使用分区，必须是0
		DD		2880			; 重写一次磁盘大小
		DB		0,0,0x29		; 意义不明，固定
		DD		0xffffffff		; (可能是)卷标号码
		DB		"Time--OS   "	; 磁盘的名称 11字节
		DB		"FAT12   "		; 磁盘格式名称 8字节
		RESB	18				; 先空出18字节

; 程序核心
entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; 读磁盘 

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; 柱面0
		MOV		DH,0			; 磁头0
		MOV		CL,2			; 扇区2
readloop:
        MOV     SI,0            ;记录失败次数的寄存器
retry:

		MOV		AH,0x02			; AH=0x02 :读盘 
		MOV		AL,1			; 1个扇区
		MOV		BX,0
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
        JNC     next            ;没出错跳转到next
        ADD     SI,1            ;往SI加1
        CMP     SI,5            ;比较SI与5
        JAE     error           ;SI>=5,跳转到error
        MOV     AH,0X00         
        MOV     DL,0X00         ;A驱动器
        INT     0X13            ;重置驱动器
        JMP     retry
next:
        MOV     AX,ES
        ADD     AX,0x0020       ;把ES加0x00220,即把内存地址后移0x200
        MOV     ES,AX
        ADD     CL,1            ;CL+1
        CMP     CL,18           ;比较CL与18的大小
        JBE     readloop        ;小于等于18则继续读取
        MOV     CL,1
        ADD     DH,1
        CMP     DH,2            ;若DH<2则跳转到readloop
        JB      readloop
        MOV     DH,0
        ADD     CH,1
        CMP     CH,CYLS
        JB      readloop        ;如果CH<CYLS则跳转到readloop
        JMP     0xc200          ;启动区装载完跳到Timeos.sys的地址
fin:
		HLT						; 让cpu停止，等待指令
		JMP		fin				; 无限循环

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 给si加1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,15			; 指定字符颜色
		INT		0x10			; 调用显卡BIOS
		JMP		putloop

msg:
		DB		0x0a, 0x0a		; 换行两次
		DB		"load erroe"
		DB		0x0a			; 换行
		DB		0

		RESB	0x7dfe-$		; 填写0x00直到0x001fe

		DB		0x55, 0xaa
