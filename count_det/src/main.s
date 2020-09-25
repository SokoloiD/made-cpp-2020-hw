.text

.global count_det
count_det:
		push {r4}
	push {r5}
	push {r6}
	push {r7}
	push {r8}
	
	ldr r7, [r0], #4	//a1
	mov r1, r7
	mov r4, r7
	ldr r7, [r0], #4	//b1
	mov r2, r7
	mov r5, r7
	ldr r7, [r0], #4	//c1
	mov r3, r7
	mov r6, r7
	ldr r7, [r0], #4	//a2
	mov r8, r3		
	mul r3, r8, r7		//a2*c1
	mov r8, r5
	mul r5, r8, r7		// -a2* b1//
	ldr r7, [r0], #4	//b2
	mov r8, r1
	mul r1, r8, r7		//a1 * b2
	mov r8, r6
	mul r6, r8, r7		//- b2 * c1  
	ldr r7, [r0], #4	//c2
	mov r8, r2
	mul r2, r8, r7		//b1 * c2
	mov r8, r4
	mul r4, r8, r7		//- a1 * c2
	ldr r7, [r0], #4	//a3
	mov r8, r2
	mul r2, r8, r7		//a3 * b1 * c2
	mov r8, r6
	mul r6, r8, r7		//-a3 * b2 * c1
	ldr r7, [r0], #4	//b3
	mov r8, r3
	mul r3,r8, r7		//a2 * c2 * b3
	mov r8, r4
	mul r4, r8, r7		//-a1 *c3 * b3
	ldr r7, [r0], #4	//c3
	mov r8, r1
	mul r1, r8, r7		//a1 * b2 * c3
	mov r8, r5
	mul r5, r8, r7		//-a2 * b1 * c3
	add r0, r1 ,r2		
	add r0, r3
	sub r0, r4
	sub r0, r5
	sub r0, r6
	
	pop {r8}
	pop {r7}
	pop {r6}
	pop {r5}
	pop {r4}
	bx lr
	
