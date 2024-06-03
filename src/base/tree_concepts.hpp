#ifndef LANTR_BASE_TREE_CONCEPTS_H_
#define LANTR_BASE_TREE_CONCEPTS_H_

#include <concepts>
#include <ranges>

#include "base/n_ary_tree.hpp"

#include <memory>

namespace LANTr::Base::TreeConcepts {

template<typename T>
concept AntlrTree = requires(T& t) {
  { t.parent } -> std::convertible_to<T*>;
  { t.children } -> std::ranges::range;
};

template<typename T>
auto GetChildren(T*);

template<typename T>
requires TreeConcepts::AntlrTree<T>
auto GetChildren(T* tree) {
  return tree->children;
}

template<typename T>
requires std::derived_from<T, Tree<T>>
auto& GetChildren(T* tree) {
  return tree->GetChildren();
}

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


} // LANTr::Base::TreeConcepts

#endif /* LANTR_BASE_TREE_CONCEPTS_H_ */
