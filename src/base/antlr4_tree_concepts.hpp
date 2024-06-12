#ifndef LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_
#define LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_

#include <concepts>
#include <ranges>

template<typename T>
concept AntlrTree = requires(T& t) {
  { t.parent } -> std::convertible_to<T*>;
  { t.children } -> std::ranges::range;
};

template<AntlrTree T>
auto& GetChildren(T* tree) {
  return tree->children;
}

template<AntlrTree T>
T* GetParent(T* tree) {
  return tree->parent;
}

template<AntlrTree T>
size_t NumOfChildren(T* tree) {
  return GetChildren(tree).size();
}

#endif /* LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_ */
