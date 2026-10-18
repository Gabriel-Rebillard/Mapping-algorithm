#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <memory>

class Vertex
{
private:
    int _Value;
    std::list<Vertex *> _Neighbours;
    std::map<Vertex *, int> _Weights;

public:
    Vertex(int value);
    virtual ~Vertex() = default;

    std::list<Vertex *> *GetNeighbours();
    bool IsNeighbour(Vertex *neighbour);
    bool AddNeighbour(Vertex *neighbour, int weight = 1);
    bool RemoveNeighbour(Vertex *neighbour);

    int GetEdgeWeight(Vertex *neighbour);
    bool SetEdgeWeight(Vertex *neighbour, int weight = 1);

    int GetValue();
    void SetValue(int value);
};

class Graph
{
private:
    std::vector<std::unique_ptr<Vertex>> _Graph;

public:
    Graph(uint16_t size);

    bool IsVertexInGraph(Vertex *vertex);

    std::vector<std::unique_ptr<Vertex>> *GetGraph();

    void AddVertex(int value);
    void AddVertex(std::unique_ptr<Vertex> vertex);
    void AddVertex(unsigned int position, int value);

    bool AddNeighbour(Vertex *vertex1, Vertex *vertex2, int weight = 1, bool oriented = false);
    bool AddNeighbour(unsigned int position1, unsigned int position2, int weight = 1, bool oriented = false);

    bool RemoveVertex(Vertex *vertex);
    bool RemoveVertex(unsigned int position);

    bool RemoveNeighbour(Vertex *vertex1, Vertex *vertex2);
    bool RemoveNeighbour(unsigned int position1, unsigned int position2);

    Vertex *GetVertex(unsigned int position);

    std::list<Vertex *> *GetNeighbours(Vertex *vertex1);
    std::list<Vertex *> *GetNeighbours(unsigned int position);

    int GetEdgeWeight(Vertex *vertex1, Vertex *vertex2);
    int GetEdgeWeight(unsigned int position1, unsigned int position2);

    int GetVertexValue(Vertex *vertex);
    int GetVertexValue(unsigned int position);

    bool SetVertexValue(Vertex *vertex, int value);
    bool SetVertexValue(unsigned int position, int value);

    bool SetEdgeWeight(Vertex *vertex1, Vertex *vertex2, int weight);
    bool SetEdgeWeight(unsigned int position1, unsigned int position2, int weight);
};