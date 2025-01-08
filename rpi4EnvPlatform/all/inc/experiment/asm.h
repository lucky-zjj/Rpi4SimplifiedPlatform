    ldr x0, =0x80020000    // 将地址 0x20000 加载到寄存器 x0
    mov w1, #0xAB       // 将要写入的字节值 0xAB 加载到 w1
    strb w1, [x0]       // 将 w1 的字节值存储到地址 0x20000
    ldr x0, =0x80020000    // 将地址 0x20000 加载到寄存器 x0
    ldrb w1, [x0]       // 从地址 0x20000 读取一个字节到 w1


	ldr x0, =0x80020030    
    mov w1, #0xAB       
    strb w1, [x0]       
    ldr x0, =0x80020030    
    ldrb w1, [x0]       


	ldr x0, =0x80020033    
    mov w1, #0xAB       
    strb w1, [x0]       
    ldr x0, =0x80020033    
    ldrb w1, [x0]  
