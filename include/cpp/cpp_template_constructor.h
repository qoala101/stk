#ifndef STONKS_CPP_CPP_TEMPLATE_CONSTRUCTOR_H_
#define STONKS_CPP_CPP_TEMPLATE_CONSTRUCTOR_H_

#include "cpp_type_list.h"

namespace stonks::cpp {
/**
 * @brief Util to be used to pass template arguments to constructors.
 */
template <typename... Ts>
using TemplateConstructor = cpp::TypeList<Ts...> *;
}  // namespace stonks::cpp

#endif  // STONKS_CPP_CPP_TEMPLATE_CONSTRUCTOR_H_
