#ifndef PARSER_H
#define PARSER_H

struct config_entry {
  char *key;
  char *value;
  struct config_entry *next;
};

enum CONFIG_STATUS {
  CONFIG_NO_ENTRY,
  CONFIG_NO_VALUE,
  CONFIG_GOOD,
};

// load the config to memory
int load_config(const char *filepath);

// get a string value using a key
enum CONFIG_STATUS config_get_str(char *key, char **dist);

// release the memory
void free_config(void);

#endif // PARSER_H
