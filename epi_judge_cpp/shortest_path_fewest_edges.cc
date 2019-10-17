#include <stdexcept>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
using std::vector;
using std::numeric_limits;

struct GraphVertex;
struct Path {
  int distance = numeric_limits<int>::max(), length = 0;
  bool operator<(const Path &rhs) const {
    return distance < rhs.distance || (distance == rhs.distance && length < rhs.length);
  }
  bool operator==(const Path &rhs) const {
    return distance == rhs.distance && length == rhs.length;
  }
};
struct GraphVertex {
  vector<std::pair<GraphVertex *, int>> edges;
  Path path;
  GraphVertex *pred = nullptr;
};
struct VertexComp {
  bool operator()(const GraphVertex *const &lhs, const GraphVertex *const &rhs) const {
    return lhs->path < rhs->path || (lhs->path == rhs->path && lhs < rhs);
  }
};

vector<GraphVertex *> ShortestPathFewestEdges(GraphVertex *const origin, GraphVertex *const destination) {
  origin->path = {0, 0};
  std::set<GraphVertex *, VertexComp> q{origin};
  do {
    auto curr = *q.cbegin();
    if (curr == destination)
      break;
    q.erase(q.cbegin());
    for (const auto[vertex, distance] : curr->edges)
      if (Path alt{curr->path.distance + distance, curr->path.length + 1}; alt < vertex->path)
        q.erase(vertex), vertex->path = alt, vertex->pred = curr, q.insert(vertex);
  } while (!q.empty());
  auto curr = destination;
  vector<GraphVertex *> result(curr->pred ? curr->path.length + 1 : 0);
  for (auto p = result.rbegin(); p != result.rend(); ++p)
    *p = curr, curr = curr->pred;
  return result;
}

struct Edge {
  int from, to, distance;
};
template<>
struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int, int> {};

vector<int> ShortestPathFewestEdgesWrapper(TimedExecutor &executor, int k, int from, int to,
                                           const vector<Edge> &edges) {
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }

  vector<GraphVertex> graph(k, GraphVertex{});

  for (const Edge &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k || e.distance <= 0) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.emplace_back(&graph[e.to], e.distance);
  }
  auto vec = executor.Run([&] { return ShortestPathFewestEdges(&graph[from], &graph[to]); });
  vector<int> result(vec.size());
  std::transform(vec.begin(), vec.end(), result.begin(), [&](const auto &v) { return v - graph.data(); });
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "from", "to", "edges"};
  return GenericTestMain(
      args, "shortest_path_fewest_edges.cc", "shortest_path_fewest_edges.tsv",
      &ShortestPathFewestEdgesWrapper, DefaultComparator{}, param_names);
}
