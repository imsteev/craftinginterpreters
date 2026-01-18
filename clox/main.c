#include "common.h"
#include "chunk.h"
#include "debug.h"

// clox
// front-end: compiler
// representation: bytecode
// execution: virtual machine (VM)

int main(int argc, const char *argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  writeChunk(&chunk, OP_RETURN);
  freeChunk(&chunk);
  return 0;
}
