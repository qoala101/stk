#ifndef STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_TYPES_H_
#define STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_TYPES_H_

#include "cpp_typed_struct.h"

namespace stonks::app::dt::stec {
struct Symbol : public cpp::TypedStruct<std::string> {};

struct Price : public cpp::TypedStruct<double> {};
}  // namespace stonks::app::dt::stec

#endif  // STONKS_APP_DOCKER_TEST_SERVER_TO_EXTERNAL_CLIENT_APP_DT_STEC_TYPES_H_
