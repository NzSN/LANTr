#ifndef LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_
#define LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_

#include <concepts>
#include <ranges>
#include <vector>

template<typename T>
concept AbstTree = requires(T& t) {
  { t.parent } -> std::convertible_to<T*>;
  { t.children } -> std::ranges::range;
};

template<AbstTree T>
auto GetChildren(T* tree) -> decltype(std::vector<T*>{}) & {
  return tree->children;
}

template<AbstTree T>
T* GetParent(T* tree) {
  return tree->parent;
}

template<AbstTree T>
size_t NumOfChildren(T* tree) {
  return GetChildren(tree).size();
}

#endif /* LANTR_BASE_ANTLR4_TREE_CONCEPTS_H_ */
