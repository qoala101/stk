#ifndef STONKS_DI_DI_FACTORY_H_
#define STONKS_DI_DI_FACTORY_H_

#include <boost/di/extension/injections/factory.hpp>

#include "cpp_not_null.h"

namespace stonks::di {
/**
 * @file DI factory wrapper which creates not null objects.
 */
template <typename T>
class Factory {
 public:
  explicit Factory(cpp::NnSp<boost::di::extension::ifactory<T>> factory)
      : factory_{std::move(factory)} {}

  /**
   * @brief Creates not null object of type T.
   */
  [[nodiscard]] auto Create() const {
    return cpp::AssumeNn(factory_->create());
  }

 private:
  cpp::NnSp<boost::di::extension::ifactory<T>> factory_;
};
}  // namespace stonks::di

#endif  // STONKS_DI_DI_FACTORY_H_
