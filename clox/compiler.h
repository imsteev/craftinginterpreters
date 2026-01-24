#ifndef clox_compiler_h
#define clox_compiler_h

#include "vm.h"

// source code in, bytecode out.
// false if compiler error.
bool compile(const char* source, Chunk* chunk);

#endif