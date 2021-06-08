/*
 * read_write_ppm.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "read_write_ppm.h"
#include <pnm.h>
#include <a2methods.h>
#include "a2plain.h"
#include <uarray2.h>
#include <stdbool.h>

static Pnm_ppm trim_edges(Pnm_ppm image, Pnm_ppm new_image, int new_width, int new_height);
static void apply_trim(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl);

Pnm_ppm read_ppm(FILE* fp)
{
	A2Methods_T methods = uarray2_methods_plain;
	assert(methods);

	Pnm_ppm image = Pnm_ppmread(fp, methods);
	Pnm_ppm new_image = malloc(sizeof(struct Pnm_ppm));
	int new_width, new_height;

	if (image->width % 2 != 0 && image->height % 2 != 0) {

		new_width = image->width - 1;
		new_height = image->height - 1;

		new_image = trim_edges(image, new_image, new_width, new_height);

	} else if (image->width % 2 != 0 && image->height % 2 == 0) {

		new_width = image->width - 1;
		new_height = image->height;

		new_image = trim_edges(image, new_image, new_width, new_height);

	} else if (image->width % 2 == 0 && image->height % 2 != 0) {

		new_width = image->width;
		new_height = image->height - 1;

		new_image = trim_edges(image, new_image, new_width, new_height);

	} else {
		free(new_image);
		return image;
	}

	Pnm_ppmfree(&image);
	return new_image;
}

static Pnm_ppm trim_edges(Pnm_ppm image, Pnm_ppm new_image, int new_width, int new_height)
{
	new_image->width = new_width;
	new_image->height = new_height;
	new_image->denominator = image->denominator;
	new_image->pixels = image->methods->new(new_width, new_height, sizeof(struct Pnm_rgb));
	new_image->methods = image->methods;

	new_image->methods->map_row_major(new_image->pixels, apply_trim, image);

	return new_image;
}

static void apply_trim(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl)
{
	(void)elem;
	Pnm_ppm old_image = cl;

	*(struct Pnm_rgb *)(UArray2_at(array2, col, row)) = 
	*(struct Pnm_rgb *)(old_image->methods->at(old_image->pixels, col, row));
}

void write_ppm(FILE* fp, Pnm_ppm image)
{
	Pnm_ppmwrite(fp, image);
}