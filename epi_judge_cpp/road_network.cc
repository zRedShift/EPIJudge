#include <vector>
#include "test_framework/fmt_print.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct HighwaySection {
  int x, y, distance;
};

HighwaySection FindBestProposals(const vector<HighwaySection> &H, const vector<HighwaySection> &P, const int n) {
  vector<vector<int>> dist(n, vector<int>(n, std::numeric_limits<int>::max()));
  for (const auto &s : H)
    dist[s.x][s.y] = dist[s.y][s.x] = s.distance;
  for (int i = 0; i < n; ++i)
    dist[i][i] = 0;
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
        if (dist[i][k] != std::numeric_limits<int>::max() && dist[j][k] != std::numeric_limits<int>::max())
          dist[i][j] = dist[j][i] = std::min(dist[i][j], dist[i][k] + dist[j][k]);
  int max = 0;
  const HighwaySection *result = &P.front();
  for (const auto &s : P) {
    int c = 0;
    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
        c += std::max(dist[i][j] - s.distance - std::min(dist[s.x][i] + dist[s.y][j], dist[s.y][i] + dist[s.x][j]), 0);
    if (c > max)
      max = c, result = &s;
  }
  return *result;
}
template<>
struct SerializationTraits<HighwaySection>
    : UserSerTraits<HighwaySection, int, int, int> {
};

bool operator==(const HighwaySection &lhs, const HighwaySection &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y && lhs.distance == rhs.distance;
}

std::ostream &operator<<(std::ostream &out, const HighwaySection &hs) {
  return PrintTo(out, std::make_tuple(hs.x, hs.y, hs.distance));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"H", "P", "n"};
  return GenericTestMain(args, "road_network.cc", "road_network.tsv",
                         &FindBestProposals, DefaultComparator{}, param_names);
}
