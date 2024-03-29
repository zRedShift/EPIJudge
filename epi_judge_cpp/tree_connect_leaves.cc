#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;
using std::vector;
void CreateListOfLeavesHelper(vector<const unique_ptr<BinaryTreeNode<int>> *> &result,
                              const unique_ptr<BinaryTreeNode<int>> &tree) {
  bool leaf = true;
  if (auto &next = tree->left; next)
    leaf = false, CreateListOfLeavesHelper(result, next);
  if (auto &next = tree->right; next)
    leaf = false, CreateListOfLeavesHelper(result, next);
  if (leaf)
    result.push_back(&tree);
}

vector<const unique_ptr<BinaryTreeNode<int>> *> CreateListOfLeaves(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  std::vector<const unique_ptr<BinaryTreeNode<int>> *> result;
  CreateListOfLeavesHelper(result, tree);
  return result;
}
vector<int> CreateListOfLeavesWrapper(
    TimedExecutor &executor, const unique_ptr<BinaryTreeNode<int>> &tree) {
  auto result = executor.Run([&] { return CreateListOfLeaves(tree); });

  if (std::any_of(std::begin(result), std::end(result),
                  [](const unique_ptr<BinaryTreeNode<int>> *&x) {
                    return !x || !*x;
                  })) {
    throw TestFailure("Result list can't contain nullptr");
  }

  vector<int> extracted_result;
  for (const auto &x : result) {
    extracted_result.push_back(x->get()->data);
  }
  return extracted_result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree"};
  return GenericTestMain(args, "tree_connect_leaves.cc",
                         "tree_connect_leaves.tsv", &CreateListOfLeavesWrapper,
                         DefaultComparator{}, param_names);
}
