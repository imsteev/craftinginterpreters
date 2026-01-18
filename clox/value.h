#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

// ValueArray is used to store constants
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

void initValueArray(ValueArray* arr);
void writeValueArray(ValueArray* arr, Value v);
void freeValueArray(ValueArray* arr);
void printValue(Value v);

#endif
