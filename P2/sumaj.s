.section .data
#ifndef TEST
#define TEST 9
#endif

	
	.macro linea
	#if TEST==1
		.int 1,1,1,1
	#elif TEST==2
		.int 0x0FFFFFFF,0x0FFFFFFF,0x0FFFFFFF,0x0FFFFFFF
	#elif TEST==3
		.int 0x10000000,0x10000000,0x10000000,0x10000000
	#elif TEST==4
		.int 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
	#elif TEST==5
		.int -1,-1,-1,-1
	#elif TEST==6
		.int 200000000,200000000,200000000,200000000
	#elif TEST==7
		.int 300000000,300000000,300000000,300000000
	#elif TEST==8
		.int 5000000000,5000000000,5000000000,5000000000
	#elif TEST==9
		.int 5000000000,5000000000,5000000000,5000000000
		.int -1,-1,-1,-1
		.int  1,1,1,1

	#endif
	.endm

lista:	.irpc i,1234
			linea
.endr
longlista:	.int   (.-lista)/4
resultado:	.quad  0 #Quad directive is used to define 64 bit numeric value(s).

formato:
.ascii "resultado \t =%llu (uns)\n"
.ascii "\t\t = 0x%llx (hex)\n"
.asciz "\t\t = 0x %08x %08x\n"


.section .text

_start:

	.global _start

	mov     $lista, %rbx
	mov  longlista, %r10
	call suma

	mov  %eax, resultado
	mov	 %r9d, resultado+4


	mov   $formato, %rdi
 	mov   resultado,%rsi
 	mov   resultado,%rdx
 	mov	  resultado+4,%ecx
 	mov   resultado,%r8d

 	mov          $0,%eax	
 	call  printf	

 	mov  resultado, %edi
 	call _exit		
 	ret

suma:
	mov  $0, %eax
	mov	 $0, %r9d
	mov  $0, %rcx

bucle:
	add  (%rbx,%rcx,4), %eax
	jnc	 salto
	add	  $1,%r9d
salto:	
	inc   %rcx
	cmp   %rcx,%r10
	jne    bucle

	ret
