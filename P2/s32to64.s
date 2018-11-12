
#6                              3               1 
#4                              2               6       8       1
#+------------------------------+---------------+-------+-------+
#|                             rax                              |
#+------------------------------+---------------+-------+-------+
#            (???)              |              eax              |
#                               +---------------+-------+-------+
#                                     (???)     |       ax      |
#                                               +-------+-------+
#                                               |  ah   |   al  |
#                                               +-------+-------+


.section .data
	.macro linea
	#	.int 1,1,1,1
	#	.int 2,2,2,2
	#	.int 1,2,3,4
	#	.int -1,-1,-1,-1
	#	.int 0x08000000,0x08000000,0x08000000,0x08000000
	#	.int 0x10000000,0x20000000,0x40000000,0x80000000
	#	.int 0xFC000000,0xFC000000,0xFC000000,0xFC000000
		.int 0xF8000000,0xF8000000,0xF8000000,0xF8000000

	.endm

lista:	.irpc i,12345678
			linea
.endr
longlista:	.int   (.-lista)/4
resultado:	.quad  0 #Quad directive is used to define 64 bit numeric value(s).
formato: 	.asciz	"suma = %lld = 0x%llx hex\n"  # Es esencial cambiar los caracteres especificadores del 
												# argumento para poder visualizar correctamente el numero por la salida.
												# elegiendo lu para ref prefijo L para convertir a long prefijo u hace referencia a unsigned, lx long hexadecimal



# opción: 1) no usar printf, 2)3) usar printf/fmt/exit, 4) usar tb main
# 1) as  suma.s -o suma.o
#    ld  suma.o -o suma					1232 B
# 2) as  suma.s -o suma.o				6520 B
#    ld  suma.o -o suma -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2
# 3) gcc suma.s -o suma -no-pie –nostartfiles		6544 B
# 4) gcc suma.s -o suma	-no-pie				8664 B

.section .text
_start: .global _start
 main: .global  main

	call trabajar	# subrutina de usuario
	call imprim_C	# printf()  de libC
#	call acabar_L	# exit()   del kernel Linux
	call acabar_C	# exit()    de libC
	ret

trabajar:
	mov     $lista, %rbx
	mov  longlista, %r10
	call suma		# == suma(&lista, longlista);
	mov  %rdx, resultado #resultado esperado suma = 137438953472 = 0x2000000000 hex
	#mov	 %eax, resultado+4
	ret

suma:

	mov  $0, %eax
	mov	 $0, %r9d
	mov  $0, %rcx
	mov  $0, %rdx
bucle:

	mov  (%rbx,%rcx,4), %eax
	movsx	%eax,%rax 
	#adc   $0,%r9d #sumamos acarreo en cada iteración.
	add		%rax,%rdx

	inc   %rcx
	cmp   %rcx,%r10
	jne    bucle



	ret

 imprim_C:			# requiere libC
 	mov   $formato, %rdi
 	mov   resultado,%rsi#hemos de cambiar tamaño registros destino para que en ambas representaciones, int y hex los numeros
 	mov   resultado,%rdx
 	mov          $0,%eax	# varargin sin xmm
 	call  printf		# == printf(formato, res, res);
 	ret

#acabar_L:
# 	mov        $60, %rax
#	mov  resultado, %edi
#	syscall			# == _exit(resultado)
#	ret

 acabar_C:			# requiere libC
 	mov  resultado, %edi
 	call _exit		# ==  exit(resultado)
 	ret
