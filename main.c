#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>

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

  // get the prefix
  char *prefix = 0;
  switch (config_get_str("bot.prefix", &prefix)) {
  case CONFIG_NO_ENTRY:
    prefix = "!";
    log_warn("CONFIG: bot.prefix entry is not set");
    break;
  case CONFIG_NO_VALUE:
    prefix = "!";
    log_warn("CONFIG: bot.prefix has no value");
    break;
  case CONFIG_GOOD:
    break;
  };

  if (!starts_with(event->content, prefix))
    return;

  // find the command
  const command *cmd = get_command(event->content + strlen(prefix));

  // return if the commands was not found
  if (cmd == 0)
    return;

  // run the command
  cmd->run(client, event);
}

void on_voice_state_update(struct discord *client,
                           const struct discord_voice_state *event) {
  if (event->channel_id != 0) {
    // member join channel
    log_info(">> Member %s join %lu", event->member->user->username,
             event->channel_id);

  } else {
    // member left channel
    log_info(">> Member %s left %lu", event->member->user->username,
             event->channel_id);
  }
}

int main(int argc, char *argv[]) {
  int returnStatusCode = -1;

  // get config files names
  const char *user_config_file = argc > 1 ? argv[1] : "./user-config.conf";
  const char *bot_config_file = argc > 2 ? argv[2] : "./bot-config.json";
  const char *database_file = argc > 3 ? argv[3] : "./database.db";

  // load the config
  if (load_config(user_config_file) != 0) {
    log_error("Couldn't open config file");
    goto cleanup;
  } else {
    log_info("Config has been loaded");
  }

  // initialize the database
  if (initialize_database(database_file) != 0) {
    log_error("Couldn't initialize database");
    goto cleanup;
  } else {
    log_info("Database has been initialized");
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

  // enable cache
  discord_cache_enable(client, DISCORD_CACHE_GUILDS);
  discord_cache_enable(client, DISCORD_CACHE_MESSAGES);

  // add voice state update intent and message intent
  discord_add_intents(client, DISCORD_GATEWAY_VOICE_STATE_UPDATE);
  discord_add_intents(client, DISCORD_GATEWAY_MESSAGE_CONTENT);

  // register events
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message_create);
  discord_set_on_voice_state_update(client, &on_voice_state_update);

  // run the client
  discord_run(client);

// release memory
cleanup:
  if (client != 0)
    discord_cleanup(client);
  ccord_global_cleanup();
  free_config();
  return returnStatusCode;
}
