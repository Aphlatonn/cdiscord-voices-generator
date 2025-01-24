#include "database.h"
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
      "voice          INTEGER,"
      "reject_voice   INTEGER,"
      "logs_channel   INTEGER);"
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

// insert raw into guilds table
int db_guild_insert(guild_data data) {
  const char *sql =
      "INSERT INTO guilds (id, prefix, voice, reject_voice, logs_channel) "
      "VALUES (?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt;
  int rc;

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  // bind values to the placeholders
  sqlite3_bind_int64(stmt, 1, data.id);
  sqlite3_bind_text(stmt, 2, data.prefix, -1, SQLITE_STATIC);
  sqlite3_bind_int64(stmt, 3, data.voice);
  sqlite3_bind_int64(stmt, 4, data.reject_voice);
  sqlite3_bind_int64(stmt, 5, data.logs_channel);

  // Execute the statement
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    return -1;
  }

  // Finalize the statement to release resources
  sqlite3_finalize(stmt);
  return 0;
}
