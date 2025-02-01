#include <assert.h>
#include <concord/discord.h>
#include <concord/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "database.h"
#include "utils.h"

void on_ready(struct discord *client, const struct discord_ready *event) {
  log_info("Logged in as %s!", event->user->username);
}

void on_message_create(struct discord *client,
                       const struct discord_message *event) {
  // return if the message author is a bot
  if (event->author->bot)
    return;

  const char *prefix = "!";
  if (!starts_with(event->content, prefix)) {
    return;
  }

  // find the command
  // TODO: replace 1 with the prefix lenght
  const command *cmd = get_command(event->content + 1);

  // return if the commands was not found
  if (cmd == 0)
    return;

  // run the command
  cmd->run(client, event);
}

int main(int argc, char *argv[]) {
  // config file name
  const char *config_file = argc > 1 ? argv[1] : "./config.json";

  ccord_global_init();
  // login to discord
  struct discord *client = discord_config_init(config_file);
  assert(NULL != client && "Couldn't initialize client");

  int returnStatusCode = 0;

  // initialize the database
  if (initialize_database("./database.db") != 0) {
    returnStatusCode = -1;
    goto cleanup;
  } else {
    log_info("Database has been initialized");
  }

  // create the tables
  if (create_tables() != 0) {
    returnStatusCode = -1;
    goto cleanup;
  } else {
    log_info("Tables has been created");
  }

  // add voice state update intent and message intent
  discord_add_intents(client, DISCORD_GATEWAY_VOICE_STATE_UPDATE);
  discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);

  // register ready and message create events
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message_create);

  // run the client
  discord_run(client);

  // release memory
cleanup:
  discord_cleanup(client);
  ccord_global_cleanup();
  close_database();
  return returnStatusCode;
}
