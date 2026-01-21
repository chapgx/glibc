#include <stdio.h>

typedef struct Str {
  char *content;
  int len;
} Str;

// Generates a  new string struct from content
Str g_new_str(char *content);

// Generates a  new string struct from content with a predefined len
Str g_new_str_len(char *content, int len);

#if defined(G_STRING_IMP)

Str g_new_str(char *content) {
  Str s = {
      .content = content,
  };

  int len = 0;
  for (;;) {
    if (content[len] == NULL) {
      len = len + 1;
      break;
    }
    len = len + 1;
  }

  s.len = len;
  return s;
}

#endif // G_STRING_IMP
