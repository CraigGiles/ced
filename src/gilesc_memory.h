#if !defined(GILESC_MEMORY_H)
/* ========================================================================
   Memory

   Version: v1.0.2
   Maintainer: Craig Giles
   License:	This file is placed in the public domain.
   
   v1.0.2 - Converted to C style. Will now compile with .c files
          - Provide empty Assert statements if USE_GILESC_ASSERT is not provided
   v1.0.1 - Adding more detailed message including caller and line number for when reserve_memory_ fails
   v1.0.0 - Initial Commit
   ======================================================================== */
#define GILESC_MEMORY_H

#if USE_GILESC_MEMORY

#if !defined(USE_GILESC_ASSERT)
    #define Assert(exp) if (!(exp))  exit(EXIT_FAILURE);
    #define AssertWithMessageArg(exp, ...)  if (!(exp))  exit(EXIT_FAILURE);
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) (Kilobytes(value)*1024LL)
#define Gigabytes(value) (Megabytes(value)*1024LL)
#define Terabytes(value) (Gigabytes(value)*1024LL)

internal f32
bytes_to_kilobytes(u32 bytes)
{
    f32 result = (f32)(bytes * 0.001);
    return result;
}

internal f32
bytes_to_megabytes(u32 bytes)
{
    f32 result = (f32)(bytes * 0.000001);
    return result;
}

// NOTE(craig): https://github.com/mtrebi/memory-allocators
typedef struct {
    size_t size;
    u8 *base;
    size_t used;

    s32 temp_count;
} MemoryArena;

typedef struct {
    MemoryArena *arena;
    size_t used;
} TemporaryMemory;

typedef struct {
    b32 BeingUsed;
    MemoryArena Arena;

    TemporaryMemory MemoryFlush;
} TaskWithMemory;

typedef struct {
    s32 count;
    TaskWithMemory tasks[4];
} TaskList;

#define ReserveMemoryForStruct(Arena, type) (type *)reserve_memory_(Arena, __func__, __LINE__, sizeof(type))
#define ReserveMemoryForArray(Arena, Count, type) (type *)reserve_memory_(Arena, __func__, __LINE__, (Count)*sizeof(type))
#define ReserveMemoryForSize(Arena, Size) reserve_memory_(Arena, __func__, __LINE__, Size)
#define ReserveMemoryForString(Arena, Length) (char*)reserve_memory_(Arena, __func__, __LINE__, sizeof(char) * Length)

internal void *
reserve_memory_(MemoryArena *arena, const char* caller, s32 line,
                size_t size)
{
    AssertWithMessageArg((arena->used + size) <= arena->size,
                         "Unable to allocate memory. Arena is full. \n\t %u of %u bytes used.\nCalled by: %s:%i",
                         (arena->used + size),
                         arena->size,
                         caller, line);
    void *result = arena->base + arena->used;
    arena->used += size;

    return result;
}

internal void
init_memory_arena(MemoryArena *arena, size_t size, void *base)
{
    arena->size = size;
    arena->base = (u8*)base;
    arena->used = 0;
}

internal TemporaryMemory
begin_temporary_memory(MemoryArena *arena) {
    TemporaryMemory result;

    result.arena = arena;
    result.used = arena->used;

    ++arena->temp_count;

    return(result);
}

internal void
end_temporary_memory(TemporaryMemory TempMem) {
    MemoryArena *arena = TempMem.arena;
    Assert(arena->used >= TempMem.used);
    arena->used = TempMem.used;
    Assert(arena->temp_count > 0);
    --arena->temp_count;
}

internal void
check_arena(MemoryArena *arena) {
    Assert(arena->temp_count == 0);
}

internal void
sub_arena(MemoryArena *destination, MemoryArena *source, size_t size)
{
    destination->size = size;
    destination->base = (u8 *)reserve_memory_(source, "gilesc_memory::sub_arena", __LINE__, size);
    destination->used = 0;
    destination->temp_count = 0;
}

#endif 

#endif
