#include <vector>
#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct Rect {
  int left, right, height;
  Rect(int left, int right, int height) : left(left), right(right), height(height) {}
};
typedef vector<Rect> Skyline;

Skyline ComputeSkyline(const Skyline &buildings) {
  int left = std::numeric_limits<int>::max(), right = std::numeric_limits<int>::min();
  for (const auto &s: buildings)
    left = std::min(left, s.left), right = std::max(right, s.right);
  vector<int> heights(right - left + 1);
  for (const auto &s: buildings) {
    int end = s.right - left + 1;
    for (int i = s.left - left; i < end; ++i)
      heights[i] = std::max(heights[i], s.height);
  }
  Skyline result;
  for (int i = 0; i < heights.size();) {
    Rect &rect = result.emplace_back(i + left, 0, heights[i]);
    while (++i < heights.size() && heights[i] == rect.height) continue;
    rect.right = i + left - 1;
  }
  return result;
}
bool operator==(const Rect &a, const Rect &b) {
  return a.left == b.left && a.right == b.right && a.height == b.height;
}

template<>
struct SerializationTraits<Rect> : UserSerTraits<Rect, int, int, int> {};

std::ostream &operator<<(std::ostream &out, const Rect &r) {
  return PrintTo(out, std::make_tuple(r.left, r.right, r.height));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"buildings"};
  return GenericTestMain(args, "drawing_skyline.cc", "drawing_skyline.tsv",
                         &ComputeSkyline, DefaultComparator{}, param_names);
}
