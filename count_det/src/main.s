.text
.global count_det

count_det:
        // LDRB Rd, [Rb, Ro] - пред-индексное чтение байта из памяти:
        // Вычислить адрес ячейки памяти посредством сложения содержимого регистров Rb и Ro.
        // Прочитать байт из ячейки памяти по этому адресу и разместить его в регистре Rd.

        ldrb r1, [r0], #0
        ldrb r2, [r0], #4
        mul r2, r1, r2
        ldrb r1, [r0], #8
        mul r2, r2, r1
        add r3 r2

        ldrb r1, [r0], #2
        ldrb r2, [r0], #3
        mul r2, r1, r2
        ldrb r1, [r0], #7
        mul r2, r2, r1
        add r3 r3 r2

        ldrb r1, [r0], #1
        ldrb r2, [r0], #5
        mul r2, r1, r2
        ldrb r1, [r0], #6
        mul r2, r2, r1
        add r3 r3 r2

        ldrb r1, [r0], #2
        ldrb r2, [r0], #4
        mul r2, r1, r2
        ldrb r1, [r0], #6
        mul r2, r2, r1
        sub r3 r3 r2

        ldrb r1, [r0], #1
        ldrb r2, [r0], #3
        mul r2, r1, r2
        ldrb r1, [r0], #8
        mul r2, r2, r1
        sub r3 r3 r2

        ldrb r1, [r0], #0
        ldrb r2, [r0], #5
        mul r2, r1, r2
        ldrb r1, [r0], #7
        mul r2, r2, r1
        sub r0 r3 r2

        bx lr

//
