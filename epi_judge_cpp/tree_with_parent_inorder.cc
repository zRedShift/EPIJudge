#include <vector>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/generic_test.h"
using std::vector;

void ReachNext(BinaryTreeNode<int> *&node, vector<int> &vec) {
  while (true) {
    while (node->left)
      node = node->left.get();
    vec.emplace_back(node->data);
    if (node->right)
      node = node->right.get();
    else
      break;
  }
}

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<int> vec;
  if (!tree)
    return vec;
  auto node = tree.get();
  ReachNext(node, vec);
  while (node->parent)
    if (auto temp = node; temp == (node = node->parent)->left.get()) {
      vec.emplace_back(node->data);
      if (node->right) {
        node = node->right.get();
        ReachNext(node, vec);
      }
    }
  return vec;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_with_parent_inorder.cc",
                         "tree_with_parent_inorder.tsv", &InorderTraversal,
                         DefaultComparator{}, param_names);
}
