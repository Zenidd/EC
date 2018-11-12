// gcc popcount.c -o popcount -Og -g -D TEST=1


#include <stdio.h>		// para printf()
#include <stdlib.h>		// para exit()
#include <sys/time.h>		// para gettimeofday(), struct timeval

int resultado=0;

#ifndef TEST
#define TEST 5
#endif

/* -------------------------------------------------------------------- */
    #if TEST==1
    #define SIZE 4
    unsigned lista[SIZE]={0x80000000, 0x00400000, 0x00000200, 0x00000001};
    #define RESULT 4
/* -------------------------------------------------------------------- */
    #elif TEST==2
     #define SIZE 8
    unsigned lista[SIZE]={0x7fffffff, 0xffbfffff, 0xfffffdff, 0xfffffffe,
                          0x01000023, 0x00456700, 0x8900ab00, 0x00cd00ef};
    #define RESULT 156
/* -------------------------------------------------------------------- */
     #elif TEST==3
     #define SIZE 8
    unsigned lista[SIZE]={0x0       , 0x01020408, 0x35906a0c, 0x70b0d0e0,
                          0xffffffff, 0x12345678, 0x9abcdef0, 0xdeadbeef};
    #define RESULT 116                      
/* -------------------------------------------------------------------- */
    #elif TEST==4 || TEST==0
/* -------------------------------------------------------------------- */
    #define NBITS 20
    #define SIZE (1<<NBITS)// tamaño suficiente para tiempo apreciable
    unsigned lista[SIZE];// unsigned para desplazamiento derecha lógico
    #define RESULT ( NBITS * ( 1 << (NBITS-1) ) )// pistas para deducir fórmula
/* -------------------------------------------------------------------- */
    #else
    #error "Definir TEST entre 0..4"
    #endif
/* -------------------------------------------------------------------- */

    #define WSIZE 8*sizeof(unsigned)








int popcount1(unsigned* array, size_t len){
    size_t i,j;
    int result = 0;
    unsigned x=0;
    for(j=0; j < len ; j++){
    	x=array[j];
        for (i = 0; i < WSIZE; i++){
            result += x & 0x1;
            x >>= 1;
        }
    }
    return result;
}   


int popcount2(unsigned* array, size_t len){
	size_t j;
    unsigned result = 0;
   	unsigned x=0;
   	for(j=0; j < len ; j++){
   		x=array[j];
 	while (x){
       		result += x & 0x1;
       		 x >>= 1;
    }
}

return result;

}

int popcount3(unsigned* array, size_t len){
	int j;
	unsigned result = 0;
   	unsigned x = 0, val = 0;
   	for(j=0; j < len ; j++){
   		x=array[j];
   		asm("\n"		
			"ini3:					\n\t"		// seguir mientras que x!=0
			"shr 		%[x]		\n\t"		// LSB en CF
			"adc 	$0x0, %[r]		\n\t"
			"test  %[x],%[x]	    \n\t"
			"jnz ini3		   		\n\t"
			:[r]"+r" (result)		// e/salida: añadir a lo acumulado por el momento
			:[x] "r" (x)
			 );	// entrada: valor elemento
	}

	return result;
}   


int popcount4(unsigned* array, size_t len){
	size_t j;
	unsigned result = 0;
   	unsigned x=0, val=0;
   	for(j=0; j < len ; j++){
   		x=array[j];										
   								
		asm(
			"clc 			\n\t" //; pone bit de carry a 0 ( 1ª iteracion)
		"ini4:				\n\t"
			"adc $0, %[r]	\n\t" //: sumo el bit de carry en x
			"shr 	 %[x]	\n\t" //; desplazo a la derecha
			"jne ini4		\n\t" //salto si la palabra no es 0 en su totalidad
			"adc $0, %[r]	\n\t" // sumo el ultimo bit de carry 
			:[r]"+r" (result)		// e/salida: añadir a lo acumulado por el momento
			:[x] "r" (x)		);	// entrada: valor elemento
	}
	return result;
}

int popcount5(unsigned* array, size_t len){
	size_t i,j;
	unsigned val = 0, x = 0;
	unsigned result=0;
	

	for(j = 0; j < len; j++){
		x=array[j];
		val=0;
		for (i = 0; i < 8; i++) {
			val += x & 0x01010101;
			x >>= 1;
		}
		val += (val >> 16);
		val += (val >> 8);
		result+=(val & 0xFF);

	}

	return result;

}

	const unsigned m1  = 0x55555555; //binary: 0101...
	const unsigned m2  = 0x33333333; //binary: 00110011..
	const unsigned m4  = 0x0f0f0f0f; //binary: 4 zeros, 4 ones ...
	const unsigned m8  = 0x00ff00ff; //binary: 8 zeros, 8 ones ...
	const unsigned m16 = 0x0000ffff; //binary: 16 zeros, 16 ones ...


int popcount6(unsigned* array, size_t len){
	size_t j;
	unsigned x;
	unsigned result=0;
	for(j=0; j<len; j++){
		x=array[j];
		x = (x & m1 ) + ((x >> 1) & m1 ); //put count of each 2 bits into
		x = (x & m2 ) + ((x >> 2) & m2 ); //put count of each 4 bits into
		x = (x & m4 ) + ((x >> 4) & m4 ); //put count of each 8 bits into
		x = (x & m8 ) + ((x >> 8) & m8 ); //put count of each 16 bits into
		x = (x & m16) + ((x >> 16) & m16); //put count of each 32 bits into
		result+=x;
	}

	return result;
}



int popcount7(unsigned* array, size_t len){
	size_t i;
	unsigned x,x1,x2,x3;
	int result=0;

	if (len & 0x3) printf("leyendo 128b pero len no múltiplo de 4\n");
		for (i=0; i<len; i+=4){
			x = array[i];
			x1 = array[i+1];
			x2 = array[i+2];
			x3 = array[i+3];

			x = (x & m1 ) + ((x >> 1) & m1 ); //put count of each 2 bits into
			x = (x & m2 ) + ((x >> 2) & m2 ); //put count of each 4 bits into
			x = (x & m4 ) + ((x >> 4) & m4 ); //put count of each 8 bits into
			x = (x & m8 ) + ((x >> 8) & m8 ); //put count of each 16 bits into
			x = (x & m16) + ((x >> 16) & m16); //put count of each 32 bits into

			x1 = (x1 & m1 ) + ((x1 >> 1) & m1 ); //put count of each 2 bits into
			x1 = (x1 & m2 ) + ((x1 >> 2) & m2 ); //put count of each 4 bits into
			x1 = (x1 & m4 ) + ((x1 >> 4) & m4 ); //put count of each 8 bits into
			x1 = (x1 & m8 ) + ((x1 >> 8) & m8 ); //put count of each 16 bits into
			x1 = (x1 & m16) + ((x1 >> 16) & m16); //put count of each 32 bits into

			x2 = (x2 & m1 ) + ((x2 >> 1) & m1 ); //put count of each 2 bits into
			x2 = (x2 & m2 ) + ((x2 >> 2) & m2 ); //put count of each 4 bits into
			x2 = (x2 & m4 ) + ((x2 >> 4) & m4 ); //put count of each 8 bits into
			x2 = (x2 & m8 ) + ((x2 >> 8) & m8 ); //put count of each 16 bits into
			x2 = (x2 & m16) + ((x2 >> 16) & m16); //put count of each 32 bits into

			x3 = (x3 & m1 ) + ((x3 >> 1) & m1 ); //put count of each 2 bits into
			x3 = (x3 & m2 ) + ((x3 >> 2) & m2 ); //put count of each 4 bits into
			x3 = (x3 & m4 ) + ((x3 >> 4) & m4 ); //put count of each 8 bits into
			x3 = (x3 & m8 ) + ((x3 >> 8) & m8 ); //put count of each 16 bits into
			x3 = (x3 & m16) + ((x3 >> 16) & m16); //put count of each 32 bits into

			result+= x+x1+x2+x3;
		}
	return result;
}

int popcount8(unsigned* array, size_t len){
	size_t i;
	int val, result=0;
	int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
	int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
	if (len & 0x3) printf("leyendo 128b pero len no múltiplo de 4\n");
	for (i=0; i<len; i+=4){
		
		asm("movdqu 	  %[x]	, %%xmm0	\n\t"
			"movdqa 	%%xmm0 	, %%xmm1 	\n\t" //; x: two copies xmm0-1
			"movdqu		  %[m] 	, %%xmm6 	\n\t" //; mask: xmm6
			"psrlw  		$4 	, %%xmm1 	\n\t" //; switch to double word		
			"pand 		%%xmm6	, %%xmm0 	\n\t" //; xmm0 – lower nibbles
			"pand		%%xmm6	, %%xmm1 	\n\t" //; xmm1 – higher nibbles

			"movdqu		  %[l]	, %%xmm2 	\n\t" //; since instruction pshufb modifies LUT
			"movdqa		%%xmm2	, %%xmm3 	\n\t" //; we need 2 copies
			"pshufb		%%xmm0	, %%xmm2 	\n\t" //; xmm2 = vector of popcount lower nibbles
			"pshufb		%%xmm1	, %%xmm3 	\n\t" //; xmm3 = vector of popcount upper nibbles
			
			"paddb		%%xmm2	, %%xmm3 	\n\t" //; - vector of popcount for bytes
			"pxor		%%xmm0	, %%xmm0 	\n\t" //; = 0,0,0,0
			"psadbw		%%xmm0	, %%xmm3 	\n\t" //;  [pcnt bytes0..7|pcnt bytes8..15]
			"movhlps	%%xmm3	, %%xmm0 	\n\t" //;   [	 0	 	  |pcnt bytes0..7 ]
			"paddb		%%xmm3	, %%xmm0 	\n\t" //;   [ not needed  |pcnt bytes0..15]
			"movd		%%xmm0	, %[val]	\n\t"
			: [val]"=r" (val)
			: [x] "m" (array[i]),
			  [m] "m" (SSE_mask[0]),
			  [l] "m" (SSE_LUTb[0])
		);
			result += val;
	}

	return result;
}




int popcount9(unsigned* array, size_t len){
	size_t i;
	unsigned x;
	int val, result=0;

	for(i=0; i<len; i++){
		x = array[i];
		asm( "popcnt %[x], %[val]"
			 : [val] "=r" (val)
			 : [x] "r" (x)
		);
		result += val;
	}
	return result;
}


int popcount10(unsigned* array, size_t len){
	size_t i;
	unsigned x, x1, x2, x3;
	unsigned val=0; int result=0;
	if (len & 0x3) printf("leyendo 128b pero len no múltiplo de 4\n");
	for (i=0; i<len; i+=4) {

		x  = array[i];
		x1 = array[i+1];
		x2 = array[i+2];
		x3 = array[i+3];

		asm(
			"popcnt %[x]  , %[val] 	\n\t"
			"popcnt %[x1] , %%ebx  	\n\t"
			"popcnt %[x2] , %%ecx 	\n\t"
			"popcnt %[x3] , %%edx 	\n\t"
			"add 	%%ebx , %[val]	\n\t"
			"add 	%%ecx , %[val]	\n\t"
			"add 	%%edx , %[val]	\n\t"
			: [val]"=r" (val)
			: [x]  "r" (x),
			  [x1] "r" (x1),
			  [x2] "r" (x2),
			  [x3] "r" (x3)
			: "ebx","ecx","edx"
		);
		result += val;
	}
	return result;	
}





void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2; 			// gettimeofday() secs-usecs
    long           tv_usecs;			// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
    
    #if TEST==0
             printf("%ld" "\n",tv_usecs);
 
    #else
            printf("resultado = %d\t", resultado);
            printf("%s:%9ld us\n", msg, tv_usecs);
    #endif
}

int main()
{
    // inicializar array

    #if TEST==0 || TEST==4
    size_t i;
    for (i=0; i<SIZE; i++)
    lista[i]=i;
    #endif

crono(popcount1 ,"popcount1 (lenguaje C - for)");
crono(popcount2 , "popcount2 (lenguaje C - while)");
crono(popcount3 , "popcount3 (leng.ASM-body while 4i)");
crono(popcount4 , "popcount4 (leng.ASM-body while 3i)");
crono(popcount5 , "popcount5 (CS:APP2e 3.49-group 8b)");
crono(popcount6 , "popcount6 (Wikipedia- naive - 32b)");
crono(popcount7 , "popcount7 (Wikipedia- naive -128b)");
crono(popcount8 , "popcount8 (asm SSE3 - pshufb 128b)");
crono(popcount9 , "popcount9 (asm SSE4- popcount 32b)");
crono(popcount10, "popcount10(asm SSE4- popcount128b)");



#if TEST != 0
	printf("calculado = %d\n", RESULT);
#endif
exit(0);

}






