/*
 * read_write_compressed.h
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "pack_unpack.h"

#ifndef READ_WRITE_COMPRESSED_H
#define READ_WRITE_COMPRESSED_H

void write_compressed(struct packed_arr *image);
struct packed_arr *read_compressed(FILE *fp);

#endif
