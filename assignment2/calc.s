section .bss
    user_input: resb 1024
    stack_base: resd 1
    stack_top: resd 1
    new_node: resd 1
    node_data: resb 1
    carry: resd 1
    node_pointer: resd 1
    hex_value: resb 1
section .data                   ; initialized data section
    count:   dd 0             ; user prompt
    first_number_counter:   dd 0             
    second_number_counter:   dd 0             
    calc:   db "calc: ", 0             ; user prompt
    format_string: db "Input = %s", 0
    hex_format: db "%02X", 0
    hex_format_no_two: db "%X", 0
    dec_format: db "%x", 10, 0
    format_pointer: db "%p", 10, 0
    new_line: db 10, 0
    just_test: db "test!", 10, 0

section .text
  align 16
  global main
  extern printf
  extern fprintf 
  extern fflush
  extern malloc 
  extern calloc 
  extern free 
  extern getchar 
  extern fgets 
  extern stdout
  extern stdin
  extern stderr

;------------------Help functions------------------
%macro count_chars 1
    pushad
    xor edx, edx
    mov eax, %1
    %%count:
        cmp byte[eax], 0
        je %%done
        cmp byte[eax], 10
        je %%done
        inc eax
        inc edx
        jmp %%count
    %%done:
        mov dword[count], edx
    popad
%endmacro
;------------------stack implementation------------------
;%1 is stack size
%macro create_stack 1
    pushad
    push 4
    push %1
    call calloc
    add esp, 8
    mov dword[stack_base], eax
    mov dword[stack_top], eax
    popad
%endmacro

;%1 pointer to a linked list
%macro stack_push 1
    pushad
    mov eax, dword[stack_top]
    mov dword[eax], %1
    add dword[stack_top], 4
    popad
%endmacro

;%1 is now pointing at popped value
%macro stack_pop 1
    sub dword[stack_top], 4
    mov eax, dword[stack_top]
    mov %1, dword[eax]
%endmacro

;------------------list implementation------------------
%macro create_node 0
    pushad
    push 1
    push 5
    call calloc
    add esp, 8
    mov dword[new_node], eax
    popad
%endmacro

%macro print_new_line 0
    pushad
    push new_line
    call printf
    add esp, 4
    popad
%endmacro

;%1 is the node
;%2 is the data (size is byte)
%macro get_data 2
    pushad
    mov eax, %1
    mov bl, byte[eax]
    mov byte[node_data], bl
    popad
    mov %2, byte[node_data]
%endmacro

;%1 is the node
;%2 is the data (size is byte)
%macro set_data 2
    pushad
    mov eax, %1
    mov byte[eax], %2
    popad
%endmacro

;%1 is the node
;%2 is the pointer]
%macro get_next 2
    pushad
    mov eax, %1
    mov ebx, dword[eax+1]
    mov dword[node_pointer], ebx
    popad
    mov %2, dword[node_pointer]
%endmacro

;%1 is the node
;%2 is the pointer
%macro set_next 2
    pushad
    mov ebx, %1
    mov eax, %2
    mov dword[ebx+1], %2
    popad
%endmacro

;%1 is the head
;%2 is the index
%macro get_node 2
    pushad
    mov eax, %1
    mov edx, %2
    dec edx
    %%loop_get:
    cmp edx, 0
    je %%done
    cmp eax, 0
    je %%error
    get_next eax, eax
    dec edx
    jmp %%loop_get
    %%error:
        mov eax, 0
    %%done:
        mov dword[node_pointer], eax
    popad
    mov %1, dword[node_pointer]
%endmacro

;%1 is the head
%macro count_nodes 1
    pushad
    mov eax, %1
    xor edx, edx
    %%loop_get:
    cmp eax, 0
    je %%done
    get_next eax, eax
    inc edx
    jmp %%loop_get
    %%done:
        mov dword[count], edx
    popad
%endmacro

;%1 is the old node
;%2 is the new node
%macro duplicate_node 2
    pushad
    xor ebx, ebx
    mov eax, %1
    create_node
    get_data eax, bl
    set_data dword[new_node], bl
    popad
    mov %2, dword[new_node]
%endmacro

;%1 first node
;%2 second node
;%3 new node
;%4 first index
;%5 second index
%macro and_node 5
    pushad
    xor ecx, ecx
    xor edx, edx
    mov eax, %1
    mov ebx, %2
    get_node eax, %4
    get_node ebx, %5
    create_node
    get_data eax, cl
    get_data ebx, dl
    ;print_data dec_format, ecx
    ;print_data dec_format, edx
    and cl, dl
    set_data dword[new_node], cl
    popad
    mov %3, dword[new_node]
%endmacro

%macro add_node 6
    pushad
    xor ecx, ecx
    xor edx, edx
    mov eax, %1
    mov ebx, %2
    get_node eax, %4
    get_node ebx, %5
    create_node
    get_data eax, cl
    get_data ebx, dl
    ;print_data dec_format, ecx
    ;print_data dec_format, edx
    add ecx, edx
    add ecx, dword[carry]
    mov byte[carry], ch
    ;print_data dec_format, dword[carry]
    %%finish_add_node:
    set_data dword[new_node], cl
    popad
    mov %3, dword[new_node]
%endmacro

%macro drain_add_carry 1
    pushad
    ;print_data just_test, 0
    xor ecx, ecx
    mov eax, %1
    get_data eax, cl
    add ecx, dword[carry]
    set_data eax, cl
    mov byte[carry], ch
    popad
%endmacro

%macro add_last_node_case 1
    pushad
    xor ecx, ecx
    create_node
    mov cl, byte[carry]
    ;print_data dec_format, ecx
    mov edx, %1
    mov ebx, dword[new_node]
    set_data ebx, cl
    set_next ebx, edx
    popad
    mov edx, dword[new_node]
%endmacro

%macro or_node 5
    pushad
    xor ecx, ecx
    xor edx, edx
    mov eax, %1
    mov ebx, %2
    get_node eax, %4
    get_node ebx, %5
    create_node
    get_data eax, cl
    get_data ebx, dl
    ;print_data dec_format, ecx
    ;print_data dec_format, edx
    or cl, dl
    set_data dword[new_node], cl
    popad
    mov %3, dword[new_node]
%endmacro

;%1 a pointer to char
%macro hex_to_value 1
    pushad
    xor eax, eax
    mov ebx, %1
    mov al, byte[ebx]
    cmp al, 'A'
    jl %%number
    sub al, 'A'
    add al, 10
    jmp %%finished
    %%number:
    sub al, '0'
    %%finished:
    mov byte[hex_value], al
    popad
%endmacro

;%1 input data (e.g. "54563248")
%macro create_linked_list 1
    ;eax = new node
    ;ebx = prev node
    ;ecx = input data poiter
    ;edx = node data
    pushad
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    mov ecx, %1
    count_chars ecx
    mov esi, dword[count]
    and esi, 0x1
    ;print_data dec_format, esi
    mov ebx, dword[stack_top]
    dec ebx
    %%create_list_loop:
        cmp byte[ecx], 0
        je %%finished
        cmp byte[ecx], 10
        je %%finished
        hex_to_value ecx
        inc ecx
        mov dl, byte[hex_value]
        cmp esi, 1
        je %%skip_second_digit
        cmp byte[ecx], 0
        je %%skip_second_digit
        cmp byte[ecx], 10
        je %%skip_second_digit
        hex_to_value ecx
        inc ecx
        shl edx, 4
        or dl, byte[hex_value]
        %%skip_second_digit:
        mov esi, 0
        create_node
        mov eax, dword[new_node]
        set_next ebx, eax
        set_data eax, dl
        mov ebx, eax
        jmp %%create_list_loop
    %%finished:
        add dword[stack_top], 4
    popad
%endmacro

%macro print_data 2
    pushad
    push %2
    push %1
    call printf
    add esp, 8
    popad
%endmacro

%macro print_list 1
    ;ecx = current node
    ;ebx = data
    pushad
    xor ebx, ebx
    mov edx, %1
    %%print_loop:
        cmp edx, 0
        je %%finish_printing
        get_data edx, bl
        cmp ebx, 0
        jne print_no_digits
        ;print_data format_pointer, edx
        print_data hex_format, ebx
        jmp %%skip
        print_no_digits:
            print_data hex_format_no_two, ebx
        %%skip:
        get_next edx, edx
        jmp %%print_loop
    %%finish_printing:
    print_new_line
    popad
%endmacro

;%1 is the head
%macro free_list 1
    pushad
    mov eax, %1
    mov ebx, eax
    count_nodes eax
    mov ecx, dword[count]
    %%free_loop:
    mov eax, ebx
    cmp ecx, 0
    jl %%done
    get_node eax, ecx
    dec ecx
    free_Something eax
    jmp %%free_loop
    %%done:
    popad
%endmacro

%macro free_Something 1
    push %1
    call free
    add esp 4
%endmacro
main:
    push ebp
    mov ebp, esp
    mov eax, 5
    create_stack eax
main_loop:
    push calc
    call printf
    add esp, 4
    push dword [stdin]
    push 1024
    push user_input
    call fgets
    add esp, 12
    ;push user_input
    ;push format_string
    ;call printf
    ;add esp, 8
    cmp byte[user_input], 'q'
    je finish_program
    cmp byte[user_input], '+'
    je plus_input
    cmp byte[user_input], 'p'
    je pop_input
    cmp byte[user_input], 'd'
    je duplicate_input
    cmp byte[user_input], '&'
    je and_input
    cmp byte[user_input], '|'
    je or_input
    cmp byte[user_input], 'n'
    je count_input
    number_input:
        create_linked_list user_input
        jmp main_loop
    plus_input:
        ;eax = first number
        ;ebx = second number
        ;first_number_counter = first number size
        ;second_number_counter = second number size
        ;ecx = current node
        ;edx = previous node
        stack_pop eax   ;eax first to add
        mov ebx, eax    ;ebx second to add
        stack_pop eax   
        ;print_data format_pointer, eax
        ;print_data format_pointer, ebx
        count_nodes eax
        mov ecx, dword[count]
        count_nodes ebx
        mov edx, dword[count]
        mov dword[first_number_counter], ecx
        mov dword[second_number_counter], edx
        xor ecx, ecx
        mov byte[carry], 0
        add_node eax, ebx, ecx, dword[first_number_counter], dword[second_number_counter], byte[carry]
        mov edx, ecx
        dec dword[first_number_counter]
        dec dword[second_number_counter]
        add_loop:
            ;print_data format_pointer, ecx
            cmp dword[first_number_counter], 0
            je .drain_second
            cmp dword[second_number_counter], 0
            je .drain_first
            add_node eax, ebx, edx, dword[first_number_counter], dword[second_number_counter], byte[carry]
            dec dword[first_number_counter]
            dec dword[second_number_counter]
            set_next edx, ecx
            mov ecx, edx
            jmp add_loop
        .drain_first:
        mov ebx, eax ;ebx is head
        .drain_first_loop:
        ;print_data dec_format, dword[first_number_counter]
        mov eax, ebx
        cmp dword[first_number_counter], 0
        je finished_add
        get_node eax, dword[first_number_counter]
        dec dword[first_number_counter]
        drain_add_carry eax
        set_next eax, edx
        mov edx, eax
        jmp .drain_first_loop
        .drain_second:
        mov eax, ebx ;ebx is head
        .drain_second_loop:
        mov ebx, eax
        cmp dword[second_number_counter], 0
        je finished_add
        get_node ebx, dword[second_number_counter]
        dec dword[second_number_counter]
        drain_add_carry ebx
        set_next ebx, edx
        mov edx, ebx
        jmp .drain_second_loop
        finished_add:
        cmp byte[carry], 0
        je .end_add
        add_last_node_case edx
        .end_add:
        stack_push edx
        jmp main_loop
    pop_input:
        stack_pop eax
        ;print_data format_pointer, eax
        print_list eax
        jmp main_loop
    duplicate_input:
        stack_pop eax
        stack_push eax
        mov ebx, dword[stack_top]
        dec ebx
        duplicate_loop:
        cmp eax, 0
        je finished_duplicate
        duplicate_node eax, ecx
        set_next ebx, ecx
        mov ebx, ecx
        get_next eax, eax
        jmp duplicate_loop
        finished_duplicate:
        add dword[stack_top], 4
        jmp main_loop
    and_input:
        ;eax = first number
        ;ebx = second number
        ;first_number_counter = first number size
        ;second_number_counter = second number size
        ;ecx = current node
        ;edx = previous node
        stack_pop eax   ;eax first to and
        mov ebx, eax
        stack_pop eax   ;ebx second to and
        ;print_data format_pointer, eax
        ;print_data format_pointer, ebx
        count_nodes eax
        mov ecx, dword[count]
        count_nodes ebx
        mov edx, dword[count]
        mov dword[first_number_counter], ecx
        mov dword[second_number_counter], edx
        xor ecx, ecx
        and_node eax, ebx, ecx, dword[first_number_counter], dword[second_number_counter]
        mov edx, ecx
        dec dword[first_number_counter]
        dec dword[second_number_counter]
        and_loop:
            cmp dword[first_number_counter], 0
            je finished_and
            cmp dword[second_number_counter], 0
            je finished_and
            and_node eax, ebx, edx, dword[first_number_counter], dword[second_number_counter]
            dec dword[first_number_counter]
            dec dword[second_number_counter]
            set_next edx, ecx
            mov ecx, edx
            jmp and_loop
        finished_and:
        stack_push edx
        jmp main_loop
    or_input:
        ;eax = first number
        ;ebx = second number
        ;first_number_counter = first number size
        ;second_number_counter = second number size
        ;ecx = current node
        ;edx = previous node
        stack_pop eax   ;eax first to and
        mov ebx, eax
        stack_pop eax   ;ebx second to and
        ;print_data format_pointer, eax
        ;print_data format_pointer, ebx
        count_nodes eax
        mov ecx, dword[count]
        count_nodes ebx
        mov edx, dword[count]
        mov dword[first_number_counter], ecx
        mov dword[second_number_counter], edx
        xor ecx, ecx
        or_node eax, ebx, ecx, dword[first_number_counter], dword[second_number_counter]
        mov edx, ecx
        dec dword[first_number_counter]
        dec dword[second_number_counter]
        or_loop:
            cmp dword[first_number_counter], 0
            je drain_second
            cmp dword[second_number_counter], 0
            je drain_first
            or_node eax, ebx, edx, dword[first_number_counter], dword[second_number_counter]
            dec dword[first_number_counter]
            dec dword[second_number_counter]
            set_next edx, ecx
            mov ecx, edx
            jmp or_loop
        drain_first:
        mov ebx, eax ;ebx is head
        drain_first_loop:
        mov eax, ebx
        cmp dword[first_number_counter], 0
        je finished_or
        get_node eax, dword[first_number_counter]
        dec dword[first_number_counter]
        set_next eax, edx
        mov edx, eax
        jmp drain_first_loop
        drain_second:
        mov eax, ebx ;ebx is head
        drain_second_loop:
        mov ebx, eax
        cmp dword[second_number_counter], 0
        je finished_or
        get_node ebx, dword[first_number_counter]
        dec dword[second_number_counter]
        set_next ebx, edx
        mov edx, ebx
        jmp drain_second_loop
        finished_or:
        stack_push edx
        jmp main_loop
    count_input:
        xor edx, edx
        stack_pop eax
        count_nodes eax
        mov edx, dword[count]
        get_data eax, bl
        cmp bl, 15
        jle odd
        shl edx, 1
        jmp print_digits_count
        odd:
        shl edx, 1
        dec edx
        print_digits_count:
        ;print_data hex_format, edx
        print_new_line
        jmp main_loop
	finish_program:
        mov esp, ebp	
        pop ebp
        ret