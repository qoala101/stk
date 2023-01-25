#ifndef STONKS_CPP_CPP_FACTORY_H_
#define STONKS_CPP_CPP_FACTORY_H_

#include <type_traits>

#include "cpp_not_null.h"

namespace stonks::cpp {
/**
 * @brief Simple factory interface.
 */
template <typename T>
// NOLINTNEXTLINE(*-special-member-functions)
class IFactory {
 public:
  virtual ~IFactory() = default;

  /**
   * @brief Creates the object.
   */
  virtual auto Create [[nodiscard]] () const -> T = 0;
};

/**
 * @brief Factory for polymorphic types.
 */
template <typename T>
  requires std::is_polymorphic_v<T>
class IFactory<T> : public IFactory<NnUp<T>> {};
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_FACTORY_H_
