/*
 * read_write_compressed.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "pack_unpack.h"
#include "read_write_compressed.h"
#include <a2methods.h>
#include <assert.h>
#include "a2plain.h"
#include "bitpack.h"

static void write_compressed_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);

static void read_compressed_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);

void write_compressed(struct packed_arr *image)
{
	printf("COMP40 Compressed image format 2\n%u %u",
		2 * image->methods->width(image->pixels), 
		2 * image->methods->height(image->pixels));
	printf("\n");

	image->methods->map_row_major(image->pixels, write_compressed_apply, NULL);

	image->methods->free(&(image->pixels));
	free(image);
}

static void write_compressed_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void)array2;
	(void)cl;
	(void)col;
	(void)row;

	uint64_t value = *(uint64_t *)elem;

	putchar(Bitpack_getu(value, 8, 24));
	putchar(Bitpack_getu(value, 8, 16));
	putchar(Bitpack_getu(value, 8, 8));
	putchar(Bitpack_getu(value, 8, 0));
}

struct packed_arr *read_compressed(FILE *fp)
{
	struct packed_arr *compressed_img = malloc(sizeof(struct packed_arr));

	unsigned height, width;
	int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width, &height);
	assert(read == 2);
	int c = getc(fp);
	assert(c == '\n');

	A2Methods_T methods = uarray2_methods_plain;
	compressed_img->methods = methods;

	compressed_img->pixels = compressed_img->methods->new(width / 2, height / 2, sizeof(uint64_t));

	compressed_img->methods->map_row_major(compressed_img->pixels,
		read_compressed_apply, fp);

	return compressed_img;
}

static void read_compressed_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void)array2;
	(void)col;
	(void)row;

	FILE *fp = cl;
	uint64_t *int_ptr = elem;

	uint64_t word = 0;

	word = Bitpack_newu(word, 8, 24, getc(fp));
	word = Bitpack_newu(word, 8, 16, getc(fp));
	word = Bitpack_newu(word, 8, 8, getc(fp));
	word = Bitpack_newu(word, 8, 0, getc(fp));

	*int_ptr = word;
}

