#ifndef CONFIG_H
#define CONFIG_H

struct config {
	struct {
		char prefix[128];
	} bot;
};

struct user_data {
	struct config config;
};

// get cconfig
struct config get_config(struct discord *client);

#endif // CONFIG_H
