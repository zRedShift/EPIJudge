#include <vector>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
using std::vector;
using Iter = vector<int>::const_iterator;

unique_ptr<BinaryTreeNode<int>> SubTree(const Iter ib, const Iter ie, const Iter pb, const Iter pe) {
  if (!std::distance(ib, ie))
    return nullptr;
  auto node = std::make_unique<BinaryTreeNode<int>>(BinaryTreeNode<int>{*pb});
  const auto ir = std::find(ib, ie, *pb);
  node->left = SubTree(ib, ir, pb + 1, pe), node->right = SubTree(ir + 1, ie, pb + std::distance(ib, ir) + 1, pe);
  return node;
}

unique_ptr<BinaryTreeNode<int>> BinaryTreeFromPreorderInorder(
    const vector<int> &preorder, const vector<int> &inorder) {
  return SubTree(inorder.begin(), inorder.end(), preorder.begin(), preorder.end());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder", "inorder"};
  return GenericTestMain(
      args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
      &BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
