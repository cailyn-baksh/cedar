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
	vec->items = realloc(vec->items, vec->_alloc);
}

void cdr_VecPushBack(CdrVector *vec, void *item) {
	vec->length += 1;

	// Resize if necessary
	if (vec->_alloc < vec->length) {
		vec->_alloc *= 2;
		vec->items = realloc(vec->items, vec->_alloc * vec->_elem_size);
	}

	// vec->items[vec->length - 1] = item;
	VecIndex(vec, vec->length - 1) = item;
}

void cdr_VecInsert(CdrVector *vec, size_t index, void *item) {
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

	// vec->items[index] = item;
	VecIndex(vec, index) = item;
}

void *cdr_VecGet(CdrVector *vec, size_t index) {
	return VecIndex(vec, index);
}

void cdr

