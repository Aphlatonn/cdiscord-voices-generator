#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdint.h>

typedef struct {
	uint64_t id;
	uint64_t voice;
	uint64_t reject_voice;
	uint64_t logs_channel;
} guild_data;

// initialize databse func
int initialize_database(const char *database_file);

// insert raw into guilds table
int db_guild_insert(guild_data data);

// read raw from guilds table
int db_guild_read(uint64_t guild_id, guild_data *dest);

// delete raw from guilds table
int db_guild_delete(uint64_t guild_id);

#endif // DATABASE_H
