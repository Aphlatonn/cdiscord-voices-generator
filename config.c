#include <concord/discord.h>

#include "config.h"

struct config get_config(struct discord *client) {
	struct config conf = {0};
	char *path[3] = {"user_config", "", ""};
	struct ccord_szbuf_readonly json;

	/* config.bot */
	path[1] = "bot";

	/* config.bot.prefix */
	path[2] = "prefix";
	json = discord_config_get_field(client, path, 3);
	snprintf(conf.bot.prefix, sizeof(conf.bot.prefix), "%.*s", (int)json.size, json.start);

	return conf;
}
