/*
 * ppmdiff.c
 * A program to calculate the difference between two ppm files.
 * COMP40 Spring 2019
 * Written by Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 */

#include <stdio.h>
#include <stdlib.h>
#include "pnm.h"
#include "a2plain.h"
#include <assert.h>
#include <string.h>
#include <math.h>

double compare_imgs(Pnm_ppm img1, Pnm_ppm img2, int small_width, 
	int small_height, A2Methods_T methods);
double pixel_diff(Pnm_ppm img1, Pnm_ppm im2, 
	int col, int row, A2Methods_T methods);

int main(int argc, char *argv[])
{

	if (argc != 3) {
		fprintf(stderr, "USAGE: [executable] [file1] [file2]\n");
		exit(EXIT_FAILURE);
	} else {
		FILE *file1;
		FILE *file2;

		if (strcmp(argv[1], "-") == 0) {
			file1 = stdin;
			if (strcmp(argv[2], "-") == 0) {
				fprintf(stderr, "ERROR: only one file may be standard" 
					" input\n");
				exit(EXIT_FAILURE);
			}
		} else {
			file1 = fopen(argv[1], "r");
			assert(file1 != NULL);
		}

		if (strcmp(argv[2], "-") == 0) {
			file2 = stdin;
		} else {
			file2 = fopen(argv[2], "r");
			assert(file2 != NULL);
		}

		A2Methods_T methods = uarray2_methods_plain;

		Pnm_ppm img1 = Pnm_ppmread(file1, methods);
		Pnm_ppm img2 = Pnm_ppmread(file2, methods);

		if (img1->height == 0 || img2->height == 0 || 
			img1->width == 0 || img2->width == 0) {
			fprintf(stderr, "ERROR: both ppm files must have non-zero" 
				"height and width\n");
			exit(EXIT_FAILURE);
		} else if (abs(img1->height - img2->height) > 1 ||
			abs(img1->width - img2->width) > 1) {
			fprintf(stderr, "ERROR: difference in heights or widths of images"
				"too large\n");
			printf("1.0\n");
		} else {
			int smaller_width, smaller_height;
			smaller_width = (img1->width <= img2->width) ? 
				img1->width : img2->width;
			smaller_height = (img1->height <= img2->height) ? 
				img1->height : img2->height;

			double E_diff = compare_imgs(img1, img2, smaller_width, smaller_height, methods);
			fprintf(stdout, "E: %f\n", E_diff);
		}
		Pnm_ppmfree(&img1);
		Pnm_ppmfree(&img2);

		fclose(file1);
		fclose(file2);
	}

	return EXIT_SUCCESS;
}

double compare_imgs(Pnm_ppm img1, Pnm_ppm img2, int small_width, 
	int small_height, A2Methods_T methods)
{
	double numerator_sum = 0.0;

	for (int row = 0; row < small_height; row++) {
		for (int col = 0; col < small_width; col++) {
			numerator_sum += pixel_diff(img1, img2, col, row, methods);
		}
	}

	double fraction = numerator_sum / (double) (3 * small_width * small_height);

	return sqrt(fraction);
}

double pixel_diff(Pnm_ppm img1, Pnm_ppm img2, int col, int row, A2Methods_T methods)
{
	double r1, r2, g1, g2, b1, b2;
	double r, g, b;

	r1 = (double) (((struct Pnm_rgb *)(methods->at(img1->pixels, col, row)))->red) / (double) img1->denominator;
	r2 = (double) (((struct Pnm_rgb *)(methods->at(img2->pixels, col, row)))->red) / (double) img2->denominator;

	g1 = (double) (((struct Pnm_rgb *)(methods->at(img1->pixels, col, row)))->green) / (double) img1->denominator;
	g2 = (double) (((struct Pnm_rgb *)(methods->at(img2->pixels, col, row)))->green) / (double) img2->denominator;

	b1 = (double) (((struct Pnm_rgb *)(methods->at(img1->pixels, col, row)))->blue) / (double) img1->denominator;
	b2 = (double) (((struct Pnm_rgb *)(methods->at(img2->pixels, col, row)))->blue) / (double) img2->denominator;

	r = (r1 - r2);
	g = (g1 - g2);
	b = (b1 - b2);

	return (r * r) + (g * g) + (b * b);
}
