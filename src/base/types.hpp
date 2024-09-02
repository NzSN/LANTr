#ifndef LANTR_BASE_TYPES_H_
#define LANTR_BASE_TYPES_H_

#include <concepts>
#include <string>

namespace LANTr::Base::Types {

using Source = std::string;



///////////////////////////////////////////////////////////////////////////////
//                               Algegra Types                               //
///////////////////////////////////////////////////////////////////////////////
template<typename T>
concept Monoid = requires(T& t1, T& t2) {
  // Associated binary operation
  { t1 + t2 } -> std::same_as<T>;
  // Identity
  { T{} } -> std::same_as<T>;
};


} // LANTr::Base::Types

#endif // LANTR_BASE_TYPES_H_
