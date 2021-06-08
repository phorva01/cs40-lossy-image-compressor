/* Ruthie Block (rblock02) and Grace Fagan (gfagan01)
 * Comp40 Spring 2019
 * Homework 3: Locality
 *
 * a2plain.c (A2 Methods struct containing methods than can interact with a 
 * UArray2 object)
 */

#include <string.h>

#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2;

struct a2fun_closure {
        A2Methods_smallapplyfun *apply;
        void *cl;
        A2 a2;
};

static A2 new(int width, int height, int size){

        return UArray2_new(width, height, size);
}

static A2 new_with_blocksize(int width, int height, int size, int blocksize){

        (void) blocksize;
        return UArray2_new(width, height, size);
}

static void a2free(A2 * array2p){

        UArray2_free((UArray2_T *) array2p);

}

static int width(A2 array2){

        return UArray2_width(array2);

}

static int height(A2 array2){

        return UArray2_height(array2);

}

static int size(A2 array2){

        return UArray2_size(array2);

}

static int blocksize(A2 array2){

        (void) array2;
        return 0;

}

static A2Methods_Object *at(A2 array2, int i, int j){

        return UArray2_at(array2, i, j);

}

typedef void applyfun(int i, int j, UArray2_T array2, void *elem, void *cl);

static void map_row_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_row_major(array2, (applyfun *) apply, cl);
}

static void map_col_major(A2 array2, A2Methods_applyfun apply, void *cl)
{
        UArray2_map_col_major(array2, (applyfun *) apply, cl);
}

static void apply_small(int i, int j, UArray2_T array2, void *elem, void *vcl)
{
        struct a2fun_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)array2;
        cl->apply(elem, cl->cl); //outer apply func
}

static void small_map_row_major(A2 a2, A2Methods_smallapplyfun apply,
        void *cl)
{
        struct a2fun_closure mycl = { apply, cl, a2 };
        UArray2_map_row_major(a2, apply_small, &mycl);

}

static void small_map_col_major(A2 a2, A2Methods_smallapplyfun apply,
        void *cl)
{
        struct a2fun_closure mycl = { apply, cl, a2};
        UArray2_map_col_major(a2, apply_small, &mycl);
        
}

static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,
        map_col_major,                  
        NULL,
        map_row_major,                  //map_default
        small_map_row_major,    
        small_map_col_major,    
        NULL,
        small_map_row_major,    // small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
