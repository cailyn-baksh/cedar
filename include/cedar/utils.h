#ifndef _LIBCEDAR_UTILS_H_
#define _LIBCEDAR_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*
 * A variable length array.
 *
 * _elem_size	The size of the array's elements' type.
 * _alloc		The number of elements allocated
 * length		The length of the array
 * items		The items in the array.
 */
struct CdrVector {
	size_t _elem_size;
	size_t _alloc;
	size_t length;
	void **items;
};

/*
 * Initializes a new vector
 *
 * type			The type of the vector's elements
 * Returns a new vector.
 */
#define cdr_VecInit(type) _cdr_VecInit(sizeof(type))
static inline CdrVector _cdr_VecInit(size_t elem_size) {
	return (CdrVector){
		._elem_size = elem_size,
		._alloc = 1,
		.length = 0,
		.items = malloc(elem_size)
	};
}

/*
 * Reserve space in the vector. This can be used to either grow or shrink the
 * vector.
 *
 * vec			The vector to reserve space in.
 * size			The new size of the vector. This affects _alloc, not length.
 * 				If size is less than vec.length, then the vector is shrunk to
 * 				the minimum size needed to fit all the items. The vector cannot
 * 				be resized to smaller than 1 element.
 */
void cdr_VecReserve(CdrVector *vec, size_t size);

/* Reserve the minimum space in the vector */
#define CDR_VECTOR_RESERVE_MIN 0

/*
 * Push a new item onto the end of the vector. This function takes two
 * arguments: the first one is named (vec), and the second is a variadic
 * argument (here called 'item').
 *
 * vec			The vector to push onto
 * item			The item to push onto the vector
 */
void cdr_VecPushBack(CdrVector *vec, ...);

/*
 * Insert a new item into the vector
 *
 * vec			The vector to insert into
 * index		The index to insert at
 * item			The item to insert into the vector
 */
void cdr_VecInsert(CdrVector *vec, size_t index, ...);

/*
 * Gets an item from the vector
 *
 * vec			The vector to get the item from
 * index		The index of the item to get
 * Returns a pointer to the item at the specified index.
 */
void **cdr_VecGet(CdrVector *vec, size_t index);

/*
 * Removes an item from the vector
 *
 * vec			The vector to remove the item from
 * index		The index of the item to remove
 */
void cdr_VecRemove(CdrVector *vec, size_t index);

/*
 * Cleans up a vector's memory.
 *
 * vec			The vector to free.
 */
void cdr_VecFree(CdrVector *vec);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_UTILS_H_

