/*
 * read_write_ppm.h
 *
 */

#include <stdlib.h>
#include <uarray2.h>
#include <pnm.h>

#ifndef READ_WRITE_PPM_H
#define READ_WRITE_PPM_H

Pnm_ppm read_ppm(FILE* fp);
void write_ppm(FILE* fp, Pnm_ppm image);

#endif
