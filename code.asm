%include 'readPrint.asm'
section .bss
newLine RESB 1
tempIp	RESB	20
stackBase	RESB	28
section .text
global _start
_start:

MOV EBP, stackBase
MOV AL, 0AH
MOV EBX, newLine
MOV [EBX], AL
MOV AX, 3
MOV [EBP + 12], AX

MOV AX, [EBP + 12]
MOV [EBP + 2], AX

MOV EDX, 20
MOV ECX, tempIp
MOV EBX, 0
MOV EAX, 3
INT 80H
MOV EAX, tempIp
CALL atoi
MOV [EBP + 0], AX

XOR EAX, EAX
MOV AX,[EBP + 2]
CALL iprintLF

MOV EBX, 0
MOV EAX, 1
INT 80H
