#include "stdio.h"

#include "common.h"
#include "debug.h"
#include "vm.h"

// Author's note: global VM is *probably* not good in production,
// but makes the implementation a bit simpler to follow.
VM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

// NOTE: RETURN POINTS AT NEXT AVAILABLE ITEM
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

// EXECUTOR
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

// TIL: macros are pure text substitution done by compiler's preprocessor
// This do-while is a trick to ensure the series of statements get completed regardless of how "parent" code is structured.
// I.e it's possible `pop(); pop();` would run the first statement in one branch and the second in another.
#define BINARY_OP(op) \
  do { \
    double b = pop(); \
    double a = pop(); \
    push(a op b); \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
  printf("       ");
  for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
    printf("[ ");
    printValue(*slot);
    printf(" ]");
  }
  printf("\n");
  disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD: BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break; 
      case OP_MULTIPLY: BINARY_OP(*); break; 
      case OP_DIVIDE: BINARY_OP(/); break;
      case OP_NEGATE: push(-pop()); break;
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}

