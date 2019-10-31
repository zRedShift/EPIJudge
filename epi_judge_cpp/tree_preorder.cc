#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<int> PreorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  std::vector<int> result;
  std::stack<BinaryTreeNode<int> *> stack;
  auto curr = tree.get();
  stack.push(nullptr);
  do {
    if (curr) {
      result.push_back(curr->data);
      if (auto right = curr->right.get(); right)
        stack.push(right);
      curr = curr->left.get();
    } else
      curr = stack.top(), stack.pop();
  } while (!stack.empty());
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_preorder.cc", "tree_preorder.tsv",
                         &PreorderTraversal, DefaultComparator{}, param_names);
}
