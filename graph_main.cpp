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


// find a maximal clique by choosing vertices at random
vector<int> find_maximal_clique(AdjacencyListGraph& G, std::mt19937_64& mt)
{
  int v = mt() % G.number_of_vertices();
  
  cout << v << endl;
  vector<int> C = {v};
  vector<int> S0 = G.Vertices[v].neighbors;

  while (!S0.empty())
  {
    int i = mt() % S0.size();
    int u = S0[i];
    C.push_back(u);

    S0 = Intersection(G.Vertices[u].neighbors, S0);
  }

  sort(C.begin(), C.end());
  return C;
}

// dominated[v] = num of vertices in C that adjacacent to v
vector<int> dominated;
vector<int> solution;

void add_vertex(AdjacencyListGraph& G, vector<int>& C, int v)
{
  if (solution[v])
  {
    cout << v << " is already in C" << endl;
    exit(1);
  }

  solution[v] = 1;
  for (int u : G.Vertices[v].neighbors)
  {
    dominated[u]++;
  }
}

// find a maximal clique by choosing vertices at random
vector<int> simple_local_search(AdjacencyListGraph& G, std::mt19937_64& mt)
{
  for (int i = 0; i < G.number_of_vertices(); i++) dominated.push_back(0);
  for (int i = 0; i < G.number_of_vertices(); i++) solution.push_back(0);
  
  // clique
  vector<int> C;


  

  vector<int> S0, S1;

  // for two vertices in G
  int u = mt() % G.number_of_vertices();
  S0 = G.Vertices[u].neighbors;
  int v = S0[mt() % S0.size()];


  S0.clear();

  vector<int>::iterator iu, iv;
  iu = G.Vertices[u].neighbors.begin();
  iv = G.Vertices[v].neighbors.begin();
  while (iu != G.Vertices[u].neighbors.end() && 
         iv != G.Vertices[v].neighbors.end())
  {
    if (*iu == v) iu++;
    else if (*iv == u) iv++;
    else if (iu == G.Vertices[u].neighbors.end())
    {
      S1.push_back(*iv);
      iv++;
    }
    else if (iv == G.Vertices[v].neighbors.end())
    {
      S1.push_back(*iu);
      iu++;
    }
    else if (*iu == *iv)
    {
      S0.push_back(*iu);
      iu++;
      iv++;
    }
    else if (*iu < *iv)
    {
      S1.push_back(*iu);
      iu++;
    }
    else if (*iv < *iu)
    {
      S1.push_back(*iv);
      iv++;
    }
  }

  while (!S0.empty())
  {
    v = S0[mt() % S0.size()];
    C.push_back(v);
    S0 = Intersection(G.Vertices[v].neighbors, S0);
    S1 = Intersection(G.Vertices[v].neighbors, S1);
  }

  sort(C.begin(), C.end());
  return C;
}





int main()
{
  std::random_device rnd;    // 非決定的な乱数生成器
  std::mt19937_64 mt(rnd()); // メルセンヌ・ツイスタ
  


  
  string filename = "instances/c-fat200-1.clq";
  vector<string> lines;

  AdjacencyListGraph G(filename, "dimacs");

  //vector<int> clq = find_maximal_clique(G, mt);
  vector<int> clq = simple_local_search(G, mt);

  cout << clq.size() << endl;
  for (int v : clq) cout << v << " ";
  cout << endl;
  // int v = 0;
  // for (auto u : G.Vertices[v].neighbors)
  // {
  //   cout << u << " ";
  // }
  // cout << endl;

  // vector<int> S1 = {1, 36, 37, 38, 73, 148};
  // vector<int> S2 = {1, 36, 37, 38, 73, 100};


  return 0;
}
