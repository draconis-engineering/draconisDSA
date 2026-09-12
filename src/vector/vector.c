/* VECTOR LIBRARY - DURENDAL ENGINEERING - V.1 */

/*
NOTE - This is not much more than a simple wrapper around the duraC::Array struct.
The vector class' only difference is compatability with the duraC::Matrix struct.
*/

#include <stdio.h>
#include <stdint.h>
#include "vector.h"

void init_vector(Vector *a) {

}

int16_t dim(Vector *a) {
    return a->elem_count;
}

float get_element(Vector *a, int idx) {
    if (idx > dim(a) || idx < 0) {
        printf("ERROR: Invalid index for vector!");
        return NULL;
    }
    return a->elements[idx];
}

float dot_product(Vector *a, Vector *b) {
    if (dim(a) != dim(b)) {
        printf("ERROR: Can't calculate dot product of two different-length vectors.");
    }

    float sum = 0.0f;

    for (int idx = 0; idx <= dim(a); idx++) {
        sum = sum + get_element(a, idx) * get_element(b, idx);
    }

    return sum;
}
