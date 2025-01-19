#include <stdint.h>

typedef uint8_t uint8;
typedef uint32_t uint32;

typedef enum {
    SUCCESS = 0,
    FILE_OPENING_ERROR = 1,
    NULL_POINTER_ERROR = 2,
} errorId_t;

typedef enum {
    false = 0,
    true = 1
} bool;