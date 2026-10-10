.intel_syntax noprefix
.text
.global test_workload_execute_x86_64

# void test_asm_execute(wl_state_t *state)
test_workload_execute_x86_64:
    mov rcx, [rdi]       # array_len
    mov rsi, [rdi + 8]   # array pointer
    xor rax, rax         # i = 0

.loop:
    cmp rax, rcx
    jae .done

    mov rdx, [rsi + rax*8]  # read array[i]
    inc rax
    jmp .loop

.done:
    ret

.section .note.GNU-stack,"",@progbits
