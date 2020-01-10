	.file	"serial.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	__vector_10
	.type	__vector_10, @function
__vector_10:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 12 */
.L__stack_usage = 12
	lds r24,tx_buzy
	tst r24
	breq .L2
	lds r24,bit_index
	cpse r24,__zero_reg__
	rjmp .L3
	cbi 0x18,4
	rjmp .L4
.L3:
	lds r24,bit_index
	cpi r24,lo8(9)
	brsh .L5
	lds r24,_tx_buffer
	sbrs r24,0
	rjmp .L6
	sbi 0x18,4
	rjmp .L7
.L6:
	cbi 0x18,4
.L7:
	lds r24,_tx_buffer
	lsr r24
	sts _tx_buffer,r24
	rjmp .L4
.L5:
	lds r24,bit_index
	cpi r24,lo8(9)
	brlo .L4
	sbi 0x18,4
	sts tx_buzy,__zero_reg__
.L4:
	lds r24,bit_index
	subi r24,lo8(-(1))
	sts bit_index,r24
.L2:
	lds r24,_1000us
	lds r25,_1000us+1
	subi r24,-103
	sbci r25,-1
.L13:
	sts _1000us+1,r25
	sts _1000us,r24
	lds r24,_1000us
	lds r25,_1000us+1
	cpi r24,-23
	sbci r25,3
	brlo .L15
	lds r18,_millis
	lds r19,_millis+1
	lds r20,_millis+2
	lds r21,_millis+3
	lds r22,_millis+4
	lds r23,_millis+5
	lds r24,_millis+6
	lds r25,_millis+7
	ldi r26,lo8(1)
	rcall __adddi3_s8
	sts _millis,r18
	sts _millis+1,r19
	sts _millis+2,r20
	sts _millis+3,r21
	sts _millis+4,r22
	sts _millis+5,r23
	sts _millis+6,r24
	sts _millis+7,r25
	lds r24,_1000us
	lds r25,_1000us+1
	subi r24,-24
	sbci r25,3
	rjmp .L13
.L15:
/* epilogue start */
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_10, .-__vector_10
.global	serial_write
	.type	serial_write, @function
serial_write:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L17:
	lds r25,tx_buzy
	cpse r25,__zero_reg__
	rjmp .L17
	sts bit_index,__zero_reg__
	sts _tx_buffer,r24
	ldi r24,lo8(1)
	sts tx_buzy,r24
	ret
	.size	serial_write, .-serial_write
.global	serial_putc
	.type	serial_putc, @function
serial_putc:
	push r28
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	mov r28,r24
	rcall serial_write
	mov r24,r28
	lsl r28
	sbc r25,r25
/* epilogue start */
	pop r28
	ret
	.size	serial_putc, .-serial_putc
.global	init_printf
	.type	init_printf, @function
init_printf:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,0
	ldi r23,0
	ldi r24,lo8(gs(serial_putc))
	ldi r25,hi8(gs(serial_putc))
	rjmp fdevopen
	.size	init_printf, .-init_printf
.global	millis
	.type	millis, @function
millis:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
/* #APP */
 ;  119 "serial.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r18,_millis
	lds r19,_millis+1
	lds r20,_millis+2
	lds r21,_millis+3
	lds r22,_millis+4
	lds r23,_millis+5
	lds r24,_millis+6
	lds r25,_millis+7
/* #APP */
 ;  121 "serial.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ret
	.size	millis, .-millis
.global	setup
	.type	setup, @function
setup:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x17,3
	sbi 0x18,3
	sbi 0x17,4
	sbi 0x18,4
	in r24,0x2a
	ori r24,lo8(2)
	out 0x2a,r24
	in r24,0x33
	ori r24,lo8(2)
	out 0x33,r24
	ldi r24,lo8(103)
	out 0x29,r24
	in r24,0x39
	ori r24,lo8(16)
	out 0x39,r24
/* #APP */
 ;  146 "serial.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	rjmp init_printf
	.size	setup, .-setup
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"toggle LED\r"
	.text
.global	loop
	.type	loop, @function
loop:
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
/* stack size = 8 */
.L__stack_usage = 8
	rcall millis
	lds r10,old_millis
	lds r11,old_millis+1
	lds r12,old_millis+2
	lds r13,old_millis+3
	lds r14,old_millis+4
	lds r15,old_millis+5
	lds r16,old_millis+6
	lds r17,old_millis+7
	rcall __subdi3
	cpi r18,-48
	sbci r19,7
	cpc r20,__zero_reg__
	cpc r21,__zero_reg__
	cpc r22,__zero_reg__
	cpc r23,__zero_reg__
	cpc r24,__zero_reg__
	cpc r25,__zero_reg__
	breq .L23
	brlo .L23
	in r25,0x18
	ldi r24,lo8(8)
	eor r24,r25
	out 0x18,r24
	rcall millis
	sts old_millis,r18
	sts old_millis+1,r19
	sts old_millis+2,r20
	sts old_millis+3,r21
	sts old_millis+4,r22
	sts old_millis+5,r23
	sts old_millis+6,r24
	sts old_millis+7,r25
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
/* epilogue start */
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	rjmp puts
.L23:
/* epilogue start */
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	ret
	.size	loop, .-loop
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	rcall setup
.L26:
	rcall loop
	rjmp .L26
	.size	main, .-main
	.comm	_tx_buffer,1,1
	.comm	bit_index,1,1
.global	tx_buzy
	.section .bss
	.type	tx_buzy, @object
	.size	tx_buzy, 1
tx_buzy:
	.zero	1
.global	old_millis
	.type	old_millis, @object
	.size	old_millis, 8
old_millis:
	.zero	8
.global	_1000us
	.type	_1000us, @object
	.size	_1000us, 2
_1000us:
	.zero	2
.global	_millis
	.type	_millis, @object
	.size	_millis, 8
_millis:
	.zero	8
	.ident	"GCC: (GNU) 5.4.0"
.global __do_copy_data
.global __do_clear_bss
