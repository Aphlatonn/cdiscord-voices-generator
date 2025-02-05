#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>

#include "commands.h"
#include "config.h"
#include "database.h"
#include "utils.h"

void on_ready(struct discord *client, const struct discord_ready *event)
{
	log_info("Logged in as %s!", event->user->username);
}

void on_message_create(struct discord *client, const struct discord_message *event)
{
	// return if the message author is a bot
	if (event->author->bot)
		return;

	// get user data
	struct user_data *userdata = discord_get_data(client);

	if (!starts_with(event->content, userdata->config.bot.prefix))
		return;

	// find the command
	const command *cmd = get_command(event->content + strlen(userdata->config.bot.prefix));

	// return if the commands was not found
	if (cmd == 0)
		return;

	// run the command
	cmd->run(client, event);
}

void on_voice_state_update(struct discord *client, const struct discord_voice_state *event)
{
	if (event->channel_id != 0) {
		// member join channel
		log_info(">> Member %s joined %lu", event->member->user->username, event->channel_id);
	} else {
		// member left channel
		log_info(">> Member %s left %lu", event->member->user->username, event->channel_id);
	}
}

int main(int argc, char *argv[])
{
	// get files names
	const char *config_file = argc > 2 ? argv[2] : "./config.json";
	const char *database_file = argc > 3 ? argv[3] : "./database.db";

	// initialize the database
	if (initialize_database(database_file) != 0) {
		log_error("Couldn't initialize database");
		return -1;
	} else {
		log_info("Database has been initialized");
	}

	// login to discord
	ccord_global_init();
	struct discord *client = discord_config_init(config_file);
	if (client == 0) {
		log_error("Couldn't initialize client");
		return -1;
	} else {
		log_info("Client has been initialized");
	}

	// set client data
	struct user_data data = {.config = get_config(client)};
	discord_set_data(client, &data);

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

	discord_cleanup(client);
	ccord_global_cleanup();
	return 0;
}
