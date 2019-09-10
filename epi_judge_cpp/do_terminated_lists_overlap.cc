#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::shared_ptr;

void Length(shared_ptr<ListNode<int>> &l, int &i) {
  for (i = 0; l->next; ++i, l = l->next);
}
void Advance(shared_ptr<ListNode<int>> &l, int i) {
  while (i--)
    l = l->next;
}
shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
  if (!l0 || !l1)
    return nullptr;
  auto m0 = l0, m1 = l1;
  int i, j;
  Length(l0, i), Length(l1, j);
  if (l0 != l1)
    return nullptr;
  i < j ? Advance(m1, j - i) : Advance(m0, i - j);
  while ((m0 = m0->next) != (m1 = m1->next));
  return m0;
}
void OverlappingNoCycleListsWrapper(TimedExecutor &executor,
                                    shared_ptr<ListNode<int>> l0,
                                    shared_ptr<ListNode<int>> l1,
                                    shared_ptr<ListNode<int>> common) {
  if (common) {
    if (l0) {
      auto i = l0;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l0 = common;
    }

    if (l1) {
      auto i = l1;
      while (i->next) {
        i = i->next;
      }
      i->next = common;
    } else {
      l1 = common;
    }
  }

  auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

  if (result != common) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1", "common"};
  return GenericTestMain(
      args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
      &OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
