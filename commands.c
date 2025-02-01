#include <concord/discord.h>

#include "commands.h"
#include "utils.h"

void cmd_help(struct discord *client, const struct discord_message *event) {
  struct discord_create_message params = {
      .content = "commands prefix is !",
  };
  discord_create_message(client, event->channel_id, &params, 0);
}

static command commands[] = {
    {.name = "help", .run = &cmd_help},
    // end marker
    {.name = 0, .run = 0},
};

// get command by name
const command *get_command(const char *name) {
  for (int i = 0; commands[i].name != 0; i++) {
    if (starts_with(name, commands[i].name))
      return &commands[i];
  }
  return 0;
}
