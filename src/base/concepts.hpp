#ifndef LANTR_BASE_CONCEPTS_H_
#define LANTR_BASE_CONCEPTS_H_

#include <memory>

#include "base/tree_concepts.hpp"

namespace LANTr::Base::Concepts {

template<typename T>
requires std::is_pointer_v<T>
T GetRawPtr(T t) {
  return t;
}

template<typename T>
T* GetRawPtr(std::unique_ptr<T>& ptr) {
  return ptr.get();
}

template<typename T>
T* GetRawPtr(std::shared_ptr<T>& ptr) {
  return ptr.get();
}


} // LANTr::Base::Concepts


#endif /* LANTR_BASE_CONCEPTS_H_ */
