#include "adjacency_list_graph.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
using namespace std;


void print_all(vector<int> v)
{
  for (int c : v) cout << c << " ";
  cout << endl;
}

vector<int> Intersection(vector<int>& vec1, vector<int>& vec2)
{
  vector<int> intersection;
  set_intersection(vec1.begin(), vec1.end(),
                   vec2.begin(), vec2.end(),
                   inserter(intersection, intersection.end()));
  return intersection;
}


// Test whether vertex v is adjacent to all vertices in S
// Assume that v is not in S
bool is_adjacent_to(AdjacencyListGraph& G, int v, vector<int> S)
{
  bool res = false;
  if (!S.empty())
  {
    vector<int> Sv = Intersection(G.Vertices[v].neighbors, S);
    if (Sv.size() == S.size()) res = true;
  }
  return res;
}


// add vertex v into the clique C
void add_vertex(AdjacencyListGraph& G,
                vector<int>& C,        // Clique
                vector<int>& solution, // solution[v] == 1 if v is in C
                vector<int>& dominated, // num of vertices adjacent to some vertex in C
                int v)
{
  if (solution[v])
  {
    cout << v << " is already in C" << endl;
    exit(1);
  }

  C.push_back(v);
  solution[v] = 1;
  for (int u : G.Vertices[v].neighbors)
  {
    dominated[u]++;
  }
}


// ランダムに1頂点ずつ追加して極大クリークを作る単純な例
vector<int> find_maximal_clique(AdjacencyListGraph& G, std::mt19937_64& mt)
{
  vector<int> C;                // clique
  vector<int> solution(G.number_of_vertices(), 0);
  vector<int> dominated(G.number_of_vertices(), 0);

  // ランダムに1頂点選択してCに追加
  int v = mt() % G.number_of_vertices();

  add_vertex(G, C, solution, dominated, v);
  vector<int> S0 = G.Vertices[v].neighbors;

  while (!S0.empty())
  {
    int i = mt() % S0.size();
    v = S0[i];

    add_vertex(G, C, solution, dominated, v);

    vector<int> Stmp = S0;
    S0.clear();
    for (int u : Stmp)
    {
      if (dominated[u] == (int)C.size()) S0.push_back(u);
    }
    // S0の頂点はCの頂点すべてと隣接しているはず
  }

  sort(C.begin(), C.end());
  return C;
}




int main()
{
  std::random_device rnd;    // 非決定的な乱数生成器
  std::mt19937_64 mt(rnd()); // メルセンヌ・ツイスタ

  // インスタンス
  string filename = "instances/c-fat200-1.clq";

  // ファイル読む
  AdjacencyListGraph G(filename, "dimacs");


  // 使い方の例
  // 頂点数と辺の数
  printf("This graph has %d vertices and %d edges.\n", G.number_of_vertices(), G.number_of_edges());
  // 頂点の次数を表示
  for (int v = 0; v < 10; v++) printf("deg(%d) = %3d\n", v, G.degree(v));

  // 頂点vと隣接する頂点をすべて表示
  int v = 0;
  printf("The neighborhood of vertex %d:", v);
  for (int u : G.Vertices[v].neighbors)
  {
    printf("%d ", u);
  }
  printf("\n");

  // 極大クリークを1個作る
  vector<int> clq = find_maximal_clique(G, mt);
  cout << clq.size() << endl;
  for (int v : clq) cout << v << " ";
  cout << endl;


  return 0;
}
