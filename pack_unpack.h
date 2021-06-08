/* 
 * pack_unpack.h
 * Interface to use our bitpack interface to pack a, b, c, d and chroma values
 * into a 32-bit word.
 * Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 * COMP40 Spring 2019 - arith
 */

#include <stdlib.h>
#include "DCT_and_chroma.h"

#ifndef PACK_UNPACK_H
#define PACK_UNPACK_H

/* An UArray2 of words (long unsigneds) */
struct packed_arr {
	A2Methods_UArray2 pixels;
	const struct A2Methods_T *methods;
};

/* pack_bits
 * Input: the image as an array of a, b, c, d and chroma values
 * Output: the image as an array of packed words
 * Does: packs the a, b, c, d and chroma values of the image into 32-bit words
 *       stored in 64-bit unsigneds */
struct packed_arr *pack_bits(struct brights_chroma_arr *img_to_compress);

/* unpack_bits
 * Input: the image as an array of packed words
 * Output: the image as an array of a, b, c, d and chroma values
 * Does: unpacks each word to convert the image from an array of words to an
 *       array of a, b, c, d and chroma values */
struct brights_chroma_arr *unpack_bits(struct packed_arr *img_to_decompress);

#endif
