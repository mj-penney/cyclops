.text
.global test_workload_execute_aarch64
.type test_workload_execute_aarch64, %function

// void test_workload_execute_aarch64(wl_state_t *state)
test_workload_execute_aarch64:
    ldr x1, [x0]          // array_len
    ldr x2, [x0, #8]      // array pointer
    mov x3, #0            // i = 0

.loop:
    cmp x3, x1
    b.hs .done            // unsigned i >= array_len

    ldr x4, [x2, x3, lsl #3]  // read array[i]
    add x3, x3, #1        // i++
    b .loop

.done:
    ret

.size test_workload_execute_aarch64, .-test_workload_execute_aarch64

.section .note.GNU-stack,"",%progbits
