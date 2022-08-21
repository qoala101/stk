#ifndef STONKS_UTILITY_UUID_H_
#define STONKS_UTILITY_UUID_H_

#include <boost/uuid/uuid.hpp>

namespace stonks::utils {
boost::uuids::uuid ParseUuidFromString(std::string_view uuid);

std::string ConvertUuidToString(boost::uuids::uuid uuid);

boost::uuids::uuid GenerateUuid();
}

#endif  // STONKS_UTILITY_UUID_H_
