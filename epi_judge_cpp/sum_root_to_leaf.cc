#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

int SumRootToLeafHelper(const unique_ptr<BinaryTreeNode<int>> &tree, int result) {
  result = result * 2 + tree->data;
  int sum = tree->left ? SumRootToLeafHelper(tree->left, result) : 0;
  sum += tree->right ? SumRootToLeafHelper(tree->right, result) : 0;
  return sum ? sum : result;
}

int SumRootToLeaf(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return tree ? SumRootToLeafHelper(tree, 0) : 0;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "sum_root_to_leaf.cc", "sum_root_to_leaf.tsv",
                         &SumRootToLeaf, DefaultComparator{}, param_names);
}
