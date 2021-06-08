/*
 * compress40.c
 * Implementation of the compress40 interface provided to us
 * Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 * COMP40 Spring 2019 - arith
 */

#include "compress40.h"
#include "read_write_ppm.h"
#include "rgb_compvid.h"
#include "DCT_and_chroma.h"
#include "pack_unpack.h"
#include "read_write_compressed.h"
#include <bitpack.h>
#include <stdio.h>
#include <stdlib.h>
#include <pnm.h>

/* each line below corresponds to a process in the compression method described
 * in the arith specification */
void compress40(FILE* fp)
{
	assert(fp != NULL);

	Pnm_ppm image = read_ppm(fp);

	struct comp_vid_arr *new_img = rgb_to_compvid(image);

	struct brights_chroma_arr *fat_img = DCT_compress(new_img);

	struct packed_arr *packed_img = pack_bits(fat_img);

	write_compressed(packed_img);
}

/* each line below corresponds to a process in the decompression method
 * described in the arith specification */
void decompress40(FILE* fp)
{
	struct packed_arr *compressed_img = read_compressed(fp);

	struct brights_chroma_arr *unpacked_img = unpack_bits(compressed_img);

	struct comp_vid_arr *skinny_img = DCT_decompress(unpacked_img);

	Pnm_ppm final_image = compvid_to_rgb(skinny_img);

	write_ppm(stdout, final_image);

	Pnm_ppmfree(&final_image);
}
