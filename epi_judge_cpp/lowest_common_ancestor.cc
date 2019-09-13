#include <memory>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;
std::pair<BinaryTreeNode<int> *, bool> LCAHelper(const unique_ptr<BinaryTreeNode<int>> &tree,
                                                 const unique_ptr<BinaryTreeNode<int>> &node0,
                                                 const unique_ptr<BinaryTreeNode<int>> &node1) {
  if (!tree)
    return {nullptr, false};
  bool found = tree == node0 || tree == node1;
  auto left = LCAHelper(tree->left, node0, node1);
  if (left.first)
    return left;
  if (left.second && found)
    return {tree.get(), true};
  found = found || left.second;
  auto right = LCAHelper(tree->right, node0, node1);
  if (right.first)
    return right;
  if (right.second && found)
    return {tree.get(), true};
  return {nullptr, right.second || found};
}
BinaryTreeNode<int> *LCA(const unique_ptr<BinaryTreeNode<int>> &tree,
                         const unique_ptr<BinaryTreeNode<int>> &node0,
                         const unique_ptr<BinaryTreeNode<int>> &node1) {
  if (node0 == node1)
    return node0.get();
  return LCAHelper(tree, node0, node1).first;
}
int LcaWrapper(TimedExecutor &executor,
               const unique_ptr<BinaryTreeNode<int>> &tree, int key0,
               int key1) {
  const unique_ptr<BinaryTreeNode<int>> &node0 = MustFindNode(tree, key0);
  const unique_ptr<BinaryTreeNode<int>> &node1 = MustFindNode(tree, key1);

  auto result = executor.Run([&] { return LCA(tree, node0, node1); });

  if (!result) {
    throw TestFailure("Result can not be nullptr");
  }
  return result->data;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
  return GenericTestMain(args, "lowest_common_ancestor.cc",
                         "lowest_common_ancestor.tsv", &LcaWrapper,
                         DefaultComparator{}, param_names);
}
