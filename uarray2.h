/*
 * uarray2.h
 * 
 * Written by Grace Fagan (gfagan01) and Peter Horvath (phorva01)
 * COMP 40 Spring 2018
 * Interface for the unboxed 2d array, an extension of Hanson and Ramsey's
 * UArray, an unboxed array
 */

#ifndef UARRAY2_H
#define UARRAY2_H

#define T UArray2_T
typedef struct T *T;

/* UArray2_new()
 * Allocates memory for a new two-dimensional array of a given width and height
 * and elements of given size
 * Expectations: the given width, height and size are positive integer values,
 * memory is successfully allocated
 */
T UArray2_new(int width, int height, int size);

/* UArray2_free()
 * Frees all memory associated with the array and its elements
 * Expectations: uarray2, *uarray2 exist, free() works properly on its elements
 */
void UArray2_free(T *uarray2);

/* UArray2_map_row_major()
 * Applies the given function to each element in the given array using the row
 * major traversal method
 * Expectations: uarray2 exists, apply function is declared and defined, *apply
 * exists and is non-null, and *cl exists (can be null)
 */
void UArray2_map_row_major(T uarray2, void apply(int i, int j, 
        UArray2_T uarray2, void *p1, void *p2), void *cl);

/* UArray2_map_col_major()
 * Applies the given function to each element in the given array using the col
 * major traversal method
 * Expectations: uarray2 exists, apply function is declared and defined, *apply
 * exists and is non-null, and *cl exists (can be null)
 */
void UArray2_map_col_major(T uarray2, void apply(int i, int j, 
        UArray2_T uarray2, void *p1, void *p2), void *cl);

/* UArray2_width()
 * Returns the width of uarray2
 * Expectations: uarray2 exists
 */
int UArray2_width(T uarray2);

/* UArray2_height()
 * Returns the height of uarray2
 * Expectations: uarray2 exists
 */
int UArray2_height(T uarray2);

/* UArray2_size()
 * Returns the size in memory of each element of uarray2
 * Expectations: uarray2 exists, the elements are of uniform size in memory
 */
int UArray2_size(T uarray2);

/* UArray2_at()
 * Returns a pointer to the element in uarray2 at the given row and column
 * index. Returns NULL if there is no value stored at the row and column.
 * Expectations: uarray2 exists, the row and col are valid indices
 */
void *UArray2_at(T uarray2, int col, int row);

#undef T
#endif
