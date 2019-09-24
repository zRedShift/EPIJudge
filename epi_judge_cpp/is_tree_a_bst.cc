#include <memory>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;

bool InorderTraversal(const unique_ptr<BinaryTreeNode<int>> &node, int &prev) {
  if (!node)
    return true;
  if (!InorderTraversal(node->left, prev) || prev > node->data)
    return false;
  return InorderTraversal(node->right, prev = node->data);
}

bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>> &tree) {
  int prev = std::numeric_limits<int>::min();
  return InorderTraversal(tree, prev);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                         &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
