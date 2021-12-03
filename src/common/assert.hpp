#pragma once
#include "terminal.hpp"
#include <cassert>

#define Assert(x) assert(x) // NOLINT

#ifdef __clang__
#    define WARNING_SUPPRESS_UNUSED_FUNCTION() _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#else
#    define WARNING_SUPPRESS_UNUSED_FUNCTION()
#endif
