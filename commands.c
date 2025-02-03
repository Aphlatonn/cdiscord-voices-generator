#include <concord/discord.h>
#include <stdio.h>
#include <string.h>

#include "commands.h"
#include "parser.h"
#include "utils.h"

void cmd_help(struct discord *client, const struct discord_message *event) {
  // get he prefix
  char *prefix = 0;
  config_get_str("bot.prefix", &prefix);
  if (prefix == 0)
    prefix = "!";

  char buff[strlen(prefix) + 20];
  sprintf(buff, "commands prefix is %s", prefix);

  // send the message
  struct discord_create_message params = {
      .content = buff,
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
