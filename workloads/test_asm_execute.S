.intel_syntax noprefix
.text
.global test_asm_execute

# void test_asm_execute(wl_state_t *state)
test_asm_execute:
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
