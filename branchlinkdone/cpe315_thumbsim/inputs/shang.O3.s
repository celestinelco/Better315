	.arch armv6
	.fpu softvfp
	.code	16
	.file	"shang.c"
	.text
	.align	2
	.global	initGame
	.code	16
	.thumb_func
	.type	initGame, %function
initGame:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	mov	r3, #3
	push	{r6, r7}
	mov	r7, r0
	mov	r6, r1
	mov	ip, r0
	mov	r8, r1
	mov	r4, r0
	mov	r2, r1
	add	r7, r7, #4
	add	r6, r6, #4
	mov	r0, #40
	mov	r9, r3
	mov	r1, #0
	b	.L2
.L20:
	mov	r3, r1
	mov	r5, r1
	cmp	r2, r7
	adc	r3, r3, r1
	cmp	r4, r6
	adc	r5, r5, r1
	orr	r3, r5
	beq	.L8
	ldr	r5, [r4]
	sub	r0, r0, #1
	str	r5, [r2]
	ldr	r3, [r7]
	add	r7, r7, #21
	str	r3, [r6]
	ldr	r5, [r4, #8]
	add	r6, r6, #21
	str	r5, [r2, #8]
	ldr	r3, [r4, #12]
	str	r3, [r2, #12]
	ldr	r5, [r4, #16]
	str	r5, [r2, #16]
	ldrb	r3, [r4, #20]
	add	r4, r4, #21
	strb	r3, [r2, #20]
	add	r2, r2, #21
	cmp	r0, #0
	beq	.L5
.L2:
	mov	r3, r4
	orr	r3, r2
	mov	r5, r9
	tst	r5, r3
	beq	.L20
.L8:
	mov	r3, #0
.L4:
	ldrb	r5, [r4, r3]
	strb	r5, [r2, r3]
	add	r3, r3, #1
	cmp	r3, #21
	bne	.L4
	sub	r0, r0, #1
	add	r4, r4, #21
	add	r2, r2, #21
	add	r7, r7, #21
	add	r6, r6, #21
	cmp	r0, #0
	bne	.L2
.L5:
	mov	r3, #210
	lsl	r3, r3, #2
	mov	r1, r3
	mov	r2, r3
	add	r1, r1, ip
	add	r2, r2, r8
	mov	r3, #20
.L10:
	ldmia	r1!, {r0}
	sub	r3, r3, #1
	stmia	r2!, {r0}
	cmp	r3, #0
	bne	.L10
	mov	r3, #230
	lsl	r3, r3, #2
	mov	r0, r3
	mov	r2, r3
	add	r0, r0, ip
	add	r2, r2, r8
	mov	r3, #40
.L12:
	ldmia	r0!, {r1}
	sub	r3, r3, #1
	stmia	r2!, {r1}
	cmp	r3, #0
	bne	.L12
	@ sp needed
	pop	{r2, r3}
	mov	r8, r2
	mov	r9, r3
	pop	{r4, r5, r6, r7, pc}
	.size	initGame, .-initGame
	.align	2
	.global	my_strncmp
	.code	16
	.thumb_func
	.type	my_strncmp, %function
my_strncmp:
	push	{r4, r5, lr}
	cmp	r2, #0
	ble	.L25
	ldrb	r4, [r0]
	ldrb	r3, [r1]
	cmp	r4, r3
	bcc	.L28
	bhi	.L29
	mov	r3, #0
	b	.L23
.L24:
	ldrb	r5, [r0, r3]
	ldrb	r4, [r1, r3]
	cmp	r5, r4
	bcc	.L28
	bhi	.L29
.L23:
	add	r3, r3, #1
	cmp	r3, r2
	bne	.L24
.L25:
	mov	r0, #0
.L22:
	@ sp needed
	pop	{r4, r5, pc}
.L28:
	mov	r0, #1
	neg	r0, r0
	b	.L22
.L29:
	mov	r0, #1
	b	.L22
	.size	my_strncmp, .-my_strncmp
	.align	2
	.global	my_strcpy
	.code	16
	.thumb_func
	.type	my_strcpy, %function
my_strcpy:
	mov	r3, #0
	push	{lr}
.L32:
	ldrb	r2, [r1, r3]
	strb	r2, [r0, r3]
	add	r3, r3, #1
	ldrb	r2, [r1, r3]
	cmp	r2, #0
	bne	.L32
	@ sp needed
	pop	{pc}
	.size	my_strcpy, .-my_strcpy
	.align	2
	.global	CheckConstraint
	.code	16
	.thumb_func
	.type	CheckConstraint, %function
CheckConstraint:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	ldr	r4, .L51
	push	{r6, r7}
	add	sp, sp, r4
	mov	ip, r0
	mov	r6, r1
	mov	r7, r0
	add	r1, sp, #168
	add	r0, r0, #20
.L34:
	mov	r2, #210
	lsl	r2, r2, #1
	add	r5, r7, r2
	mov	r3, r7
	mov	r2, r1
.L37:
	ldrb	r4, [r3]
	add	r3, r3, #21
	strb	r4, [r2]
	add	r2, r2, #1
	cmp	r3, r5
	bne	.L37
	add	r7, r7, #1
	add	r1, r1, #20
	cmp	r7, r0
	bne	.L34
	mov	r3, #230
	lsl	r3, r3, #2
	mov	r1, r3
	add	r1, r1, ip
	mov	r2, #160
	add	r0, sp, #8
	bl	memcpy
	add	r2, sp, #168
	mov	r8, r2
	mov	r2, #200
	lsl	r2, r2, #1
	ldr	r3, .L51+4
	add	r8, r8, r2
	mov	r2, #0
	add	r5, sp, #168
	mov	r9, r3
	mov	ip, r2
.L38:
	mov	r4, r9
	mov	r1, #0
.L45:
	lsl	r7, r1, #2
	add	r3, sp, #8
	ldr	r3, [r3, r7]
	cmp	r3, #0
	beq	.L39
	cmp	r6, #0
	blt	.L40
	ldrb	r2, [r4]
	ldrb	r3, [r5]
	cmp	r2, r3
	bne	.L39
	mov	r3, #0
	b	.L41
.L42:
	ldrb	r0, [r4, r3]
	ldrb	r2, [r5, r3]
	cmp	r0, r2
	bne	.L39
.L41:
	add	r3, r3, #1
	cmp	r6, r3
	bge	.L42
.L40:
	add	r2, sp, #8
	mov	r3, ip
	add	r5, r5, #20
	str	r3, [r2, r7]
	cmp	r5, r8
	bne	.L38
	mov	r0, #1
.L50:
	mov	r3, #143
	lsl	r3, r3, #2
	add	sp, sp, r3
	@ sp needed
	pop	{r2, r3}
	mov	r8, r2
	mov	r9, r3
	pop	{r4, r5, r6, r7, pc}
.L39:
	add	r1, r1, #1
	add	r4, r4, #21
	cmp	r1, #40
	bne	.L45
	mov	r0, #0
	b	.L50
.L52:
	.align	2
.L51:
	.word	-572
	.word	.LANCHOR0
	.size	CheckConstraint, .-CheckConstraint
	.align	2
	.global	SolveGame
	.code	16
	.thumb_func
	.type	SolveGame, %function
SolveGame:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r6, r10
	mov	r7, fp
	mov	r5, r9
	mov	r8, r0
	push	{r4, r5, r6, r7}
	mov	r6, r1
	ldr	r4, .L88
	add	sp, sp, r4
	cmp	r1, #20
	bne	.LCB334
	b	.L76	@long jump
.LCB334:
	mov	r1, #230
	lsl	r1, r1, #2
	mov	r2, r1
	lsl	r3, r6, #1
	add	r2, r2, r8
	add	r3, r3, r6
	ldr	r1, .L88+4
	str	r2, [sp, #16]
	mov	r9, r2
	str	r3, [sp, #20]
	mov	r2, #0
	lsl	r3, r3, #3
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	str	r3, [sp, #28]
	str	r0, [sp, #24]
	b	.L72
.L55:
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #12]
	add	r2, r2, #1
	mov	r3, #4
	add	r1, r1, #21
	str	r2, [sp, #8]
	add	r9, r9, r3
	str	r1, [sp, #12]
	cmp	r2, #40
	bne	.LCB365
	b	.L87	@long jump
.LCB365:
.L72:
	mov	r3, r9
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L55
	mov	r3, #210
	lsl	r3, r3, #2
	ldr	r5, [sp, #24]
	mov	r4, #0
	mov	r8, r3
.L56:
	add	r1, sp, #592
	add	r0, r1, r4
	mov	r2, #21
	add	r1, r5, r4
	add	r4, r4, #21
	bl	memcpy
	cmp	r4, r8
	bne	.L56
	ldr	r2, [sp, #24]
	mov	r0, #179
	mov	r3, #210
	lsl	r3, r3, #2
	lsl	r0, r0, #3
	add	r1, r2, r3
	add	r0, r0, sp
	mov	r2, #80
	bl	memcpy
	mov	r0, #189
	lsl	r0, r0, #3
	ldr	r1, [sp, #16]
	mov	r2, #160
	add	r0, r0, sp
	bl	memcpy
	mov	r3, r6
	add	r3, r3, #210
	ldr	r2, [sp, #8]
	add	r1, sp, #592
	lsl	r3, r3, #2
	ldr	r0, [sp, #12]
	str	r2, [r1, r3]
	lsl	r3, r2, #2
	add	r3, r1, r3
	mov	r1, #230
	lsl	r1, r1, #2
	add	r3, r3, r1
	mov	r2, #0
	str	r2, [r3]
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #20]
	sub	r1, r2, r3
	ldr	r3, [sp, #12]
	add	r2, sp, #592
	add	r1, r2, r1
	ldrb	r2, [r3]
	mov	r3, #0
.L60:
	strb	r2, [r1, r3]
	add	r3, r3, #1
	ldrb	r2, [r0, r3]
	cmp	r2, #0
	bne	.L60
	add	r1, sp, #592
	mov	ip, r1
	mov	r1, #20
	add	ip, ip, r1
	add	r0, sp, #192
	add	r4, sp, #592
	mov	r5, ip
.L59:
	mov	r2, r4
	mov	r3, #0
.L63:
	ldrb	r1, [r2]
	add	r2, r2, #21
	strb	r1, [r0, r3]
	add	r3, r3, #1
	cmp	r3, #20
	bne	.L63
	add	r4, r4, #1
	add	r0, r0, #20
	cmp	r4, r5
	bne	.L59
	mov	r1, #189
	lsl	r1, r1, #3
	mov	r2, #160
	add	r0, sp, #32
	add	r1, r1, sp
	bl	memcpy
	add	r2, sp, #192
	mov	fp, r2
	mov	r2, #200
	lsl	r2, r2, #1
	mov	r3, #0
	add	fp, fp, r2
	add	r0, sp, #192
	mov	r10, r3
.L64:
	ldr	r4, .L88+4
	mov	r5, #0
.L71:
	lsl	r7, r5, #2
	add	r1, sp, #32
	ldr	r1, [r1, r7]
	cmp	r1, #0
	beq	.L65
	cmp	r6, #0
	blt	.L66
	ldrb	r2, [r4]
	ldrb	r3, [r0]
	cmp	r2, r3
	bne	.L65
	mov	r3, #0
	b	.L67
.L68:
	ldrb	r1, [r4, r3]
	ldrb	r2, [r0, r3]
	cmp	r1, r2
	bne	.L65
.L67:
	add	r3, r3, #1
	cmp	r6, r3
	bge	.L68
.L66:
	add	r2, sp, #32
	mov	r3, r10
	add	r0, r0, #20
	str	r3, [r2, r7]
	cmp	r0, fp
	bne	.L64
	add	r1, r6, #1
	add	r0, sp, #592
	bl	SolveGame
	cmp	r0, #0
	bne	.LCB513
	b	.L55	@long jump
.LCB513:
.L76:
	mov	r0, #1
	b	.L84
.L65:
	add	r5, r5, #1
	add	r4, r4, #21
	cmp	r5, #40
	bne	.L71
	b	.L55
.L87:
	mov	r0, #0
.L84:
	ldr	r3, .L88+8
	add	sp, sp, r3
	@ sp needed
	pop	{r2, r3, r4, r5}
	mov	r8, r2
	mov	r9, r3
	mov	r10, r4
	mov	fp, r5
	pop	{r4, r5, r6, r7, pc}
.L89:
	.align	2
.L88:
	.word	-1676
	.word	.LANCHOR0
	.word	1676
	.size	SolveGame, .-SolveGame
	.section	.text.startup,"ax",%progbits
	.align	2
	.global	main
	.code	16
	.thumb_func
	.type	main, %function
main:
	push	{r7, lr}
	mov	r2, #1
	ldr	r7, .L93
	add	sp, sp, r7
	add	r1, sp, #1020
	add	r3, sp, #920
	add	r1, r1, #60
.L92:
	stmia	r3!, {r2}
	cmp	r3, r1
	bne	.L92
	mov	r0, sp
	mov	r1, #0
	bl	SolveGame
	mov	r3, #135
	lsl	r3, r3, #3
	add	sp, sp, r3
	@ sp needed
	pop	{r7, pc}
.L94:
	.align	2
.L93:
	.word	-1080
	.size	main, .-main
	.global	g_mainSetting
	.data
	.align	2
.LANCHOR0 = . + 0
	.type	g_mainSetting, %object
	.size	g_mainSetting, 840
g_mainSetting:
	.ascii	"                   *\000"
	.ascii	"                  **\000"
	.ascii	"       *     *  *   \000"
	.ascii	"           *   ***  \000"
	.ascii	"*   *          *  * \000"
	.ascii	"           **    * *\000"
	.ascii	"  * **  *     *     \000"
	.ascii	"            *****   \000"
	.ascii	"   **    *   * *    \000"
	.ascii	"          * *   *** \000"
	.ascii	" * *   *   *  *     \000"
	.ascii	"      **    * *  *  \000"
	.ascii	"  *  *  *     * *   \000"
	.ascii	"          *  *  ** *\000"
	.ascii	" ***   *         *  \000"
	.ascii	" *  * * *   * *     \000"
	.ascii	"   **  * *    * *   \000"
	.ascii	"  *****           * \000"
	.ascii	"  *  *   * **  *    \000"
	.ascii	"    *  *  * * * *   \000"
	.ascii	"         * ***   ** \000"
	.ascii	"     ** *  *   * *  \000"
	.ascii	"        *** * *   * \000"
	.ascii	"  ***   *  *  *     \000"
	.ascii	"       *  ** * *  * \000"
	.ascii	"   ** * * *  * *    \000"
	.ascii	"    *  *  * *** *   \000"
	.ascii	"*****         *    *\000"
	.ascii	"   **    * * ***    \000"
	.ascii	"*  * * ***    *     \000"
	.ascii	"         ***  ** * *\000"
	.ascii	"     ** *   ** * *  \000"
	.ascii	"***   * *      ***  \000"
	.ascii	"      **   ** *** * \000"
	.ascii	" * * **  *** *      \000"
	.ascii	"* *** ***   **      \000"
	.ascii	"        *** * ** ***\000"
	.ascii	" **** **       ***  \000"
	.ascii	"**   *** *   **  ** \000"
	.ascii	"*   * * *****  **  *\000"
