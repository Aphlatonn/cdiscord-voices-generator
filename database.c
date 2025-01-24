#include <sqlite3.h>
#include <stdio.h>

static sqlite3 *db = 0;

// initialize databse func
int initialize_database(const char *database_file) {
  // Check if the database has already been initialized
  if (db != 0) {
    return 0;
  }

  // open the sqlite database
  int rc = sqlite3_open(database_file, &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

// create tables if not exist
int create_tables(void) {
  // sql
  const char *sql =
      "CREATE TABLE IF NOT EXISTS guilds (" // guilds table
      "id             INTEGER PRIMARY KEY,"
      "prefix         TEXT,"
      "voice          TEXT,"
      "reject_voice   TEXT,"
      "logs_channel   TEXT);"
      "CREATE TABLE IF NOT EXISTS temp_voices (" // tempvoices table
      "id             INTEGER PRIMARY KEY,"
      "guild_id       INTEGER,"
      "owner_id       INTEGER)";

  // create tables
  int rc = sqlite3_exec(db, sql, 0, 0, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

// close the database
void close_database(void) {
  if (db != 0) {
    sqlite3_close(db);
    db = 0;
  }
}
