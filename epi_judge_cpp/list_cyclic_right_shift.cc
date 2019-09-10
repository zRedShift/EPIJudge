#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
using std::shared_ptr;

shared_ptr<ListNode<int>> CyclicallyRightShiftList(shared_ptr<ListNode<int>> L,
                                                   int k) {
  if (!L)
    return nullptr;
  auto node = L;
  int len = 1;
  while (node->next)
    ++len, node = node->next;
  node->next = L;
  k = (len - k % len) % len;
  while (k--)
    node = node->next;
  L = node->next;
  node->next = nullptr;
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "k"};
  return GenericTestMain(
      args, "list_cyclic_right_shift.cc", "list_cyclic_right_shift.tsv",
      &CyclicallyRightShiftList, DefaultComparator{}, param_names);
}
