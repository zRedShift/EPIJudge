#include <memory>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;

BinaryTreeNode<int> *LCA(const unique_ptr<BinaryTreeNode<int>> &node0,
                         const unique_ptr<BinaryTreeNode<int>> &node1) {
  std::unordered_set<BinaryTreeNode<int> *> parents;
  auto parent0 = node0.get(), parent1 = node1.get();
  while (parent0 && parent1) {
    if (!parents.insert(parent0).second)
      return parent0;
    if (!parents.insert(parent1).second)
      return parent1;
    parent0 = parent0->parent, parent1 = parent1->parent;
  }
  if (!parent0)
    std::swap(parent0, parent1);
  while (parent0) {
    if (parents.find(parent0) != parents.end())
      return parent0;
    parent0 = parent0->parent;
  }
  return nullptr;
}
int LcaWrapper(TimedExecutor &executor,
               const unique_ptr<BinaryTreeNode<int>> &tree, int key0,
               int key1) {
  const unique_ptr<BinaryTreeNode<int>> &node0 = MustFindNode(tree, key0);
  const unique_ptr<BinaryTreeNode<int>> &node1 = MustFindNode(tree, key1);

  auto result = executor.Run([&] { return LCA(node0, node1); });

  if (!result) {
    throw TestFailure("Result can not be nullptr");
  }
  return result->data;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
  return GenericTestMain(args, "lowest_common_ancestor_close_ancestor.cc",
                         "lowest_common_ancestor.tsv", &LcaWrapper,
                         DefaultComparator{}, param_names);
}
