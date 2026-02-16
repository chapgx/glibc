#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #define GLIBC_IMPLEMENT

#define assert(expr, msg)                                                      \
  if (!(expr)) {                                                               \
    printf("\n1: ");                                                           \
    printf(msg "\n");                                                          \
    printf("2: Expression: %s\n3: File: %s\n4: Line %d\n\n", #expr, __FILE__,  \
           __LINE__);                                                          \
    abort();                                                                   \
  }

#ifndef GCORE
#define GCORE

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

// A slice contains a pointer to the underlying array
typedef struct Slice {
  u64 len;
  u64 cap;
  u8 *bytes;
} Slice;

/// New [Slice] allocates memory based on s
Slice g_slice_new(u64 s);

// Returns a new [Slice] from the `b` based on the start and end. This does not
// allocate memory.
//
// @param b - pointer to array
// @param start - start of [Slice] inclusive
// @param end - end of [Slice] exclusive
Slice g_slice_from(u8 *b, u8 start, u8 end);

// Append b to s if the capacity of s minus the len of s does not have enough
// space for b then a new allocation happens and the data is copied.
int g_slice_append(Slice *s, u8 *b);

#if defined(GLIBC_IMPLEMENT)

Slice g_slice_new(u64 s) {
  Slice buff = {.len = s, .cap = s};
  buff.bytes = (u8 *)malloc(buff.len);
  if (buff.bytes == NULL) {
    // TODO: handle memory error
  }
  return buff;
}

Slice g_slice_from(u8 *b, u8 start, u8 end) {
  // TODO: maybe do something if end is less that start
  assert(end > start, "expected start to be less than end");
  u8 *startat = b + start;
  u64 len = (u64)(end - start);
  Slice buff = {.len = len, .bytes = startat, .cap = len};
  return buff;
}

// TODO: implement g_slice_append

#endif

#endif // end GCORE

#ifndef G_RING_BUFFER
#define G_RING_BUFFER

// Ring buffer style memory allocation
typedef struct RingBuffer {
  u64 size;
  u8 *buffer;
  int head;
  int tail;
} RingBuffer;

// creates a new ring buffer
RingBuffer g_ringbuffer_new(u64 size);

// write to the ring buffer
//
// returns the number of bytes written -1 on error
//
int g_ringbuffer_write(RingBuffer *rb, Slice *b);

#endif // end G_RING_BUFFER

#if defined(GLIBC_IMPLEMENT)

// creates a new ring buffer
RingBuffer g_ringbuffer_new(u64 size) {
  u8 *buffer = (u8 *)malloc(size);
  RingBuffer rb = {.size = size, .buffer = buffer, .head = 0, .tail = 0};
  return rb;
}

int g_ringbuffer_write(RingBuffer *rb, Slice *b) {
  int isTailAhead = rb->tail > rb->head;

  int cap;
  if (isTailAhead) {
    cap = rb->tail - rb->head;
  } else {
    cap = (rb->size - rb->head) + rb->tail;
  }

  if (cap > b->len) {
    cap = b->len;
  }

  int written = 0;
  for (int i = 0; i < cap; i++) {
    rb->buffer[rb->head] = b->bytes[i];
    rb->head = (rb->head + 1) % rb->size;
    written += 1;
  }

  return written;
}

#endif // G_RING_BUFFER IMPLEMENTATION

#ifndef G_STR
#define G_STR

// String type
typedef struct Str {
  char *content;
  int len;
} Str;

// Generates a  new string struct from content
Str g_str_new(char *content);

// Generates a  new string struct from content with a predefined len
Str g_str_new_len(char *content, int len);

// g_str_append appends y to x return a new Str
Str g_str_append(Str x, Str y);

// TODO: probably will get rid of this one
//  g_str_free  frees the allocated memory for a string
void g_str_free(Str *s);
#endif

#if defined(GLIBC_IMPLEMENT)
Str g_str_new(char *content) {
  Str s = {.content = content};

  int len = 0;
  for (;;) {
    if (content[len] == 0) {
      break;
    }
    len = len + 1;
  }

  s.len = len;
  return s;
}

Str g_str_new_len(char *content, int len) {
  Str s = {.len = len, .content = content};
  return s;
}

Str g_str_append(Str x, Str y) {
  Str s = {.len = x.len + y.len};

  // TODO: probably better to handle with arenas or the option of.
  // Once you start concatunating like this and the memory is no longer own by
  // the outsid it could probably leak
  char *final = (char *)malloc(s.len);
  if (final == NULL) {
    Str s = {.len = 0, .content = 0};
    return s;
  }

  for (int i = 0; i < x.len; i += 1) {
    final[i] = x.content[i];
  }

  for (int i = 0; i < y.len; i += 1) {
    final[i] = y.content[i];
  }

  return s;
}

void g_str_free(Str *s) {
  free(s->content);
  s->content = 0;
}

#endif // GLIBC_IMPLEMENT
