#ifndef VH_CPP_META_TEMPLATE_CONSTRUCTOR_H_
#define VH_CPP_META_TEMPLATE_CONSTRUCTOR_H_

#include "cpp_type_list.h"

namespace vh::cpp::meta {
/**
 * @brief Util to be used to pass template arguments to constructors.
 */
template <typename... Ts>
using TemplateConstructor = TypeList<Ts...> *;
}  // namespace vh::cpp::meta

#endif  // VH_CPP_META_TEMPLATE_CONSTRUCTOR_H_
