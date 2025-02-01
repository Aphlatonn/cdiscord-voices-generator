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
