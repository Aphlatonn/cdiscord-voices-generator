#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

static struct config_entry *config = 0;

int add_config_entry(char *key, char *value) {
  struct config_entry *new_entry = malloc(sizeof(struct config_entry));
  if (new_entry == 0) {
    return -1;
  }

  new_entry->key = strdup(key);
  if (new_entry->key == 0) {
    free(new_entry);
    return -1;
  }

  new_entry->value = strdup(value);
  if (new_entry->value == 0) {
    free(new_entry->key);
    free(new_entry);
    return -1;
  }

  new_entry->next = 0;

  if (config == 0) {
    config = new_entry;
  } else {
    struct config_entry *temp = config;
    while (temp->next) {
      temp = temp->next;
    }
    temp->next = new_entry;
  }

  return 0;
}

int load_config(const char *filepath) {
  FILE *fp = fopen(filepath, "r");
  if (fp == 0) {
    return -1;
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), fp)) {
    // skip comments
    if (starts_with(buffer, "#"))
      continue;

    buffer[strlen(buffer) - 1] = '\0';

    char key[512];
    char value[512];
    split_at_first_delim(buffer, '=', key, value);

    if (add_config_entry(key, value) != 0) {
      fclose(fp);
      return -1;
    }
  }

  fclose(fp);
  return 0;
}

char *config_get_str(char *key) {
  struct config_entry *temp = config;
  while (temp) {
    if (strcmp(key, temp->key) == 0) {
      return temp->value;
    }

    temp = temp->next;
  };
  return 0;
}

void free_config(void) {
  while (config) {
    struct config_entry *next = config->next;
    free(config->key);
    free(config->value);
    free(config);
    config = next;
  }
}
