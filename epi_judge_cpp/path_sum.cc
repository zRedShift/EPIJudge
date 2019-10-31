#include <memory>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;

bool HasPathSumHelper(const unique_ptr<BinaryTreeNode<int>> &tree, int remaining_weight) {
  remaining_weight -= tree->data;
  char left = tree->left ? HasPathSumHelper(tree->left, remaining_weight) : -1,
      right = tree->right ? HasPathSumHelper(tree->right, remaining_weight) : -1;
  return left + right == -2 ? remaining_weight == 0 : left == 1 || right == 1;
}

bool HasPathSum(const unique_ptr<BinaryTreeNode<int>> &tree, int remaining_weight) {
  return tree ? HasPathSumHelper(tree, remaining_weight) : remaining_weight == 0;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "remaining_weight"};
  return GenericTestMain(args, "path_sum.cc", "path_sum.tsv", &HasPathSum,
                         DefaultComparator{}, param_names);
}
