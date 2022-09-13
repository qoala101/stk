#include <boost/di.hpp>
#include <cstdio>
#include <iostream>
#include <string>
#include <tuple>

#include "app_pnd_items_interface_server.h"
#include "aws_api_handle.h"
#include "aws_dynamodb_factory.h"
#include "aws_dynamodb_sync_db_proxy.h"
#include "cpp_di_enable_not_null.h"
#include "cpp_factory.h"
#include "cpp_not_null.h"
#include "cpp_smart_pointers.h"
#include "network_i_factory.h"
#include "network_i_rest_request_receiver.h"
#include "nosqldb_i_factory.h"
#include "nosqldb_i_items_interface.h"
#include "not_null.hpp"
#include "restsdk_factory.h"
#include "restsdk_rest_request_receiver.h"

namespace {
auto MakeNetworkInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<stonks::network::IFactory>().to<stonks::restsdk::Factory>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::network::IFactory>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::network::IFactory>>(),

    boost::di::bind<stonks::cpp::IFactory<stonks::network::IRestRequestReceiver>>().to(stonks::cpp::Factory<stonks::restsdk::RestRequestReceiver>{}),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::cpp::IFactory<stonks::network::IRestRequestReceiver>>>()
  );
  // clang-format on
}

auto MakeAwsInjector() {
  // clang-format off
  return make_injector(
    boost::di::bind<stonks::cpp::NnSp<stonks::aws::ApiHandle>>.to([]() {
      return stonks::aws::ApiHandle::Instance();
    })
  );
  // clang-format on
}

auto MakeNosqldbInjector() {
  // clang-format off
  return make_injector(
    MakeAwsInjector(),

    boost::di::bind<stonks::nosqldb::IItemsInterface>().to<stonks::aws::dynamodb::SyncDbProxy>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::nosqldb::IItemsInterface>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::nosqldb::IItemsInterface>>(),

    boost::di::bind<stonks::nosqldb::IFactory>().to<stonks::aws::dynamodb::Factory>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Up<stonks::nosqldb::IFactory>>(),
    stonks::cpp::di::EnableNn<stonks::cpp::Sp<stonks::nosqldb::IFactory>>()
  );
  // clang-format on
}
}  // namespace

auto main(int /*unused*/, char* /*unused*/[]) -> int {
  std::cout << "Started: public_nosql_db\n";

  // clang-format off
  const auto injector = make_injector(
    MakeNetworkInjector(),
    MakeNosqldbInjector(),
    boost::di::bind<std::string>.to("http://localhost:6506/Entity")
  );
  // clang-format on

  const auto server = injector.create<stonks::app::pnd::ItemsInterfaceServer>();

  std::ignore = getchar();
  std::cout << "Ended: public_nosql_db\n";
}