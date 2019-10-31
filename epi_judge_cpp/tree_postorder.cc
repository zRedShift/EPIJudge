#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
std::vector<int> PostorderTraversal(const std::unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  std::vector<int> result;
  std::stack<BinaryTreeNode<int> *> stack;
  stack.push(tree.get());
  BinaryTreeNode<int> *prev = tree.get();
  do {
    auto curr = stack.top();
    bool not_prev_left = curr->left && curr->left.get() != prev;
    if (curr->right.get() == prev || !(curr->right || not_prev_left))
      result.push_back(curr->data), prev = curr, stack.pop();
    else
      stack.push(not_prev_left ? curr->left.get() : curr->right.get());
  } while (!stack.empty());
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_postorder.cc", "tree_postorder.tsv",
                         &PostorderTraversal, DefaultComparator{}, param_names);
}
