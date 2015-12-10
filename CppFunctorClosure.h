#ifndef ___CPP_FUNCTOR_CLOSURE__H___
#define ___CPP_FUNCTOR_CLOSURE__H___

/* Adapted from code [here][1].  See referral to solution on
 * [stackoverflow.com][2] [here][3].
 *
 * There was no license assigned to the original code and no copyright notice,
 * but [fscked.org][4] seems to be the website of someone named Mike Perry.
 *
 * [1]: http://fscked.org/projects/minihax/c-functor-c-function-pointer-conversion
 * [2]: http://stackoverflow.com
 * [3]: http://stackoverflow.com/questions/1840029/passing-functor-as-function-pointer#1840080
 *
 * Notes from original author:
 *
 * > Cross platform Functor to Function pointer conversion.
 * >
 * > Works unmodified on:
 * >  - CentOS 4.x 32 or 64bit (Even w/ NX support)
 * >  - Fedora (ExecShield and/or grsec segment-based proection)
 * >  - Free,NetBSD 32bit
 * >  - MacOS intel
 * > Broken Under:
 * >  - Grsec boxes with real NX support
 * > Minor issues:
 * >  - 64bit g++ requires at least -O1 to use 64bit immediates
 * >    (required for this to work)
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "aligned_alloc.h"

namespace cpp_functor_closure {

using namespace std;

typedef unsigned long ptr_size_t;
// -1 is sign extended on x64. Need unsigned max
static const ptr_size_t MAX_INT = (((ptr_size_t)-1) >> 1);
static const ptr_size_t MAX_FCN_SIZE = 128;

// This unfortunately must be global static so that Win32 doesn't use the IAT.
void closure();

typedef void (*fptr_t)();

template <typename F>
inline fptr_t generate_closure(F &functor) {
    /* Allocate block of memory large enough to hold the compiled code of the
     * `closure()` function.
     * Align to 16-byte boundary.  This is necessary for CENT OS (and maybe in
     * other cases).
     *
     * __Note__ that the aligned allocation requires `aligned_free` instead of
     * `free`. */
    uint8_t *start_ptr = (uint8_t*)aligned_malloc(16, MAX_FCN_SIZE);

    // Copy compiled `closure` function code to `start_ptr`.
    memcpy(start_ptr, (void*)closure, MAX_FCN_SIZE);

    /* Scan copied `closure` machine code and replace `MAX_INT` marker with
     * pointer to functor. */
    for (uint8_t *end_ptr = start_ptr; end_ptr - start_ptr < MAX_FCN_SIZE;
         end_ptr++) {
        if(*(F**)end_ptr == (F*)MAX_INT) {
            // `MAX_INT` was found, so replace it with the functor pointer.
            *(F**)end_ptr = &functor;
        }
    }

    /* **N.B.,** This aligned pointer requires `aligned_free` instead of
     * `free`. */
    return (fptr_t)start_ptr;
}


class Functor {
public:
    // Need virtual so there is no relative call or g++ thunk BS
    virtual void operator()(void) = 0;
};

}  // namespace cpp_functor_closure

#endif  // #ifndef ___CPP_FUNCTOR_CLOSURE__H___
