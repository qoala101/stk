#ifndef STONKS_CPP_META_CPP_META_TEMPLATE_CONSTRUCTOR_H_
#define STONKS_CPP_META_CPP_META_TEMPLATE_CONSTRUCTOR_H_

#include "cpp_type_list.h"

namespace stonks::cpp::meta {
/**
 * @brief Util to be used to pass template arguments to constructors.
 */
template <typename... Ts>
using TemplateConstructor = cpp::TypeList<Ts...> *;
}  // namespace stonks::cpp::meta

#endif  // STONKS_CPP_META_CPP_META_TEMPLATE_CONSTRUCTOR_H_
