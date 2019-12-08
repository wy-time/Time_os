; Time-os
; TAB=4
CYLS    EQU     10              ;������ֵ

		ORG		0x7c00			; ָ������ļ��ص�ַ
; ������δ����Ǳ�׼FAT12��ʽ���̵�ר�ô���
		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; ������������(8�ֽ�)
		DW		512				; ÿ�������Ĵ�С(������512�ֽ�)
		DB		1				; �صĴ�С(����Ϊ1������)
		DW		1				; FAT����ʼλ��(һ��ӵ�һ��������ʼ)
		DB		2				; FAT�ĸ��� (����Ϊ2)
		DW		224				; ��Ŀ¼�Ĵ�С(һ�����224��)
		DW		2880			; �ô��̵Ĵ�С(������2880����)
		DB		0xf0			; ���̵�����(������0xf0)
		DW		9				; FAT�ĳ��� ������9����
		DW		18				; 1���ŵ��м������� ������18
		DW		2				; ��ͷ�� ������2
		DD		0				; ��ʹ�÷�����������0
		DD		2880			; ��дһ�δ��̴�С
		DB		0,0,0x29		; ���岻�����̶�
		DD		0xffffffff		; (������)������
		DB		"Time--OS   "	; ���̵����� 11�ֽ�
		DB		"FAT12   "		; ���̸�ʽ���� 8�ֽ�
		RESB	18				; �ȿճ�18�ֽ�

; �������
entry:
		MOV		AX,0			; ��ʼ���Ĵ���
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ������ 

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; ����0
		MOV		DH,0			; ��ͷ0
		MOV		CL,2			; ����2
readloop:
        MOV     SI,0            ;��¼ʧ�ܴ����ļĴ���
retry:

		MOV		AH,0x02			; AH=0x02 :���� 
		MOV		AL,1			; 1������
		MOV		BX,0
		MOV		DL,0x00			; A������
		INT		0x13			; ���ô���BIOS
        JNC     next            ;û������ת��next
        ADD     SI,1            ;��SI��1
        CMP     SI,5            ;�Ƚ�SI��5
        JAE     error           ;SI>=5,��ת��error
        MOV     AH,0X00         
        MOV     DL,0X00         ;A������
        INT     0X13            ;����������
        JMP     retry
next:
        MOV     AX,ES
        ADD     AX,0x0020       ;��ES��0x00220,�����ڴ��ַ����0x200
        MOV     ES,AX
        ADD     CL,1            ;CL+1
        CMP     CL,18           ;�Ƚ�CL��18�Ĵ�С
        JBE     readloop        ;С�ڵ���18�������ȡ
        MOV     CL,1
        ADD     DH,1
        CMP     DH,2            ;��DH<2����ת��readloop
        JB      readloop
        MOV     DH,0
        ADD     CH,1
        CMP     CH,CYLS
        JB      readloop        ;���CH<CYLS����ת��readloop
        JMP     0xc200          ;������װ��������Timeos.sys�ĵ�ַ
fin:
		HLT						; ��cpuֹͣ���ȴ�ָ��
		JMP		fin				; ����ѭ��

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; ��si��1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; ��ʾһ������
		MOV		BX,15			; ָ���ַ���ɫ
		INT		0x10			; �����Կ�BIOS
		JMP		putloop

msg:
		DB		0x0a, 0x0a		; ��������
		DB		"load erroe"
		DB		0x0a			; ����
		DB		0

		RESB	0x7dfe-$		; ��д0x00ֱ��0x001fe

		DB		0x55, 0xaa
