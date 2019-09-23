#include <memory>
#include "list_node.h"
#define main _main
#include "sorted_lists_merge.cc"
#undef main
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> StableSortList(shared_ptr<ListNode<int>> L) {
  if (!L || !L->next)
    return L;
  auto slow = L, fast = L;
  while (fast->next && fast->next->next)
    slow = slow->next, fast = fast->next->next;
  fast = slow->next, slow->next = nullptr;
  return MergeTwoSortedLists(StableSortList(L), StableSortList(fast));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "sort_list.cc", "sort_list.tsv", &StableSortList,
                         DefaultComparator{}, param_names);
}
