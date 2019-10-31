#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  std::vector<int> result;
  std::stack<BinaryTreeNode<int> *> stack;
  auto curr = tree.get();
  while (!stack.empty() || curr)
    if (curr)
      stack.push(curr), curr = curr->left.get();
    else
      curr = stack.top(), stack.pop(), result.push_back(curr->data), curr = curr->right.get();
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_inorder.cc", "tree_inorder.tsv",
                         &InorderTraversal, DefaultComparator{}, param_names);
}
