#include "adjacency_list_graph.hpp"
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

AdjacencyListGraph::AdjacencyListGraph(int number_of_vertices)
{
  for (int v = 0; v < number_of_vertices; ++v)
    add_vertex();
}


AdjacencyListGraph::AdjacencyListGraph(
    std::string instance_path,
    std::string format)
{
  std::ifstream file(instance_path);
  if (!file.good())
    throw std::runtime_error(
        "Unable to open file \"" + instance_path + "\".");

  if (format == "dimacs" || format == "dimacs1992") {
    read_dimacs1992(file);
  }
}

// Add a new vertex and returns the id of the vertex
int AdjacencyListGraph::add_vertex(int weight)
{
  Vertex vertex;
  vertex.id = Vertices.size();
  vertex.weight = weight;
  Vertices.push_back(vertex);
  total_weight_ += weight;
  return vertex.id;
}


// Assing the weight to vertex v
void AdjacencyListGraph::set_weight(int v, int weight)
{
  total_weight_ -= Vertices[v].weight;
  Vertices[v].weight = weight;
  total_weight_ += Vertices[v].weight;
};


int AdjacencyListGraph::add_edge(int v1, int v2)
{
  if (v1 == v2) {
    return -1;
  }

  Edge edge;
  edge.id = Edges.size();
  edge.v1 = v1;
  edge.v2 = v2;
  Edges.push_back(edge);

  VertexEdge ve1;
  ve1.e = edge.id;
  ve1.v = v2;
  Vertices[v1].edges.push_back(ve1);
  Vertices[v1].neighbors.push_back(v2);

  VertexEdge ve2;
  ve2.e = edge.id;
  ve2.v = v1;
  Vertices[v2].edges.push_back(ve2);
  Vertices[v2].neighbors.push_back(v1);

  number_of_edges_++;
  if (maximum_degree_ < std::max(degree(v1), degree(v2)))
    maximum_degree_ = std::max(degree(v1), degree(v2));

  return edge.id;
}

void AdjacencyListGraph::clear()
{
  Vertices.clear();
  Edges.clear();
  number_of_edges_ = 0;
  maximum_degree_ = 0;
  total_weight_ = 0;
}

void AdjacencyListGraph::clear_edges()
{
  Edges.clear();
  maximum_degree_ = 0;
  number_of_edges_ = 0;
  for (int v = 0; v < number_of_vertices(); ++v)
  {
    Vertices[v].edges.clear();
    Vertices[v].neighbors.clear();
  }
}


void AdjacencyListGraph::read_dimacs1992(std::ifstream& file)
{
  std::string tmp;
  std::vector<std::string> line;

  while (getline(file, tmp)) {
    line = split(tmp, ' ');
    if (line.size() == 0)
    { /* do nothing */ }
    else if (line[0] == "c")
    { /* do nothing */ }
    else if (line[0] == "p")
    {
      int number_of_vertices = stoi(line[2]);
      for (int v = 0; v < number_of_vertices; ++v) add_vertex();
    }
    else if (line[0] == "n") {
      int v = stoi(line[1]) - 1;
      int w = stoi(line[2]);
      set_weight(v, w);
    }
    else if (line[0] == "e")
    {
      int v1 = stoi(line[1]) - 1;
      int v2 = stoi(line[2]) - 1;
      add_edge(v1, v2);
    }
  }
}



// Split a given string by delim
std::vector<std::string> AdjacencyListGraph::split(const std::string &s, char delim)
{
  std::vector<std::string> elems;
  std::string item;

  for (char ch: s)
  {
    if (ch == delim)
    {
      if (!item.empty())
        elems.push_back(item);
      item.clear();
    }
    else
    {
      item += ch;
    }
  }

  if (!item.empty())
    elems.push_back(item);
  return elems;
}
