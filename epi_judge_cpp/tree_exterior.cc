#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using NodeVector = std::vector<const unique_ptr<BinaryTreeNode<int>> *>;
void ExteriorBinaryTreeHelper(NodeVector &result, const unique_ptr<BinaryTreeNode<int>> &tree) {
  bool leaf = true;
  if (auto &next = tree->left; next)
    leaf = false, ExteriorBinaryTreeHelper(result, next);
  if (auto &next = tree->right; next)
    leaf = false, ExteriorBinaryTreeHelper(result, next);
  if (leaf)
    result.push_back(&tree);
}

NodeVector ExteriorBinaryTree(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  NodeVector result{&tree};
  if (!tree->left && !tree->right)
    return result;
  BinaryTreeNode<int> *next;
  if (tree->left) {
    for (auto curr = &tree; (next = curr->get());)
      result.push_back(curr = next->left ? &next->left : &next->right);
    result.pop_back(), result.pop_back();
  }
  ExteriorBinaryTreeHelper(result, tree);
  if (tree->right) {
    auto offset = result.size();
    for (auto curr = &tree; (next = curr->get());)
      result.push_back(curr = next->right ? &next->right : &next->left);
    result.pop_back(), result.pop_back();
    std::reverse(result.begin() + offset, result.end());
  }
  return result;
}
std::vector<int> CreateOutputVector(const NodeVector &L) {
  if (std::find(std::begin(L), std::end(L), nullptr) != std::end(L)) {
    throw TestFailure("Resulting list contains nullptr");
  }
  std::vector<int> output;
  for (const auto *l : L) {
    output.push_back((*l)->data);
  }
  return output;
}

std::vector<int> ExteriorBinaryTreeWrapper(TimedExecutor &executor, const unique_ptr<BinaryTreeNode<int>> &tree) {
  auto result = executor.Run([&] { return ExteriorBinaryTree(tree); });

  return CreateOutputVector(result);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree"};
  return GenericTestMain(args, "tree_exterior.cc", "tree_exterior.tsv",
                         &ExteriorBinaryTreeWrapper, DefaultComparator{},
                         param_names);
}
