#include <stdio.h>
#include <string.h>
#include "cham.h"

union int32to64
{
	__int64 i64;
	int i32[2];
};

void read_tsc(int *cl_Low, int *cl_High)
{
	__asm {
		_emit 0Fh
		_emit 31h
		mov ecx, dword ptr cl_Low
		mov[ecx], eax
		mov ecx, dword ptr cl_High
		mov[ecx], edx
	}
}

void print_bytes(char *s, void *p, int len) {
	int i;
	printf("%s : ", s);
	for (i = 0; i<len; i++) {
		printf("%04x ", ((uint16_t*)p)[i]);
	}
	putchar('\n');
}

uint16_t key64[] =
{
	0x0100, 0x0302, 0x0504, 0x0706,
	0x0908, 0x0b0a,	0x0d0c, 0x0f0e
};

uint16_t plain64[] =
{
	0x1100, 0x3322, 0x5544, 0x7766
};

uint16_t cipher64[] =
{
	0x453c, 0x63bc, 0xdcfa, 0xbf4e
};

uint32_t key128[] =
{
	0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0xf3f2f1f0, 0xf7f6f5f4, 0xfbfaf9f8, 0xfffefdfc
};

//0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c, 0xf3f2f1f0, 0xf7f6f5f4, 0xfbfaf9f8, 0xfffefdfc //128/256

uint32_t plain128[] =
{
	0x33221100, 0x77665544, 0xbbaa9988, 0xffeeddcc
};

uint32_t cipher128[] =
{
	0xa899c8a0, 0xc929d55c, 0xab670d38, 0x0c4f7ac8
};
//	0xa899c8a0, 0xc929d55c, 0xab670d38, 0x0c4f7ac8 // 128/256
//  0xc3746034, 0xb55700c5, 0x8d64ec32, 0x489332f7 // 128/128
int main(void)
{
	// 64/128
	
	union int32to64 a, b;	//
	__int64 hz = 0;			//

	uint16_t roundkey[16];
	uint64_t buf1[2], buf2[2];

	printf("***CHAM-64/128***\n\n");

	memcpy(buf1, plain64, 8);
	memcpy(buf2, cipher64, 8);

	print_bytes("K", key64, 8);
	print_bytes("P", plain64, 4);
	print_bytes("C", cipher64, 4);

	printf("\n");
	read_tsc(&a.i32[0], &a.i32[1]);	//
	cham64_setkey(key64, roundkey);
	read_tsc(&b.i32[0], &b.i32[1]);	//
	hz = b.i64 - a.i64;				//
	printf("Å¬·° °ª : %I64dhz\n", hz); //
	cham64_encrypt(roundkey, buf1);
	print_bytes("encrypt", buf1, 4);
	cham64_decrypt(roundkey, buf2);
	print_bytes("decrypt", buf2, 4);
	
	/*
	// 128/128
	uint32_t roundkey[16];
	uint64_t buf1[2], buf2[2];

	printf("***CHAM-128/128***\n\n");

	memcpy(buf1, plain128, 16);
	memcpy(buf2, cipher128, 16);

	print_bytes("K", key128, 4);
	print_bytes("P", plain128, 4);
	print_bytes("C", cipher128, 4);

	printf("\n");

	cham128_setkey(key128, roundkey);
	cham128_encrypt(roundkey, buf1);
	print_bytes("encrypt", buf1, 4);
	cham128_decrypt(roundkey, buf2);
	print_bytes("decrypt", buf2, 4);
	*/
	/*
	uint32_t roundkey[16];
	uint64_t buf1[2], buf2[2];

	printf("***CHAM-128/256***\n\n");

	memcpy(buf1, plain128, 16);
	memcpy(buf2, cipher128, 16);

	print_bytes("K", key128, 8);
	print_bytes("P", plain128, 4);
	print_bytes("C", cipher128, 4);

	printf("\n");

	cham128_setkey(key128, roundkey);
	cham128_encrypt(roundkey, buf1);
	print_bytes("encrypt", buf1, 4);
	cham128_decrypt(roundkey, buf2);
	print_bytes("decrypt", buf2, 4);
	*/
	return 0;
} 