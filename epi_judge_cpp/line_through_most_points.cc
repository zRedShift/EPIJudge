#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
using std::isinf;
using std::fabs;

struct Point {
  int x, y;
  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }
};
namespace std {
template<>
struct hash<Point> {
  size_t operator()(const Point &p) const {
    return (static_cast<size_t>(p.x) << 8 * sizeof(int)) + p.y;
  }
};
}

Point Slope(const Point &a, const Point &b) {
  Point p{b.x - a.x, b.y - a.y};
  if (p.x) {
    int gcd = std::gcd(p.x, p.y);
    p.y /= p.x < 0 ? -gcd : gcd, p.x = std::abs(p.x / gcd);
  } else
    p.x = -1, p.y = a.x;
  return p;
}

int FindLineWithMostPoints(const vector<Point> &points_vec) {
  std::unordered_map<Point, int> points, lines;
  for (const auto &point : points_vec)
    ++points[point];
  if (points.size() < 3)
    return points_vec.size();
  int result = 0;
  for (auto p = points.begin(), q = std::next(p); q != points.end(); p = q++, lines.clear())
    for (auto iter = q; iter != points.end(); ++iter)
      result = std::max(result, lines.emplace(Slope(p->first, iter->first), p->second).first->second += iter->second);
  return result;
}
template<>
struct SerializationTraits<Point> : UserSerTraits<Point, int, int> {};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"points"};
  return GenericTestMain(
      args, "line_through_most_points.cc", "line_through_most_points.tsv",
      &FindLineWithMostPoints, DefaultComparator{}, param_names);
}
