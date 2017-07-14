![https://ci.appveyor.com/api/projects/status/github/wheeler-microfluidics/cpp-functor-closure?branch=master&svg=true](https://ci.appveyor.com/api/projects/status/github/wheeler-microfluidics/cpp-functor-closure?branch=master&svg=true)
# Cross platform C++ functor to C function pointer conversion #

This C++ library provides a mechanism to *dynamically* convert a C++ functor
instance into a C function pointer at run time.

## Motivation ##

C++ functors can be useful, for example, in cases where state must be
maintained between calls to a callback.  However, when interfacing with
existing code that uses C function pointers for handling callbacks, C++ functor
objects cannot be used directly.

This library makes it possible to generate a pointer to a C function wrapping
the provided C++ functor call operator method.

## How does it work? ##

A function (named `closure`) is compiled and the compiled machine code is used
at runtime as a template for a new wrapper function that wraps the C++ functor
instance.

The basic process is as follows:

 1. Allocate memory for wrapper function instructions.
 2. Copy compiled `closure` function to wrapper memory.
 3. Using search/replace, modify new copy of `closure` to call C++ functor call
    operator method.
 4. Return pointer to wrapper function (i.e., a C pointer).

## Usage ##

See `cpp-functor-closure.cpp` for example usage.  Note that the functor class
**must** inherit from the `cpp_functor_closure::Functor` base class.

## Credits ##

Adapted by Christian Fobel <christian@fobel.net> from code written by Mike
Perry (see [here][1]).  See referral to solution on [stackoverflow.com][2]
[here][3].

There was no license assigned to the original code and no copyright notice,
but [fscked.org][4] seems to be the website of someone named Mike Perry.

See `COPYING` for license terms of this library.

Notes from [original author][1]:

> Works unmodified on:
>  - CentOS 4.x 32 or 64bit (Even w/ NX support)
>  - Fedora (ExecShield and/or grsec segment-based proection)
>  - Free,NetBSD 32bit
>  - MacOS intel
> Broken Under:
>  - Grsec boxes with real NX support
> Minor issues:
>  - 64bit g++ requires at least -O1 to use 64bit immediates
>    (required for this to work)

[1]: http://fscked.org/projects/minihax/c-functor-c-function-pointer-conversion
[2]: http://stackoverflow.com
[3]: http://stackoverflow.com/questions/1840029/passing-functor-as-function-pointer#1840080
[4]: http://fscked.org
