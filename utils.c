#include <string.h>

int starts_with(char *str, char *prefix) {
  while (*prefix) {
    if (*str != *prefix) {
      return 0;
    }
    str++;
    prefix++;
  }
  return 1;
}

void split_at_first_delim(const char *str, char delim, char *dist1,
                          char *dist2) {
  const char *str_temp_p = str;

  while (*str_temp_p) {
    if (*str_temp_p == delim) {
      strncpy(dist1, str, str_temp_p - str);
      dist1[str_temp_p - str] = '\0';

      strcpy(dist2, str_temp_p + 1);
      break;
    }
    str_temp_p++;
  }
}
