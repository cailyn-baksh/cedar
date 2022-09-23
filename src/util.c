#include <stdint.h>
#include <stdlib.h>

#include <cedar.h>
#include <cedar/utils.h>

// Subscript a vector
#define VecIndex(vec, index) (*(vec->items + ((index) * vec->_elem_size)))

void cdr_VecReserve(CdrVector *vec, size_t size) {
	// Ensure size is valid
	if (size < vec->length) {
		size = vec->length;
	} else if (size < 1) {
		size = 1;
	}

	vec->_alloc = size;
	vec->items = realloc(vec->items, vec->_alloc * vec->_elem_size);
}

void cdr_VecPushBack(CdrVector *vec, ...) {
	vec->length += 1;

	// Resize if necessary
	if (vec->_alloc < vec->length) {
		vec->_alloc *= 2;
		vec->items = realloc(vec->items, vec->_alloc * vec->_elem_size);
	}

	// Get item
	va_list args;
	va_start(args, vec);
	uint8_t item[vec->_elem_size] = va_arg(args, uint8_t[vec->_elem_size]);

	// Store item in vector
	memcpy(&VecIndex(vec, vec->length - 1), item, vec->_elem_size);

	va_end(args);
}

void cdr_VecInsert(CdrVector *vec, size_t index, ...) {
	vec->length += 1;

	// Resize if necessary
	if (vec->_alloc < vec->length) {
		vec->_alloc *= 2;
		vec->items = realloc(vec->items, vec->_alloc * vec->_elem_size);
	}

	// Shift vec[index:] over
	// start from vec->length-1 since we havent added the last item yet
	for (size_t i=vec->length-1; i >= index; --i) {
		// vec->items[i+1] = vec->items[i];
		VecIndex(vec, i+1) = VecIndex(vec, i);
	}

	// Get item
	va_list args;
	va_start(args, index);
	uint8_t item[vec->_elem_size] = va_arg(args, uint8_t[vec->_elem_size]);

	// Store item in vector
	memcpy(&VecIndex(vec, index), item, vec->_elem_size);

	va_end(args);
}

void **cdr_VecGet(CdrVector *vec, size_t index) {
	return &VecIndex(vec, index);
}

void cdr

