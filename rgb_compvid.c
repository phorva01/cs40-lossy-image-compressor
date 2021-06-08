/* 
 * rgb_compvid.c
 *
 */

#include "rgb_compvid.h"
#include <stdlib.h>
#include <uarray2.h>
#include <a2methods.h>
#include <pnm.h>
#include <math.h>
#define RGB_DENOMINATOR 255

static void rgb_to_comp_apply(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl);
static void comp_to_rgb_apply(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl);
static float bound_values(float color);

// struct comp_vid_arr {
// 	unsigned width, height;
// 	A2Methods_UArray2 pixels;
// 	const struct A2Methods_T *methods;
// };

// struct comp_vid_pixel {
// 	float y, pb, pr;
// };

struct closure {
	struct comp_vid_arr *comp_struct;
	unsigned rgb_denom;
};

struct comp_vid_arr *rgb_to_compvid(Pnm_ppm image)
{
	struct comp_vid_arr *new_image = malloc(sizeof(struct comp_vid_arr));

	new_image->width = image->width;
	new_image->height = image->height;
	new_image->pixels = image->methods->new(image->width, image->height,
		sizeof(struct comp_vid_pixel));
	new_image->methods = image->methods;

	new_image->methods->at(new_image->pixels, 0, 0);

	struct closure *cl = malloc(sizeof(struct closure));
	cl->comp_struct = new_image;
	cl->rgb_denom = image->denominator;

	image->methods->map_row_major(image->pixels, rgb_to_comp_apply, cl);

	free(cl);
	Pnm_ppmfree(&image);

	return new_image;
}

/* apply function */
static void rgb_to_comp_apply(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl)
{
	(void)array2;

	Pnm_rgb rgb = elem;
	struct closure *our_cl = cl;

	struct comp_vid_pixel *pixel = malloc(sizeof(struct comp_vid_pixel));

	float r = (float) rgb->red / (float) our_cl->rgb_denom;
	float g = (float) rgb->green / (float) our_cl->rgb_denom;
	float b = (float) rgb->blue / (float) our_cl->rgb_denom;

	pixel->y = 0.299 * r + 0.587 * g + 0.114 * b;
	pixel->pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
	pixel->pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

	//fprintf(stderr, "inital struct values: %f, %f\n", pixel->pb, pixel->pr);

	*(struct comp_vid_pixel *)our_cl->comp_struct->methods->at(our_cl->comp_struct->pixels,
		col, row) = *pixel;

	free(pixel);
}

Pnm_ppm compvid_to_rgb(struct comp_vid_arr *comp_image)
{
	Pnm_ppm rgb_image = malloc(sizeof(struct Pnm_ppm));

	rgb_image->width = comp_image->width;
	rgb_image->height = comp_image->height;
	rgb_image->denominator = RGB_DENOMINATOR;
	rgb_image->pixels = comp_image->methods->new(comp_image->width,
		comp_image->height, sizeof(struct Pnm_rgb));
	rgb_image->methods = comp_image->methods;

	comp_image->methods->map_row_major(comp_image->pixels, comp_to_rgb_apply, rgb_image);

	comp_image->methods->free(&(comp_image->pixels));
	free(comp_image);

	return rgb_image;
}

/* apply function */
static void comp_to_rgb_apply(int col, int row, A2Methods_UArray2 array2, void *elem, void *cl)
{
	(void)(array2);
	struct comp_vid_pixel *comp_vid = elem;
	Pnm_ppm rgb_image = cl;

	Pnm_rgb rgb = malloc(sizeof(struct Pnm_rgb));

	float red = roundf((1.0 * comp_vid->y + 0.0 * comp_vid->pb + 1.402 * comp_vid->pr) * RGB_DENOMINATOR);
	float green = roundf((1.0 * comp_vid->y - 0.344136 * comp_vid->pb - 0.714136 * comp_vid->pr) * RGB_DENOMINATOR);
	float blue = roundf((1.0 * comp_vid->y + 1.772 * comp_vid->pb + 0.0 * comp_vid->pr) * RGB_DENOMINATOR);

	red = bound_values(red);
	green = bound_values(green);
	blue = bound_values(blue);

	rgb->red = (unsigned)(red);
	rgb->green = (unsigned)(green);
	rgb->blue = (unsigned)(blue);

	*(struct Pnm_rgb *)(rgb_image->methods->at(rgb_image->pixels, col, row)) = *rgb;

	free(rgb);
}

static float bound_values(float color)
{
	if (color < 0){
		color = 0;
	} else if (color > RGB_DENOMINATOR){
		color = RGB_DENOMINATOR;
	}

	return color;
}