#pragma once

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <memory>

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

class Vertex
{
private:
    uint8_t _Value;
    std::list<Vertex *> _Neighbours;
    std::map<Vertex *, uint8_t> _Weights;

public:
    Vertex(uint8_t value);
    virtual ~Vertex() = default;

    std::list<Vertex *> *GetNeighbours();
    bool IsNeighbour(Vertex *neighbour);
    bool AddNeighbour(Vertex *neighbour, uint8_t weight = 1);
    bool RemoveNeighbour(Vertex *neighbour);

    uint8_t GetEdgeWeight(Vertex *neighbour);
    bool SetEdgeWeight(Vertex *neighbour, uint8_t weight = 1);

    uint8_t GetValue();
    void SetValue(uint8_t value);
};

class Graph
{
private:
    std::vector<std::unique_ptr<Vertex>> _Graph;

public:
    Graph(uint16_t size);

    bool IsVertexInGraph(Vertex *vertex);

    std::vector<std::unique_ptr<Vertex>> *GetGraph();

    void AddVertex(uint8_t value);
    void AddVertex(std::unique_ptr<Vertex> vertex);
    void AddVertex(uint8_t position, uint8_t type);

    bool AddNeighbour(Vertex *vertex1, Vertex *vertex2, uint8_t weight = 1);
    bool AddNeighbour(uint8_t position1, uint8_t position2, uint8_t weight = 1);

    bool RemoveVertex(Vertex *vertex);
    bool RemoveVertex(uint8_t position);

    bool RemoveNeighbour(Vertex *vertex1, Vertex *vertex2);
    bool RemoveNeighbour(uint8_t position1, uint8_t position2);

    Vertex *GetVertex(uint8_t position);

    std::list<Vertex *> *GetNeighbours(Vertex *vertex1);
    std::list<Vertex *> *GetNeighbours(uint8_t position);

    uint8_t GetEdgeWeight(Vertex *vertex1, Vertex *vertex2);
    uint8_t GetEdgeWeight(uint8_t position1, uint8_t position2);

    bool SetVertexValue(Vertex *vertex, uint8_t value);
    bool SetVertexValue(uint8_t position, uint8_t value);

    bool SetEdgeWeight(Vertex *vertex1, Vertex *vertex2, uint8_t weight);
    bool SetEdgeWeight(uint8_t position1, uint8_t position2, uint8_t weight);
};