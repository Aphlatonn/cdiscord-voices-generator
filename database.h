#ifndef DATABASE_H
#define DATABASE_H

#include <concord/types.h>
#include <sqlite3.h>

typedef struct {
  u64snowflake id;
  const char *prefix;
  u64snowflake voice;
  u64snowflake reject_voice;
  u64snowflake logs_channel;
} guild_data;

// initialize databse func
int initialize_database(const char *database_file);

// create tables if not exist
int create_tables(void);

// close the database
void close_database(void);

// insert raw into guilds table
int db_guild_insert(guild_data data);

#endif // DATABASE_H
