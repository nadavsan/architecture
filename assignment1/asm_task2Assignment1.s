section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			

	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
	mov ebx, dd 0
	cmp ecx, byte 'q'
	je end
	
	mov edx, 0		;counter
	mov eax, ecx	;pointer
strint_size:
	cmp eax, byte '\0' ;end of the String
	je print
	inc edx ;maybe? dd [edx]
	add eax, -1
	jmp string_size

print:
	add eax, -1		;we don't want to overwrinte '\0'
	mov an, eax 	;saving ptr adress of the new string
	mov ax, an		;pointer to output string
	mov eax, ecx	;pointer to input string
	switch:
	if0:
	cmp dd [eax], 0
	jne if1
	mov byte [ax],0
	mov byte [ax-1],0
	mov byte [ax-2],0
	sub ax, 3
	sub eax,1
	jmp switch
	if1:
	cmp dd [eax], 1
	jne if2
	mov byte [ax],0
	mov byte [ax-1],0
	mov byte [ax-2],1
	sub ax, 3
	sub eax,1
	jmp switch
	if2:
	cmp dd [eax], 2
	jne if3
	mov byte [ax],0
	mov byte [ax-1],1
	mov byte [ax-2],0
	sub ax, 3
	sub eax,1
	jmp switch
	if3:
	cmp dd [eax], 3
	jne if4
	mov byte [ax],0
	mov byte [ax-1],1
	mov byte [ax-2],1
	sub ax, 3
	sub eax,1
	jmp switch
	if4:
	cmp dd [eax], 4
	jne if5
	mov byte [ax],1
	mov byte [ax-1],0
	mov byte [ax-2],0
	sub ax, 3
	sub eax,1
	jmp switch
	if5:
	cmp dd [eax], 5
	jne if6
	mov byte [ax],1
	mov byte [ax-1],0
	mov byte [ax-2],1
	sub ax, 3
	sub eax,1
	jmp switch
	if6:
	cmp dd [eax], 6
	jne if7
	mov byte [ax],1
	mov byte [ax-1],1
	mov byte [ax-2],0
	sub ax, 3
	sub eax,1
	jmp switch
	if7:
	cmp dd [eax],7
	jne default
	mov byte [ax],1
	mov byte [ax-1],1
	mov byte [ax-2],1
	sub ax, 3
	sub eax,1
	jmp switch
	default:			;must be '\0'
	cmp dd [eax], '\0'	
	jne end
	mov byte [ax],'\0'



	push an			; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 8		; clean up stack after call
	end:
	popad			
	mov esp, ebp	
	pop ebp
	ret
