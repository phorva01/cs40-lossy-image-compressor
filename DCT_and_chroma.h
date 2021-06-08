/*
 * DCT_and_chroma.h
 * Interface to perform/reverse the Discrete Cosine Transformation on the Y 
 * values and the quantization of the Pb and Pr values of each 2-by-2 group of
 * pixels in the image.
 * Peter Horvath (phorva01) and Grace Fagan (gfagan01)
 * COMP40 Spring 2019 - arith
 */

#include <stdlib.h>
#include "rgb_compvid.h"

#ifndef DCT_AND_CHROMA_H
#define DCT_AND_CHROMA_H

/* a, b, c, and d are the Y values after DCT, and Pb and Pr are the 
 * chroma values */
struct brights_chroma_pix {
	unsigned a, pb, pr;
	int b, c, d;
};

/* The array to hold the brights/chroma pixels. This array is half the width
 * and height of the component video array, as each 2-by-2 subimage of pixels
 * in the original image is represented by one pixel in this array. */
struct brights_chroma_arr {
	unsigned width, height;
	A2Methods_UArray2 pixels;
	const struct A2Methods_T *methods;
};

/* DCT_compress
 * Input: the image as an array of component video pixels
 * Output: the image as an array of brights/chroma pixels
 * Does: converts the component video array into a brights/chroma array in
 *       which each pixel represents a square of 4 pixels in the original image
 */
struct brights_chroma_arr *DCT_compress(struct comp_vid_arr *img_to_compress);

/* DCT_decompress
 * Input: the image as an array of brights/chroma pixels
 * Output: the image as an array of component video pixels
 * Does: converts the bright/chroma array into a component video array in
 *       which each subimage of 4 pixels represents one pixel from the
 *       compressed image*/
struct comp_vid_arr *DCT_decompress(
	struct brights_chroma_arr *img_to_decompress);

#endif
