/* 
 *
 * rgb_compvid.h
 *
 */

#include <stdlib.h>
#include <pnm.h>

#ifndef RGB_COMPVID_H
#define RGB_COMPVID_H

struct comp_vid_arr {
	unsigned width, height;
	A2Methods_UArray2 pixels;
	const struct A2Methods_T *methods;
};

struct comp_vid_pixel {
	float y, pb, pr;
};

struct comp_vid_arr *rgb_to_compvid(Pnm_ppm image);
Pnm_ppm compvid_to_rgb(struct comp_vid_arr *comp_image);

#endif
