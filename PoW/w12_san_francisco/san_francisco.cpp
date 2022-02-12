#include <iostream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

long moves(int from, int moves_left, const vector<set<int>> &connections, const vector<vector<long>> &points, vector<vector<long>> &memo) {
  if (memo[from][moves_left] == -1) {
    long maxPoints = 0;
    for (auto next : connections[from]) {
      maxPoints = max(maxPoints, 
              points[from][next] + moves(next, moves_left - 1, connections, points, memo));
    }
    memo[from][moves_left] = maxPoints;
  }
  return memo[from][moves_left];
}

void testcase(int t) {
  int nr_holes, nr_canals, nr_moves; long target_score;
  cin >> nr_holes >> nr_canals >> target_score >> nr_moves;
  
  vector<set<int>> connections(nr_holes);
  vector<vector<long>> points(nr_holes, vector<long>(nr_holes, 0));
  for (int i = 0; i < nr_canals; ++i) {
    int u, v; long p;
    cin >> u >> v >> p;
    connections[u].insert(v);
    points[u][v] = max(points[u][v], p);
  }
  
  // Find synonyms for 0
  vector<bool> end(nr_holes, false);
  for (int i = 0; i < nr_holes; ++i) {
    if (connections[i].size() == 0) end[i] = true;
  }
  
  vector<set<int>> newConnections(nr_holes);
  vector<vector<long>> newPoints(nr_holes, vector<long>(nr_holes, 0));
  for (int i = 0; i < nr_holes; ++i) {
    for (auto el : connections[i]) {
      if (end[el]) {
        newConnections[i].insert(0);
        newPoints[i][0] = max(newPoints[i][0], points[i][el]);
      } else {
        newConnections[i].insert(el);
        newPoints[i][el] = points[i][el];
      }
    }
  }

  vector<vector<long>> memo(nr_holes, vector<long>(nr_moves + 1, -1));
  
  for (int i = 0; i < nr_holes; ++i) {
    memo[i][0] = 0;
  }
  
  for (int i = 0; i <= nr_moves; ++i) {
    long possible = moves(0, i, newConnections, newPoints, memo);
    if (possible >= target_score) {
      cout << i << endl;
      return;
    }
  }

  cout << "Impossible" << endl;
  
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) {
    testcase(t);
  }
}