/*
 * DCT_and_chroma.c
 *
 */

#include "DCT_and_chroma.h"
#include "rgb_compvid.h"
#include <bitpack.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <a2methods.h>
#include <arith40.h>

static void compvid_to_DCT_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);
static void DCT_to_compvid_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl);
static int bcd_quantize(float value);

struct brights_chroma_arr *DCT_compress(struct comp_vid_arr *image)
{
	struct brights_chroma_arr *new_image = malloc(sizeof(struct 
		brights_chroma_arr));

	new_image->width = image->width / 2;
	new_image->height = image->height / 2;
	new_image->pixels = image->methods->new(new_image->width, 
		new_image->height, sizeof(struct brights_chroma_pix));
	new_image->methods = image->methods;

	new_image->methods->map_row_major(new_image->pixels, 
		compvid_to_DCT_apply, image);

	image->methods->free(&(image->pixels));
	free(image);

	return new_image;
}

static void compvid_to_DCT_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void)(array2);

	struct comp_vid_arr *image = cl;
	struct brights_chroma_pix *pixel = elem;

	struct comp_vid_pixel *P1 = image->methods->at(image->pixels, col * 2, 
		row * 2);
	struct comp_vid_pixel *P2 = image->methods->at(image->pixels, (col * 2) + 
		1, (row * 2));
	struct comp_vid_pixel *P3 = image->methods->at(image->pixels, col * 2, 
		(row * 2) + 1);
	struct comp_vid_pixel *P4 = image->methods->at(image->pixels, (col * 2) +
		1, (row * 2) + 1);

	float Da = (float)(P4->y + P3->y + P2->y + P1->y) / 4.0;
	float Db = (float)(P4->y + P3->y - P2->y - P1->y) / 4.0;
	float Dc = (float)(P4->y - P3->y + P2->y - P1->y) / 4.0;
	float Dd = (float)(P4->y - P3->y - P2->y + P1->y) / 4.0;

	//fprintf(stderr, "just converted to float: %f, %f, %f, %f\n", Da, Db, Dc, Dd);

	pixel->a = (unsigned)(Da * 511);

	// fprintf(stderr, "preQ value: %f\n", Db);

	pixel->b = bcd_quantize(Db);

	// fprintf(stderr, "postQ value: %d\n", pixel->b);

	pixel->c = bcd_quantize(Dc);
	pixel->d = bcd_quantize(Dd);

	// fprintf(stderr, "just quantized: %u, %u, %u, %u\n", pixel->a, pixel->b, pixel->c, pixel->d);

	float avg_pb = (float)(P1->pb + P2->pb + P3->pb + P4->pb) / 4.0;
	float avg_pr = (float)(P1->pr + P2->pr + P3->pr + P4->pr) / 4.0;

	//fprintf(stderr, "pb, pr values: %f, %f\n", P1->pb, P1->pr);
	// fprintf(stderr, "avergae pb, pr values: %f, %f\n", avg_pb, avg_pr);

	pixel->pb = Arith40_index_of_chroma(avg_pb);
	pixel->pr = Arith40_index_of_chroma(avg_pr);
}

static int bcd_quantize(float value)
{	
	int quant_value = roundf(value * 50);

	if (quant_value < -15){
		quant_value = -15;
	} else if (quant_value > 15){
		quant_value = 15;
	}

	return quant_value;
} 

struct comp_vid_arr *DCT_decompress(struct 
	brights_chroma_arr *img_to_decompress)
{
	struct comp_vid_arr *compvid_image = malloc(sizeof(struct comp_vid_arr));

	compvid_image->width = img_to_decompress->width * 2;
	compvid_image->height = img_to_decompress->height * 2;

	compvid_image->pixels = img_to_decompress->methods->new(
		compvid_image->width, compvid_image->height, sizeof(struct comp_vid_pixel));

	compvid_image->methods = img_to_decompress->methods;

	img_to_decompress->methods->map_row_major(img_to_decompress->pixels, 
		DCT_to_compvid_apply, compvid_image);

	img_to_decompress->methods->free(&(img_to_decompress->pixels));
	free(img_to_decompress);

	return compvid_image;
}

static void DCT_to_compvid_apply(int col, int row, A2Methods_UArray2 array2,
	void *elem, void *cl)
{
	(void)array2;

	struct brights_chroma_pix *old_pix = elem;
	struct comp_vid_arr *comp_vid_img = cl;

	struct comp_vid_pixel *P1 = malloc(sizeof(struct comp_vid_pixel));
	struct comp_vid_pixel *P2 = malloc(sizeof(struct comp_vid_pixel));
	struct comp_vid_pixel *P3 = malloc(sizeof(struct comp_vid_pixel));
	struct comp_vid_pixel *P4 = malloc(sizeof(struct comp_vid_pixel));

	float a, b, c, d;

	a = (float)old_pix->a / 511.0;
	b = ((float)old_pix->b) / 50.0;
	c = ((float)old_pix->c) / 50.0;
	d = ((float)old_pix->d) / 50.0;

	// fprintf(stderr, "decompressed values: %f, %f, %f, %f\n", a, b, c, d);

	P1->y = (a - b - c + d);
	P2->y = (a - b + c - d);
	P3->y = (a + b - c - d);
	P4->y = (a + b + c + d);

	P1->pb = Arith40_chroma_of_index(old_pix->pb);
	P2->pb = Arith40_chroma_of_index(old_pix->pb);
	P3->pb = Arith40_chroma_of_index(old_pix->pb);
	P4->pb = Arith40_chroma_of_index(old_pix->pb);

	P1->pr = Arith40_chroma_of_index(old_pix->pr);
	P2->pr = Arith40_chroma_of_index(old_pix->pr);
	P3->pr = Arith40_chroma_of_index(old_pix->pr);
	P4->pr = Arith40_chroma_of_index(old_pix->pr);

	*(struct comp_vid_pixel *)(comp_vid_img->methods->at(comp_vid_img->pixels, col * 2,
		row * 2)) = *P1;
	*(struct comp_vid_pixel *)(comp_vid_img->methods->at(comp_vid_img->pixels, col * 2 + 1,
		row * 2)) = *P2;
	*(struct comp_vid_pixel *)(comp_vid_img->methods->at(comp_vid_img->pixels, col * 2,
		row * 2 + 1)) = *P3;
	*(struct comp_vid_pixel *)(comp_vid_img->methods->at(comp_vid_img->pixels, col * 2 + 1,
		row * 2 + 1)) = *P4;

	free(P1);
	free(P2);
	free(P3);
	free(P4);
}