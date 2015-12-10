#include "CppFunctorClosure.h"

/* From original author (Mike Perry):
 *
 * > This is where the magic happens. The code of this function is actually
 * > copied in gen_closure, which looks for the MAX_INT marker to replace it in
 * > malloc'd copies with 'this'.
 * >
 * > (Yes, I am a black wizard with no soul to speak of ;)
 */
namespace cpp_functor_closure {

void closure() {
    Functor *thisptr = (Functor *)MAX_INT;
    (*thisptr)();
}

}  // namespace cpp_functor_closure
