#include <sqlite3.h>
#include <stdint.h>
#include <stdio.h>

#include "database.h"

static const char *DATABASE_FILE = 0;

int initialize_database(const char *database_file) {
  DATABASE_FILE = database_file;
  sqlite3 *db;
  int rc = sqlite3_open(database_file, &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  const char *sql =
      // guilds table
      "CREATE TABLE IF NOT EXISTS guilds ("
      "id INTEGER PRIMARY KEY, voice INTEGER, "
      "reject_voice INTEGER, logs_channel INTEGER);"
      // temp_voices table
      "CREATE TABLE IF NOT EXISTS temp_voices ("
      "id INTEGER PRIMARY KEY, guild_id INTEGER, owner_id INTEGER);";

  char *errmsg = 0;
  rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", errmsg);
    sqlite3_free(errmsg);
    sqlite3_close(db);
    return -1;
  }

  sqlite3_close(db);
  return 0;
}

int db_guild_insert(guild_data data) {
  sqlite3 *db;
  int rc = sqlite3_open(DATABASE_FILE, &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  const char *sql = "INSERT INTO guilds (id, voice, reject_voice, "
                    "logs_channel) VALUES (?, ?, ?, ?);";
  sqlite3_stmt *stmt;

  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, data.id);
  sqlite3_bind_int64(stmt, 2, data.voice);
  sqlite3_bind_int64(stmt, 3, data.reject_voice);
  sqlite3_bind_int64(stmt, 4, data.logs_channel);

  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}

int db_guild_read(uint64_t guild_id, guild_data *dest) {
  sqlite3 *db;
  int rc = sqlite3_open(DATABASE_FILE, &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  const char *sql = "SELECT id, voice, reject_voice, logs_channel FROM "
                    "guilds WHERE id = ?";
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, guild_id);
  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    dest->id = guild_id;
    dest->voice = sqlite3_column_int64(stmt, 1);
    dest->reject_voice = sqlite3_column_int64(stmt, 2);
    dest->logs_channel = sqlite3_column_int64(stmt, 3);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0; // row found
  } else if (rc == SQLITE_DONE) {
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1; // row not found
  } else {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return -1; // err
  }
}

int db_guild_delete(uint64_t guild_id) {
  sqlite3 *db;
  int rc = sqlite3_open(DATABASE_FILE, &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  const char *sql = "DELETE FROM guilds WHERE id = ?";
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return -1;
  }

  sqlite3_bind_int64(stmt, 1, guild_id);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to delete: %s\n", sqlite3_errmsg(db));
    return -1;
  }

  return 0;
}
