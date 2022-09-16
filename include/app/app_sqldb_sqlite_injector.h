// #ifndef STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_
// #define STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_

// #include <boost/di.hpp>

// #include "cpp_di_enable_not_null.h"
// #include "cpp_smart_pointers.h"
// #include "sqldb_i_db.h"
// #include "sqldb_i_query_builder.h"
// #include "sqlite_db.h"
// #include "sqlite_query_builder.h"

// namespace stonks::app::injectors {
// [[nodiscard]] inline auto MakeSqldbSqliteInjector() {
//   // clang-format off
//   return make_injector(
//     boost::di::bind<sqldb::IDb>().to<sqlite::Db>(),
//     cpp::di::EnableNn<cpp::Up<sqldb::IDb>>(),
//     cpp::di::EnableNn<cpp::Sp<sqldb::IDb>>(),
//     boost::di::bind<sqldb::IQueryBuilder>().to<sqlite::QueryBuilder>(),
//     cpp::di::EnableNn<cpp::Up<sqldb::IQueryBuilder>>(),
//     cpp::di::EnableNn<cpp::Sp<sqldb::IQueryBuilder>>()
//   );
//   // clang-format on
// }
// }  // namespace stonks::app::injectors

// #endif  // STONKS_APP_APP_SQLDB_SQLITE_INJECTOR_H_
