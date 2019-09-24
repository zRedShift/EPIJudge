#include <memory>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>> RebuildBSTFromPreorder(
    const vector<int> &preorder_sequence) {
  if (preorder_sequence.empty())
    return nullptr;
  auto iter = preorder_sequence.begin();
  auto tree = std::make_unique<BstNode<int>>(*iter++);
  std::stack<BstNode<int> *> stack;
  stack.push(tree.get());
  while (iter != preorder_sequence.end()) {
    auto parent = stack.top();
    if (*iter < parent->data) {
      parent->left = std::make_unique<BstNode<int>>(*iter++);
      stack.push(parent->left.get());
    } else {
      stack.pop();
      while (!stack.empty() && stack.top()->data < *iter)
        parent = stack.top(), stack.pop();
      parent->right = std::make_unique<BstNode<int>>(*iter++);
      stack.push(parent->right.get());
    }
  }
  return tree;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder_sequence"};
  return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                         &RebuildBSTFromPreorder, DefaultComparator{},
                         param_names);
}
