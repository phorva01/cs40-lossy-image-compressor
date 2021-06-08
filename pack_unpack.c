/* 
 * pack_unpack.c
 * Interface to use our bitpack interface to pack a, b, c, d and chroma values
 * into a 32-bit word.
 * Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 * COMP40 Spring 2019 - arith
 */

#include "pack_unpack.h"
#include "DCT_and_chroma.h"
#include <bitpack.h>
#include <stdio.h>
#include <stdlib.h>
#include <a2methods.h>
#include <assert.h>

static void pack_word(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);

static void unpack_word(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);

struct packed_arr *pack_bits(struct brights_chroma_arr *img_to_compress)
{
	struct packed_arr *packed_img = malloc(sizeof(struct packed_arr));
	assert(packed_img != NULL);

	packed_img->pixels = img_to_compress->methods->new(img_to_compress->width,
		img_to_compress->height, sizeof(uint64_t));

	packed_img->methods = img_to_compress->methods;

	packed_img->methods->map_row_major(img_to_compress->pixels, pack_word, 
		packed_img);

	img_to_compress->methods->free(&(img_to_compress->pixels));
	free(img_to_compress);

	return packed_img;
}

static void pack_word(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void)array2;

	struct brights_chroma_pix *pix = elem;
	struct packed_arr *new_img = cl;

	uint64_t word = 0;
	word = Bitpack_newu(word, 9, 23, (uint64_t)pix->a);
	word = Bitpack_news(word, 5, 18, (int64_t)pix->b);
	word = Bitpack_news(word, 5, 13, (int64_t)pix->c);
	word = Bitpack_news(word, 5, 8, (int64_t)pix->d);
	word = Bitpack_newu(word, 4, 4, (uint64_t)pix->pb);
	word = Bitpack_newu(word, 4, 0, (uint64_t)pix->pr);
	/* the above lbs and width values correspond to those in the
	 * arith specification */

	*((uint64_t *)new_img->methods->at(new_img->pixels, col, row)) = word;
}

struct brights_chroma_arr *unpack_bits(struct packed_arr *img_to_decompress)
{
	struct brights_chroma_arr *unpacked_img = 
		malloc(sizeof(struct brights_chroma_arr));
	assert(unpacked_img != NULL);

	unpacked_img->width = img_to_decompress->methods->
		width(img_to_decompress->pixels);

	unpacked_img->height = img_to_decompress->methods->
		height(img_to_decompress->pixels);

	unpacked_img->pixels = img_to_decompress->methods->new(
		unpacked_img->width, unpacked_img->height, 
		sizeof(struct brights_chroma_pix));

	unpacked_img->methods = img_to_decompress->methods;

	unpacked_img->methods->map_row_major(img_to_decompress->pixels, 
		unpack_word, unpacked_img);

	img_to_decompress->methods->free(&(img_to_decompress->pixels));
	free(img_to_decompress);

	return unpacked_img;
}

static void unpack_word(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void) array2;

	uint64_t word = *(uint64_t *)elem;
	struct brights_chroma_arr *unpacked_img = cl;

	uint64_t a = Bitpack_getu(word, 9, 23);
	int64_t b = Bitpack_gets(word, 5, 18);
	int64_t c = Bitpack_gets(word, 5, 13);
	int64_t d = Bitpack_gets(word, 5, 8);
	uint64_t pb = Bitpack_getu(word, 4, 4);
	uint64_t pr = Bitpack_getu(word, 4, 0);
	/* the above lbs and width values correspond to those in the
	 * arith specification */

	struct brights_chroma_pix *pix = malloc(sizeof(struct brights_chroma_pix));

	pix->a = a;
	pix->b = b;
	pix->c = c;
	pix->d = d;
	pix->pb = pb;
	pix->pr = pr;

	*((struct brights_chroma_pix *)unpacked_img->methods->
		at(unpacked_img->pixels, col, row)) = *pix;

	free(pix);
}
