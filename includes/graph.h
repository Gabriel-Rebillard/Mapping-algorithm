#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <memory>

// Vertex definition *********************************************************************
class Vertex
{
private:
    int _Value;                       // Vertex value
    std::list<Vertex *> _Neighbours;  // Vertex neighbours
    std::map<Vertex *, int> _Weights; // Weights of the edges between the vertex and its neighbours

public:
    Vertex(int value);
    virtual ~Vertex() = default;

    // Interface with neighbours
    std::list<Vertex *> *GetNeighbours();                 // Returns the list of neighbours
    bool IsNeighbour(Vertex *neighbour);                  // Returns true if the vertex is a neighbour
    bool AddNeighbour(Vertex *neighbour, int weight = 1); // Adds a neighbour, with an optional weight on the edge
    bool RemoveNeighbour(Vertex *neighbour);              // Removes a neighbour

    // Interface with an edge
    int GetEdgeWeight(Vertex *neighbour);                  // Returns the weight of the edge connecting the vertex and the neighbour
    bool SetEdgeWeight(Vertex *neighbour, int weight = 1); // Sets the weight of the edge connecting the vertex and the neighbour

    // Interface with the vertex value
    int GetValue();
    void SetValue(int value);
};

// Graph definition *********************************************************************
class Graph
{
private:
    std::vector<std::unique_ptr<Vertex>> _Graph; // List of vertices

public:
    Graph(uint16_t size);

    std::vector<std::unique_ptr<Vertex>> *GetGraph();

    // Vertex manipulation interface
    void AddVertex(int value);                             // Adds a vertex to the graph                                                           
    void AddVertex(std::unique_ptr<Vertex> vertex);       
    void AddVertex(unsigned int position, int value);     

    bool RemoveVertex(Vertex *vertex);                     // Removes a vertex from the graph, returns false if the vertex does not exist          
    bool RemoveVertex(unsigned int position);             

    Vertex *GetVertex(unsigned int position);              // Returns the vertex at the given position, return nullptr if the vertex does not exist
    int GetVertexValue(Vertex *vertex);                    // Returns the value of the vertex, return NULL if the vertex does not exist            
    int GetVertexValue(unsigned int position);            

    bool SetVertexValue(Vertex *vertex, int value);        // Sets the value of the given vertex                                                   
    bool SetVertexValue(unsigned int position, int value);

    bool IsVertexInGraph(Vertex *vertex);                  // Returns true if the vertex is in the graph

    // Neighbouroud manipulation interface
    bool AddNeighbour(Vertex *vertex1, Vertex *vertex2, int weight = 1, bool oriented = false);               // Links two vertices with an edge, optionally weighted and/or oriented
    bool AddNeighbour(unsigned int position1, unsigned int position2, int weight = 1, bool oriented = false);

    bool RemoveNeighbour(Vertex *vertex1, Vertex *vertex2);                                                   // Unlinks two vertices                                                
    bool RemoveNeighbour(unsigned int position1, unsigned int position2);                                    

    std::list<Vertex *> *GetNeighbours(Vertex *vertex1);                                                      // Returns the list of neighbours of the given vertex                  
    std::list<Vertex *> *GetNeighbours(unsigned int position);

    // Edge manipulation interface
    int GetEdgeWeight(Vertex *vertex1, Vertex *vertex2);                            // Returns the weight of the edge connecting the two vertices
    int GetEdgeWeight(unsigned int position1, unsigned int position2);             

    bool SetEdgeWeight(Vertex *vertex1, Vertex *vertex2, int weight);               // Sets the weight of the edge connecting the two vertices   
    bool SetEdgeWeight(unsigned int position1, unsigned int position2, int weight);
};