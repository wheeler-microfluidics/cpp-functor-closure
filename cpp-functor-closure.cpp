#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "CppFunctorClosure.h"

using namespace std;

class Functor : public cpp_functor_closure::Functor {
public:
    int a_;
    int b_;

    Functor(int a, int b) : a_(a), b_(b) { }

    // Need virtual so there is no relative call or g++ thunk BS
    virtual void operator()(void) {
        cerr << "A: " << a_ << ", B: " << b_ << endl;
    }
};


int main(int argc, char **argv) {
    Functor a(2,3);
    Functor *b = &a;

    cpp_functor_closure::fptr_t ptr =
        cpp_functor_closure::generate_closure(a);

    a();
    (*b)();

    a.a_ = 42;
    a.b_ = 23;

    ptr();

    cpp_functor_closure::aligned_free((void *)ptr);
}
