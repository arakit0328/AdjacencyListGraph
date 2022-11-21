#pragma once

#include <vector>
#include <string>

class AdjacencyListGraph
{
 public:
  /**
   * Structure that stores the information for an edge incident to a vertex.
   */
  struct VertexEdge
  {
    /** Id of the edge. */
    int e;
    /** Id of the neighbor. */
    int v;
  };


  /**
   * Structure that stores the information for a vertex.
   */
  struct Vertex
  {
    /** Unique id of the vertex. */
    int id;
    /** Weight of the vertex. */
    int weight = 1;
    /** Id of the connected component of the vertex. */
    int component = -1;
    /** Neighbors of the vertex. */
    std::vector<VertexEdge> edges;
    /** Neighbors of the vertex. */
    std::vector<int> neighbors;
  };

  /**
   * Structure that stores the information for an edge.
   */
  struct Edge
  {
    /** Unique id of the edge. */
    int id;
    /** First end of the edge. */
    int v1;
    /** Second end of the edge. */
    int v2;
  };

  /*
   * Constructors destructor.
   */

  /** Create a graph from a file. */
  AdjacencyListGraph(std::string instance_path, std::string format);

  /** Constructor. */
  AdjacencyListGraph(int number_of_vertices_ = 0);

  /*
   * Methods
   */
  
  /** Add a vertex. */
  int add_vertex(int weight = 1);

  /** Set the weight of vertex 'v' to 'weight'. */
  void set_weight(int v, int weight);

  /** Add an edge. */
  int add_edge(int v1, int v2);

  /** Clear graph, i.e. remove vertices and edges. */
  void clear();

  /** Clear the edges of the graph. */
  void clear_edges();

  /*
   * Getters.
   */

  int number_of_vertices() const { return Vertices.size(); }

  int number_of_edges() const { return number_of_edges_; }

  int degree(int v) { return Vertices[v].edges.size(); }

  int maximum_degree() const { return maximum_degree_; }

  int weight(int v) { return Vertices[v].weight; }

  int total_weight() { return total_weight_; };

  /** Get the first end of edge 'e'. */
  inline int first_end(int e) const { return Edges[e].v1; }

  /** Get the second end of edge 'e'.  */
  inline int second_end(int e) const { return Edges[e].v2; }

  /** Get the end of edge 'e' which is not 'v'. */
  inline int other_end(int e, int v) const
  {
    return (v == Edges[e].v1)? Edges[e].v2: Edges[e].v1;
  }

  /** The the list of edges incident to vertex 'v'. */
  inline const std::vector<VertexEdge>& edges(int v) const { return Vertices[v].edges; }

  // private:

  /*
   * Private attributes.
   */

  /** Vertices. */
  std::vector<Vertex> Vertices;

  /** Edges. */
  std::vector<Edge> Edges;

  /** Number of edges. */
  int number_of_edges_ = 0;

  /** Maximum degree. */
  int maximum_degree_ = 0;

  /** Total weight. */
  int total_weight_ = 0;

  /*
   * Private methods.
   */

  // Split a given string by delim
  std::vector<std::string> split(const std::string &s, char delim);

  /** Read a graph in 'dimacs1992' format. */
  void read_dimacs1992(std::ifstream& file);
};
