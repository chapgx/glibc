#include "stdio.h"
#define G_STD_IMPLEMENT
#include "../glibc.h"

int main(void) {
  u8 *hello = "hello world";
  Slice buff = g_buffer_from_mem(hello, 11);
  RingBuffer rb = g_ringbuffer_new(KiB(2));
  int written = g_ringbuffer_write(&rb, &buff);
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
