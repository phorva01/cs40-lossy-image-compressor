/*
 * bitpack.c
 * Implementation of the bitpacking interface for 64-bit words.
 * Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 * COMP40 Spring 2019 - arith
 */

#include <bitpack.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static uint64_t rshiftu(uint64_t n, unsigned bits);
static uint64_t lshiftu(uint64_t n, unsigned bits);

static uint64_t rshiftu(uint64_t n, unsigned bits)
{
	if (bits >= 64) {
		return (uint64_t)0;
	} else {
		return (n >> bits);
	}
}

static uint64_t lshiftu(uint64_t n, unsigned bits)
{
	if (bits >= 64) {
		return (uint64_t)0;
	} else {
		return (n << bits);
	}
}

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
	if (lshiftu((uint64_t)1, width) > n) { /* 2 to the nth power */
		return true;
	} else {
		return false;
	}
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
	int64_t max = lshiftu((uint64_t)1, (width - 1));
	/* there is a smaller range of magnitude for signed integers in 
 	 * two's complement */
	if ((max > n) && (~(max) < n)) {
		return true;
	} else {
		return false;
	}
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
	uint64_t mask = lshiftu(rshiftu(~(uint64_t)0, (64 - width)), lsb);
	uint64_t field = rshiftu((word & mask), lsb);
	return field;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
	uint64_t mask = lshiftu(rshiftu(~(uint64_t)0, (64 - width)), lsb);
	uint64_t field = rshiftu((word & mask), lsb);

	uint64_t sign_flag = lshiftu((uint64_t)1, lsb + width - 1);

	if (sign_flag & word) {
		field = rshiftu((word & mask), lsb);
		uint64_t sign_mask = lshiftu(~(uint64_t)0, width);
		field = field | sign_mask;
	} else {
		field = rshiftu((word & mask), lsb);
	}

	int64_t signed_field = (int64_t)field;

	return signed_field;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
	uint64_t value)
{
	if (!(Bitpack_fitsu(value, width))) {
		RAISE(Bitpack_Overflow);
	}
	
	uint64_t shifted_value = lshiftu(value, lsb);

	uint64_t antimask = ~(lshiftu(rshiftu(~(uint64_t)0, (64 - width)), lsb));
	uint64_t cleared_word = word & antimask;

	uint64_t new_word = cleared_word | shifted_value;
	return new_word;
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
	int64_t value)
{
	if (!(Bitpack_fitss(value, width))) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t shifted1 = lshiftu(value, 64 - width);
	uint64_t shifted2 = rshiftu(shifted1, 64 - (lsb + width));

	uint64_t antimask = ~(lshiftu(rshiftu(~(uint64_t)0, (64 - width)), lsb));

	uint64_t cleared_word = word & antimask;

	uint64_t new_word = cleared_word | shifted2;
	return new_word;
}

Except_T Bitpack_Overflow = { "Overflow packing bits" };
