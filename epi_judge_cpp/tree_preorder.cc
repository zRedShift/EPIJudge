#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<int> PreorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  std::vector<int> result;
  std::stack<BinaryTreeNode<int> *> stack;
  stack.emplace(tree.get());
  while (!stack.empty()) {
    auto curr = stack.top();
    stack.pop();
    if (curr)
      result.push_back(curr->data), stack.push(curr->right.get()), stack.push(curr->left.get());
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_preorder.cc", "tree_preorder.tsv",
                         &PreorderTraversal, DefaultComparator{}, param_names);
}
