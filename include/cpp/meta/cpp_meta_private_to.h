#ifndef STONKS_CPP_META_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_
#define STONKS_CPP_META_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_

#include <utility>

namespace stonks::cpp::meta {
/**
 * @brief Marks method to be a friend like to the specified type.
 * You could only call it from where this object can be constructed.
 * @code {.cpp}
 * void F(PrivateTo<Friend>, int x, int y);
 *
 * class Friend {
 *   void CallF() {
 *     f({}, 1, 2);
 *   }
 * }
 * @endcode
 */
template <typename Friend>
class PrivateTo {
 private:
  friend Friend;

  PrivateTo() = default;
};
}  // namespace stonks::cpp::meta

#endif  // STONKS_CPP_META_CPP_EXPOSE_PRIVATE_CONSTRUCTORS_H_
