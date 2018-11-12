.section .data
#ifndef TEST
#define TEST 20
#endif

	
	.macro linea
	#if TEST==1
		.int -1,-1,-1,-1
	#elif TEST==2
		.int 0x04000000,0x04000000,0x04000000,0x04000000
	#elif TEST==3
		.int 0x08000000,0x08000000,0x08000000,0x08000000
	#elif TEST==4
		.int 0x10000000,0x10000000,0x10000000,0x10000000
	#elif TEST==5
		.int 0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF
	#elif TEST==6
		.int 0x80000000,0x80000000,0x80000000,0x80000000
	#elif TEST==7
		.int 0xF0000000,0xF0000000,0xF0000000,0xF0000000
	#elif TEST==8
		.int 0xF8000000,0xF8000000,0xF8000000,0xF8000000
	#elif TEST==9
		.int 0xF7FFFFFF,0xF7FFFFFF,0xF7FFFFFF,0xF7FFFFFF
	#elif TEST==10
		.int 100000000,100000000,100000000,100000000
	#elif TEST==11
		.int 200000000,200000000,200000000,200000000
	#elif TEST==12
		.int 300000000,300000000,300000000,300000000
	#elif TEST==13
		.int 2000000000,2000000000,2000000000,2000000000
	#elif TEST==14
		.int 3000000000,3000000000,3000000000,3000000000
	#elif TEST==15
		.int -100000000,-100000000,-100000000,-100000000
	#elif TEST==16
		.int -200000000,-200000000,-200000000,-200000000
	#elif TEST==17
		.int -300000000,-300000000,-300000000,-300000000
	#elif TEST==18
		.int -2000000000,-2000000000,-2000000000,-2000000000
	#elif TEST==19
		.int -3000000000,-3000000000,-3000000000,-3000000000
	#elif TEST==20
		.int -3000000000,-3000000000,-3000000000,-3000000000 
		.int 0x80000000,0x80000000,0x80000000,0x80000000
		.int 0x80000000,0x80000000,0x80000000,0x80000000
	#endif
	.endm

lista:	.irpc i,1234
			linea
.endr
longlista:	.int   (.-lista)/4
resultado:	.quad  0 #Quad directive is used to define 64 bit numeric value(s).

formato:
.ascii "resultado \t =%lld (signed)\n"
.ascii "\t\t = 0x%llx (hex)\n"
.asciz "\t\t = 0x %08x %08x\n"


.section .text

_start:

	.global _start

	mov     $lista, %rbx
	mov  longlista, %r10
	call suma

	mov  %esi, resultado
	mov	 %edi, resultado+4


	
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
	mov  $0, %edx

	mov	 $0, %r9d
	mov  $0, %rcx

	mov  $0, %esi
	mov  $0, %edi

bucle:
	mov (%rbx,%rcx,4), %eax
	cdq

	add	  %eax,%esi
	adc	  %edx,%edi


	inc   %rcx
	cmp   %rcx,%r10
	jne    bucle

	ret



