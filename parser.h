#ifndef PARSER_H
#define PARSER_H

struct config_entry {
  char *key;
  char *value;
  struct config_entry *next;
};

// load the config to memory
int load_config(const char *filepath);

// get a string value using a key
char *config_get_str(char *key);

// release the memory
void free_config(void);

#endif // PARSER_H
