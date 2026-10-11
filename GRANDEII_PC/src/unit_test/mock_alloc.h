//
// Allocation failure injection for unit tests.
// malloc/calloc/realloc are wrapped at link time (-Wl,--wrap=...), see CMakeLists.txt.
//

#ifndef MOCK_ALLOC_H
#define MOCK_ALLOC_H

/**
 * @brief Makes one upcoming allocation fail.
 * The (n+1)-th malloc/calloc/realloc call after this one returns NULL, then the
 * mock disarms itself. fail_after(0) fails the very next allocation.
 */
void mock_alloc_fail_after(int n);

/** @brief Disarms the mock; allocations go back to the real functions. */
void mock_alloc_reset(void);

#endif //MOCK_ALLOC_H
