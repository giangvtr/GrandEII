//
// Link-time wrappers for malloc/calloc/realloc, see mock_alloc.h.
//

#include <stdlib.h>
#include "mock_alloc.h"

void *__real_malloc(size_t size);
void *__real_calloc(size_t nmemb, size_t size);
void *__real_realloc(void *ptr, size_t size);

// Number of allocations to let through before failing one; -1 means disarmed
static int calls_before_failure = -1;

void mock_alloc_fail_after(int n) {
    calls_before_failure = n;
}

void mock_alloc_reset(void) {
    calls_before_failure = -1;
}

// Returns 1 if this allocation must fail
static int should_fail(void) {
    if (calls_before_failure < 0) return 0;
    if (calls_before_failure == 0) {
        calls_before_failure = -1;
        return 1;
    }
    calls_before_failure--;
    return 0;
}

void *__wrap_malloc(size_t size) {
    if (should_fail()) return NULL;
    return __real_malloc(size);
}

void *__wrap_calloc(size_t nmemb, size_t size) {
    if (should_fail()) return NULL;
    return __real_calloc(nmemb, size);
}

void *__wrap_realloc(void *ptr, size_t size) {
    if (should_fail()) return NULL;
    return __real_realloc(ptr, size);
}
