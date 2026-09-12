/* Vector prototype file - DURENDAL ENGINEERING VECTOR LIBRARY - V.1*/

#include "../array/array.h"
#include <stdint.h>

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float* elements; // The Vectors elements
    int elem_count; // The element count
} Vector;

void init_vector(Vector *a);
int16_t dim(Vector *a);
float get_element(Vector *a, int idx);
float dot_product(Vector *a, Vector *b);

#endif
