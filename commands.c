#include <concord/discord.h>
#include <concord/log.h>
#include <string.h>

#include "commands.h"
#include "config.h"
#include "utils.h"

void cmd_help(struct discord *client, const struct discord_message *event)
{
	// get user data
	struct user_data *userdata = discord_get_data(client);

	char buff[strlen(userdata->config.bot.prefix) + 20];
	snprintf(buff, sizeof(buff), "commands prefix is %s", userdata->config.bot.prefix);

	// send the message
	struct discord_create_message params = {
		.content = buff,
	};
	discord_create_message(client, event->channel_id, &params, 0);
}

static command commands[] = {
	// help command
	{.name = "help", .aliases = (const char *[]){"help_v2", 0}, .run = &cmd_help},

	// end marker
	{0, 0, 0},
};

// get command by name
const command *get_command(const char *name)
{
	for (int i = 0; commands[i].name != 0; i++) {
		if (starts_with(name, commands[i].name))
			return &commands[i];
		else
			for (int j = 0; commands[i].aliases[j]; j++) {
				if (starts_with(name, commands[i].aliases[j]))
					return &commands[i];
			}
	}
	return 0;
}
