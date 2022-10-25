        stack_pop eax   ;eax first to add
        stack_pop ebx   ;ebx second to add
        mov edx, dword[stack_top]   ;edx previous
        dec edx
        get_data eax, byte[data1]
        get_data ebx, byte[data2]
        add data1, data2
        create_node
        set_data dword[new_node], data1
        mov ecx, dword[new_node]
        stack_push ecx
        set_next edx, ecx
        mov edx, ecx
        cmp dword[eax+1],0
        je .first_no_next
        cmp dword[ebx+1],0
        je .second_no_next
        get_next eax, eax
        get_next ebx, ebx

        add_loop:
            cmp dword[eax+1],0
            je .first_no_next
            cmp dword[ebx+1],0
            je .second_no_next
            get_data eax, data1
            get_data ebx, data2
            adc data1, data2
            create_node
            set_data dword[new_node], data1
            mov ecx, dword[new_node]
            set_next edx, ecx
            mov edx, ecx
            jmp add_loop
        
        .first_no_next:
            cmp dword[ebx+1],0 ;second also has no next
            je last_add
            get_data eax, data1
            get_data ebx, data2
            add data1, data2
            create_node
            set_data dword[new_node], data1
            mov ecx, dword[new_node]
            stack_push ecx
            set_next edx, ecx
            mov edx, ecx







        stack_pop eax   ;eax first to and
        mov ebx, eax
        stack_pop eax   ;ebx second to and
        print_data format_pointer, eax
        print_data format_pointer, ebx
        mov edx, dword[stack_top]   ;edx previous
        dec edx
        ;doing it the first time outside the loop to stack_push ecx
        and_node eax, ebx, ecx  ;ecx output
        stack_push ecx
        set_next edx, ecx
        mov edx, ecx
        cmp dword[eax+1],0
        je finished_and
        cmp dword[ebx+1],0
        je finished_and
        get_next eax, eax
        get_next ebx, ebx
        
        and_loop:
            cmp dword[eax+1],0
            je last_and
            cmp dword[ebx+1],0
            je last_and
            and_node eax, ebx, ecx  
            set_next edx, ecx
            mov edx, ecx
            get_next eax, eax
            get_next ebx, ebx
            jmp and_loop
        
        ;and(ing) the last node of each one
        last_and:
            and_node eax, ebx, ecx
            set_next edx, ecx
        finished_and: