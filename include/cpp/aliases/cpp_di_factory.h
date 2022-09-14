#ifndef STONKS_CPP_ALIASES_CPP_DI_FACTORY_H_
#define STONKS_CPP_ALIASES_CPP_DI_FACTORY_H_

#include <boost/di/extension/injections/factory.hpp>

/**
 * @file Short boost::di factory aliases.
 */

namespace stonks::cpp::di {
template <typename... Ts>
using IFactory = boost::di::extension::ifactory<Ts...>;

template <typename... Ts>
using Factory = boost::di::extension::factory<Ts...>;
}  // namespace stonks::cpp::di

#endif  // STONKS_CPP_ALIASES_CPP_DI_FACTORY_H_
