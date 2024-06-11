#ifndef LANTR_BASE_TREE_CONCEPTS_H_
#define LANTR_BASE_TREE_CONCEPTS_H_

#include <concepts>
#include <ranges>

namespace LANTr::Base::TreeConcepts {

///////////////////////////////////////////////////////////////////////////////
//                         Operation of Tree Concepts                        //
///////////////////////////////////////////////////////////////////////////////
template<typename T>
auto GetChildren(T*);

template<typename T>
T* GetParent(T*);


template<typename T>
concept InternalTree = requires(T& t) {
  { t.GetChildren() } -> std::ranges::range;
  { t.Parent() } -> std::convertible_to<T*>;
};

template<InternalTree T>
auto& GetChildren(T* tree) {
  return tree->GetChildren();
}

template<InternalTree T>
auto* GetParent(T* tree) {
  return tree->Parent();
}

#include "base/antlr4_tree_concepts.hpp"

///////////////////////////////////////////////////////////////////////////////
//                               Tree Concepts                               //
///////////////////////////////////////////////////////////////////////////////
template<typename T>
concept NAryTree = requires(T& t) {
  { GetParent(&t) } -> std::convertible_to<T*>;
  { GetChildren(&t) } -> std::ranges::range;
};

} // LANTr::Base::TreeConcepts

#endif /* LANTR_BASE_TREE_CONCEPTS_H_ */
