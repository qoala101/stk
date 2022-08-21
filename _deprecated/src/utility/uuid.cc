#include "uuid.h"

#include <date/date.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <stdio.h>

#include <array>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ctime>
#include <sstream>

namespace stonks::utils {
boost::uuids::uuid ParseUuidFromString(std::string_view uuid) {
  return boost::uuids::string_generator{}(uuid.data());
}

std::string ConvertUuidToString(boost::uuids::uuid uuid) {
  return boost::uuids::to_string(uuid);
}

boost::uuids::uuid GenerateUuid() {
  static auto uuid_generator = boost::uuids::random_generator{};
  return uuid_generator();
}
}  // namespace stonks::utils