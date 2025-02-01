#include <concord/discord.h>
#include <concord/log.h>

#include "commands.h"
#include "database.h"
#include "parser.h"
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
  int returnStatusCode = -1;

  // get config files names
  const char *user_config_file = argc > 1 ? argv[1] : "./user-config.conf";
  const char *bot_config_file = argc > 2 ? argv[2] : "./bot-config.json";

  // load the config
  if (load_config(user_config_file) != 0) {
    log_error("Couldn't open config file");
    goto cleanup;
  } else {
    log_info("Config has been loaded");
  }

  // get the token
  const char *token = config_get_str("discord.token");
  if (token == 0) {
    log_error("Couldn't find token in config");
    goto cleanup;
  }

  // initialize the database
  if (initialize_database("./database.db") != 0) {
    log_error("Couldn't initialize database");
    goto cleanup;
  } else {
    log_info("Database has been initialized");
  }

  // create the tables
  if (create_tables() != 0) {
    log_error("Couldn't create database tables");
    goto cleanup;
  } else {
    log_info("Tables has been created");
  }

  // login to discord
  ccord_global_init();
  struct discord *client = discord_config_init(bot_config_file);
  if (client == 0) {
    log_error("Couldn't initialize client");
    goto cleanup;
  } else {
    log_info("Client has been initialized");
  }

  returnStatusCode = 0;

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
  if (client != 0)
    discord_cleanup(client);
  ccord_global_cleanup();
  close_database();
  free_config();
  return returnStatusCode;
}
