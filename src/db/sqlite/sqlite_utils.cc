#include "sqlite_utils.h"

#include <bits/exception.h>
#include <fmt/format.h>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace stonks::db::sqlite::utils {
namespace {
[[nodiscard]] auto Logger() -> spdlog::logger & {
  static auto logger = []() {
    auto logger = spdlog::stdout_color_mt("Sqlite");
    logger->set_level(spdlog::level::debug);
    return logger;
  }();

  return *logger;
}
}  // namespace

auto ReadSqliteDbFromFile(std::string_view file_path)
    -> gsl::not_null<sqlite3 *> {
  auto *sqlite_db = (sqlite3 *){};
  sqlite3_open(file_path.data(), &sqlite_db);

  if (sqlite_db == nullptr) {
    throw std::runtime_error{std::string{"Cannot open DB from "} +
                             file_path.data() + ": " +
                             sqlite3_errmsg(sqlite_db)};
  }

  sqlite3_exec(sqlite_db, "PRAGMA foreign_keys = ON", nullptr, nullptr,
               nullptr);

  Logger().info("Read DB from {}", file_path.data());
  return sqlite_db;
}

void WriteSqliteDbToFile(sqlite3 &sqlite_db, std::string_view file_path) {
  Logger().info("Wrote DB to {}", file_path.data());
}

// /*
// ** This function is used to load the contents of a database file on disk
// ** into the "main" database of open database connection pInMemory, or
// ** to save the current contents of the database opened by pInMemory into
// ** a database file on disk. pInMemory is probably an in-memory database,
// ** but this function will also work fine if it is not.
// **
// ** Parameter zFilename points to a nul-terminated string containing the
// ** name of the database file on disk to load from or save to. If parameter
// ** isSave is non-zero, then the contents of the file zFilename are
// ** overwritten with the contents of the database opened by pInMemory. If
// ** parameter isSave is zero, then the contents of the database opened by
// ** pInMemory are replaced by data loaded from the file zFilename.
// **
// ** If the operation is successful, SQLITE_OK is returned. Otherwise, if
// ** an error occurs, an SQLite error code is returned.
// */
// int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave) {
//   int rc;                  /* Function return code */
//   sqlite3 *pFile;          /* Database connection opened on zFilename */
//   sqlite3_backup *pBackup; /* Backup object used to copy data */
//   sqlite3 *pTo;            /* Database to copy to (pFile or pInMemory) */
//   sqlite3 *pFrom;          /* Database to copy from (pFile or pInMemory) */

//   /* Open the database file identified by zFilename. Exit early if this fails
//   ** for any reason. */
//   rc = sqlite3_open(zFilename, &pFile);
//   if (rc == SQLITE_OK) {
//     /* If this is a 'load' operation (isSave==0), then data is copied
//     ** from the database file just opened to database pInMemory.
//     ** Otherwise, if this is a 'save' operation (isSave==1), then data
//     ** is copied from pInMemory to pFile.  Set the variables pFrom and
//     ** pTo accordingly. */
//     pFrom = (isSave ? pInMemory : pFile);
//     pTo = (isSave ? pFile : pInMemory);

//     /* Set up the backup procedure to copy from the "main" database of
//     ** connection pFile to the main database of connection pInMemory.
//     ** If something goes wrong, pBackup will be set to NULL and an error
//     ** code and message left in connection pTo.
//     **
//     ** If the backup object is successfully created, call backup_step()
//     ** to copy data from pFile to pInMemory. Then call backup_finish()
//     ** to release resources associated with the pBackup object.  If an
//     ** error occurred, then an error code and message will be left in
//     ** connection pTo. If no error occurred, then the error code belonging
//     ** to pTo is set to SQLITE_OK.
//     */
//     pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
//     if (pBackup) {
//       (void)sqlite3_backup_step(pBackup, -1);
//       (void)sqlite3_backup_finish(pBackup);
//     }
//     rc = sqlite3_errcode(pTo);
//   }

//   /* Close the database connection opened on database file zFilename
//   ** and return the result of this function. */
//   (void)sqlite3_close(pFile);
//   return rc;
// }

// /*
// ** Perform an online backup of database pDb to the database file named
// ** by zFilename. This function copies 5 database pages from pDb to
// ** zFilename, then unlocks pDb and sleeps for 250 ms, then repeats the
// ** process until the entire database is backed up.
// **
// ** The third argument passed to this function must be a pointer to a progress
// ** function. After each set of 5 pages is backed up, the progress function
// ** is invoked with two integer parameters: the number of pages left to
// ** copy, and the total number of pages in the source file. This information
// ** may be used, for example, to update a GUI progress bar.
// **
// ** While this function is running, another thread may use the database pDb,
// or
// ** another process may access the underlying database file via a separate
// ** connection.
// **
// ** If the backup process is successfully completed, SQLITE_OK is returned.
// ** Otherwise, if an error occurs, an SQLite error code is returned.
// */
// int backupDb(sqlite3 *pDb,               /* Database to back up */
//              const char *zFilename,      /* Name of file to back up to */
//              void (*xProgress)(int, int) /* Progress function to invoke */
// ) {
//   int rc;                  /* Function return code */
//   sqlite3 *pFile;          /* Database connection opened on zFilename */
//   sqlite3_backup *pBackup; /* Backup handle used to copy data */

//   /* Open the database file identified by zFilename. */
//   rc = sqlite3_open(zFilename, &pFile);
//   if (rc == SQLITE_OK) {
//     /* Open the sqlite3_backup object used to accomplish the transfer */
//     pBackup = sqlite3_backup_init(pFile, "main", pDb, "main");
//     if (pBackup) {
//       /* Each iteration of this loop copies 5 database pages from database
//       ** pDb to the backup database. If the return value of backup_step()
//       ** indicates that there are still further pages to copy, sleep for
//       ** 250 ms before repeating. */
//       do {
//         rc = sqlite3_backup_step(pBackup, 5);
//         xProgress(sqlite3_backup_remaining(pBackup),
//                   sqlite3_backup_pagecount(pBackup));
//         if (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
//           sqlite3_sleep(250);
//         }
//       } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);

//       /* Release resources allocated by backup_init(). */
//       (void)sqlite3_backup_finish(pBackup);
//     }
//     rc = sqlite3_errcode(pFile);
//   }

//   /* Close the database connection opened on database file zFilename
//   ** and return the result of this function. */
//   (void)sqlite3_close(pFile);
//   return rc;
// }
}  // namespace stonks::db::sqlite::utils