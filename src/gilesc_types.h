#if !defined(GILESC_TYPES_H)
#define GILESC_TYPES_H

#if USE_GILESC_TYPES

#include <x86intrin.h>

#define internal          static
#define local_persist     static
#define global_variable   static

typedef int8_t   s8;
typedef int16_t  s16; // int
typedef int32_t  s32; // double
typedef int64_t  s64; // long
typedef s32      b32; // boolean

typedef uint8_t  u8;
typedef uint16_t u16; // int
typedef uint32_t u32; // double
typedef uint64_t u64; // long

typedef float    f32;
typedef double   f64;

#define U16_MAX  65535
#define S32_MIN  ((s32)0x80000000)
#define S32_MAX  ((s32)0x7fffffff)
#define U32_MIN  0
#define U32_MAX  ((u32)-1)
#define U64_MAX  ((u64)-1)
#define F32_MAX  3.402823E+38f
#define F32_MIN  -3.402823E+38f

#endif // USE_GILESC_TYPES
#endif
