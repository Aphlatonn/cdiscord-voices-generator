#ifndef UTILS_H
#define UTILS_H

// check if str start with prefix
int starts_with(const char *str, const char *prefix);

// split at the first delim
void split_at_first_delim(const char *str, const char delim, char *dist1, char *dist2);

#endif // UTILS_H
