#ifndef COMMANDS_H
#define COMMANDS_H

#include <concord/discord.h>

typedef struct {
  const char *name;
  void (*run)(struct discord *client, const struct discord_message *event);
} command;

// get command by name
const command *get_command(const char *name);

#endif // COMMANDS_H
