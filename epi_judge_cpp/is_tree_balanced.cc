#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
std::pair<int, bool> HeightBalanced(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {-1, true};
  auto left = HeightBalanced(tree->left);
  if (!left.second)
    return {0, false};
  auto right = HeightBalanced(tree->right);
  if (!right.second || std::abs(left.first - right.first) > 1)
    return {0, false};
  return {std::max(left.first, right.first) + 1, true};
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return HeightBalanced(tree).second;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
