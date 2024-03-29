/**
 * STK @link https://github.com/qoala101/stk @endlink
 * @author Volodymyr Hromakov (4y5t6r@gmail.com)
 * @copyright Copyright (c) 2023, MIT License
 */

#ifndef VH_NETWORKX_TYPES_H_
#define VH_NETWORKX_TYPES_H_

#include <callable.hpp>
#include <member_function.hpp>

#include "cpp_concepts.h"  // IWYU pragma: keep

namespace vh::networkx {
/**
 * @brief Passing the instance to API instead of parameter name
 * would make it to parse parameter as body.
 */
struct RequestBody {};

/**
 * @brief Traits of endpoint function.
 * Must define kMethod and kParams with param names for each param.
 */
template <cpp::MemberFunction auto kFunction>
struct EndpointFunctionTraits;

/**
 * @brief Traits of client-server type.
 * Must define its endpoint methods as kEndpoints.
 */
template <typename T>
struct ClientServerTypeTraits;

/**
 * @brief Type of the member function parent.
 */
template <cpp::MemberFunction auto kFunction>
using ParentType =
    typename member_function_traits<decltype(kFunction)>::class_type;

/**
 * @brief Argument type of the member function.
 */
template <cpp::MemberFunction auto kFunction, unsigned kIndex>
using ArgType = typename member_function_traits<
    decltype(kFunction)>::template argument_type<kIndex>;
}  // namespace vh::networkx

#endif  // VH_NETWORKX_TYPES_H_
