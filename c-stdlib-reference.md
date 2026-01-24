# C Standard Library Reference

## Memory Layout

**Stack vs Heap:**
```
High Address
┌─────────────────┐
│   Stack         │  ← Local variables, function calls (grows down)
│       ↓         │
├─────────────────┤
│                 │
│   (free space)  │
│                 │
├─────────────────┤
│       ↑         │
│   Heap          │  ← malloc/calloc (grows up)
├─────────────────┤
│   BSS           │  ← Uninitialized globals (zeroed)
├─────────────────┤
│   Data          │  ← Initialized globals
├─────────────────┤
│   Text          │  ← Program code (read-only)
└─────────────────┘
Low Address
```

**Struct Layout & Padding:**
```c
// Bad - 24 bytes (padding between fields)
struct Bad {
    char a;      // 1 byte + 7 padding
    double b;    // 8 bytes
    char c;      // 1 byte + 7 padding
};

// Good - 16 bytes (order by size descending)
struct Good {
    double b;    // 8 bytes
    char a;      // 1 byte
    char c;      // 1 byte + 6 padding
};
```

**Alignment rule:** Fields align to their size (or max 8 bytes). Structs pad to largest member's alignment.

---

## Core Headers Quick Reference

| Header | Key Functions |
|--------|---------------|
| `<stdio.h>` | printf, scanf, fopen, fread, fwrite, fclose, fgets, fputs |
| `<stdlib.h>` | malloc, calloc, realloc, free, atoi, strtol, exit, qsort |
| `<string.h>` | strlen, strcpy, strncpy, strcmp, strncmp, memcpy, memset, memmove |
| `<ctype.h>` | isalpha, isdigit, isspace, isalnum, toupper, tolower |
| `<math.h>` | sin, cos, sqrt, pow, floor, ceil, fabs, log |
| `<stdint.h>` | int8_t, int32_t, uint64_t, intptr_t, SIZE_MAX |
| `<stdbool.h>` | bool, true, false |
| `<assert.h>` | assert |
| `<errno.h>` | errno, EINVAL, ENOMEM |
| `<limits.h>` | INT_MAX, INT_MIN, CHAR_BIT, SIZE_MAX |

---

## Best Practices

### Memory Management
```c
// 1. Always check malloc
int* arr = malloc(n * sizeof(*arr));  // sizeof(*arr) not sizeof(int)
if (arr == NULL) { /* handle error */ }

// 2. Zero after free (prevents use-after-free bugs)
free(ptr);
ptr = NULL;

// 3. Use calloc for zeroed memory
int* zeroed = calloc(100, sizeof(int));

// 4. Match allocator/deallocator
// malloc/calloc/realloc → free
// fopen → fclose
// Custom allocator → custom deallocator
```

### Strings
```c
// 1. Prefer strncpy over strcpy (buffer overflow protection)
strncpy(dest, src, sizeof(dest) - 1);
dest[sizeof(dest) - 1] = '\0';  // Always null-terminate

// 2. Use snprintf over sprintf
snprintf(buf, sizeof(buf), "Value: %d", x);

// 3. strlen doesn't count null terminator
char* s = "hello";  // strlen = 5, memory = 6 bytes
```

### Pointers
```c
// 1. Initialize pointers
int* p = NULL;  // Not: int* p;

// 2. Check before dereference
if (p != NULL) { *p = 42; }

// 3. Understand pointer arithmetic
int arr[5];
int* p = arr;
p++;          // Moves sizeof(int) bytes, not 1 byte
p[2] = 10;    // Same as *(p + 2) = 10
```

---

## Common Patterns & Idioms

### 1. Flexible Array Member (dynamic struct size)
```c
typedef struct {
    size_t length;
    char data[];  // Must be last member
} String;

String* s = malloc(sizeof(String) + len + 1);
s->length = len;
memcpy(s->data, src, len + 1);
```

### 2. Opaque Pointers (encapsulation)
```c
// header.h
typedef struct Context Context;
Context* context_new(void);
void context_free(Context* ctx);

// source.c
struct Context {
    int internal_state;  // Hidden from users
};
```

### 3. Error Handling via Return Codes
```c
typedef enum { OK = 0, ERR_NOMEM, ERR_INVALID } Result;

Result do_something(int input, int* output) {
    if (input < 0) return ERR_INVALID;
    *output = input * 2;
    return OK;
}
```

### 4. Cleanup with goto
```c
int process_file(const char* path) {
    int result = -1;
    FILE* f = NULL;
    char* buf = NULL;

    f = fopen(path, "r");
    if (!f) goto cleanup;

    buf = malloc(1024);
    if (!buf) goto cleanup;

    // ... do work ...
    result = 0;

cleanup:
    free(buf);
    if (f) fclose(f);
    return result;
}
```

### 5. X-Macros (DRY for enums/strings)
```c
#define TOKENS(X) \
    X(TOKEN_PLUS, "+") \
    X(TOKEN_MINUS, "-") \
    X(TOKEN_STAR, "*")

// Generate enum
typedef enum {
    #define X(name, str) name,
    TOKENS(X)
    #undef X
} TokenType;

// Generate string array
const char* token_strings[] = {
    #define X(name, str) str,
    TOKENS(X)
    #undef X
};
```

### 6. Container_of (get parent from member pointer)
```c
#define container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - offsetof(type, member)))

struct Node { int data; };
struct List { struct Node node; int extra; };

struct Node* n = get_node();
struct List* l = container_of(n, struct List, node);
```

### 7. Static Assertions
```c
#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

STATIC_ASSERT(sizeof(int) == 4, int_must_be_4_bytes);
```

### 8. Grow-able Array
```c
typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} IntArray;

void array_push(IntArray* a, int value) {
    if (a->count >= a->capacity) {
        a->capacity = a->capacity < 8 ? 8 : a->capacity * 2;
        a->data = realloc(a->data, a->capacity * sizeof(int));
    }
    a->data[a->count++] = value;
}
```

---

## Memory Safety Checklist

- [ ] Every `malloc`/`calloc` has a matching `free`
- [ ] Every `fopen` has a matching `fclose`
- [ ] Buffer sizes checked before writes
- [ ] Pointers set to `NULL` after `free`
- [ ] No pointer arithmetic past array bounds
- [ ] Strings always null-terminated
- [ ] `sizeof` used with variable, not type: `sizeof(*ptr)`
- [ ] Check return values of allocation functions

---

## Debugging Tools

- **Valgrind:** `valgrind --leak-check=full ./program`
- **AddressSanitizer:** `gcc -fsanitize=address -g program.c`
- **UndefinedBehaviorSanitizer:** `gcc -fsanitize=undefined -g program.c`
