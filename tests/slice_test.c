#include "stdio.h"
#define GLIBC_IMPLEMENT
#include "../glibc.h"

int main(void) {
  char *hello = "hello world";
  Slice buff = g_slice_from((u8 *)hello, 0, 5);
  RingBuffer rb = g_ringbuffer_new(KiB(2));
  g_ringbuffer_write(&rb, &buff);

  printf("%zu\n", buff.len);
  printf("head=%i\n", rb.head);
  printf("tail=%i\n", rb.tail);

  printf("\"");
  for (int i = 0; i < buff.len; i++) {
    printf("%c", rb.buffer[i]);
  }
  printf("\"");
  printf("\n");
}
