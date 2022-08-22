#ifndef STONKS_CPP_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_
#define STONKS_CPP_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_

#include <utility>

namespace stonks::cpp {
/**
 * @brief Enables friend like access to the private constructors.
 * @tparam TFriend Type which would have acces to constructors of T.
 * @tparam T Type with private constructors to expose.
 * @remark Should be declared as friend of the exposed type.
 */
template <typename TFriend, typename T>
class ExposePrivateConstructorsTo {
 private:
  friend T;
  friend TFriend;

  /**
   * @brief Constructs T calling private constructor.
   */
  template <typename... Args>
  auto operator()(Args &&...args) -> T {
    return T{std::forward<Args>(args)...};
  }
};

/**
 * @brief Alias for better readability of the client code.
 */
template <typename TFriend, typename T>
using CallExposedPrivateConstructorOf = ExposePrivateConstructorsTo<T, TFriend>;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_
