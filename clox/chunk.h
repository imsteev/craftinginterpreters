#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// An OpCode defines an instruction.
// Each instruction has a specified format (e.g, constants must need relevant data to load correct values)
typedef enum {
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines; // source code line number.
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk, Value value);

#endif
