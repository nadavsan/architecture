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
	
	global _start

	section .text
_start:	
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage
	;CODE START
	mov edx, FileName
	cmp byte[edx], 'E'
	jne notELF
	cmp byte[edx + 1], 'L'
	jne notELF
	cmp byte[edx + 2], 'F'
	jne notELF
	elfFile:
		;open
		mov ebx, FileName        ; file name
		mov ecx, 1024 | 1           ; O_APPEND
		mov edx, 0777
		open ebx,ecx,edx
		mov esi, eax                   ; store return value in esi
		;write
		mov ecx, OutStr
		mov edx, OutStrLen
		write esi, ecx, edx
		;close
		close esi
		jmp printed
	notELF:
	;mov ecx, Failstr
	;mov edx, FailstrLen
	;write 1, ecx, edx
	printed:


	
	

VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)
	
FileName:	db "ELFexec2short", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
OutStrLen: equ $ - OutStr
Failstr:        db "perhaps not", 10 , 0
FailstrLen: equ $ - Failstr
	

get_my_loc:
	call next_i
next_i:
	pop ecx
	ret	
PreviousEntryPoint: dd VirusExit
virus_end:


