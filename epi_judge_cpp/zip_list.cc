#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
using std::shared_ptr;
using std::swap;

shared_ptr<ListNode<int>> ZippingLinkedList(const shared_ptr<ListNode<int>> &L) {
  if (!L)
    return nullptr;
  shared_ptr<ListNode<int>> slow = L, fast = slow, prev = nullptr;
  while (fast->next && (fast = fast->next->next))
    swap(slow->next, prev), swap(slow, prev);
  if (!fast)
    swap(slow->next, prev), swap(slow, prev);
  else
    fast = slow, slow = slow->next, fast->next = nullptr;
  while (prev)
    swap(fast, slow->next), swap(prev->next, slow), swap(prev, fast), swap(slow, prev);
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "zip_list.cc", "zip_list.tsv",
                         &ZippingLinkedList, DefaultComparator{}, param_names);
}
