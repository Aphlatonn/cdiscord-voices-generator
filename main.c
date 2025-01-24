#include <assert.h>
#include <concord/discord.h>
#include <concord/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void on_ready(struct discord *client, const struct discord_ready *event) {
  log_info("Logged in as %s!", event->user->username);
}

int main(int argc, char *argv[]) {
  // config file name
  const char *config_file = argc > 1 ? argv[1] : "./config.json";

  ccord_global_init();
  // login to discord
  struct discord *client = discord_config_init(config_file);
  assert(NULL != client && "Couldn't initialize client");

  // register ready event
  discord_set_on_ready(client, &on_ready);

  // run the client
  discord_run(client);

  // release memory
  discord_cleanup(client);
  ccord_global_cleanup();
}
