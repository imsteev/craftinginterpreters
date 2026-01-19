#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static int DUMMY_LINE_NUM = 123;

// clox
// front-end: compiler
// representation: bytecode
// execution: virtual machine (VM)

int main(int argc, const char *argv[]) {

  initVM();

  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, DUMMY_LINE_NUM);
  writeChunk(&chunk, constant, DUMMY_LINE_NUM);

  constant = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, DUMMY_LINE_NUM);
  writeChunk(&chunk, constant, DUMMY_LINE_NUM);
  
  writeChunk(&chunk, OP_ADD, DUMMY_LINE_NUM);

  constant = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, DUMMY_LINE_NUM);
  writeChunk(&chunk, constant, DUMMY_LINE_NUM);

  writeChunk(&chunk, OP_DIVIDE, DUMMY_LINE_NUM);
  writeChunk(&chunk, OP_NEGATE, DUMMY_LINE_NUM);

  writeChunk(&chunk, OP_RETURN, DUMMY_LINE_NUM); // should print -0.821429

  disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}
