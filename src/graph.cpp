#include "../includes/graph.h"

#ifndef INFINITY
#define INFINITY 0xFF
#endif

// Graph definition *********************************************************************
Graph::Graph(uint16_t size)
{
    _Graph.resize(size);
    for (uint16_t i = 0; i < size; i++)
        _Graph.at(i) = std::unique_ptr<Vertex>(new Vertex(0));
}

bool Graph::IsVertexInGraph(Vertex *vertex)
{
    if (std::find_if(_Graph.begin(), _Graph.end(), [vertex](const std::unique_ptr<Vertex> &ptr)
                     { return ptr.get() == vertex; }) == _Graph.end())
        return 0;
    return 1;
}

std::vector<std::unique_ptr<Vertex>> *Graph::GetGraph()
{
    return &_Graph;
}

void Graph::AddVertex(int value)
{
    _Graph.push_back(std::unique_ptr<Vertex>(new Vertex(value)));
}
void Graph::AddVertex(std::unique_ptr<Vertex> vertex)
{
    _Graph.push_back(std::move(vertex));
}
void Graph::AddVertex(unsigned int position, int value)
{
    if (position >= _Graph.size())
        _Graph.push_back(std::unique_ptr<Vertex>(new Vertex(value)));
    else
        _Graph.at(position) = std::unique_ptr<Vertex>(new Vertex(value));
}

bool Graph::AddNeighbour(Vertex *vertex1, Vertex *vertex2, int weight, bool oriented)
{
    if (!IsVertexInGraph(vertex1) || !IsVertexInGraph(vertex2))
        return 0;

    vertex1->AddNeighbour(vertex2);
    vertex1->SetEdgeWeight(vertex2, weight);
    if (!oriented)
    {
        vertex2->AddNeighbour(vertex1);
        vertex2->SetEdgeWeight(vertex1, weight);
    }
    return 1;
}
bool Graph::AddNeighbour(unsigned int position1, unsigned int position2, int weight, bool oriented)
{
    if (position1 >= _Graph.size() || position2 >= _Graph.size())
        return 0;
    AddNeighbour(_Graph.at(position1).get(), _Graph.at(position2).get(), weight);
    return 1;
}

bool Graph::RemoveVertex(Vertex *vertex)
{
    if (!IsVertexInGraph(vertex))
        return 0;
    _Graph.erase(std::find_if(_Graph.begin(), _Graph.end(), [vertex](std::unique_ptr<Vertex> &ptr)
                              { return ptr.get() == vertex; }));
    return 1;
}
bool Graph::RemoveVertex(unsigned int position)
{
    if (position >= _Graph.size())
        return 0;
    RemoveVertex(_Graph.at(position).get());
    return 1;
}

bool Graph::RemoveNeighbour(Vertex *vertex1, Vertex *vertex2)
{
    if (!IsVertexInGraph(vertex1) || !IsVertexInGraph(vertex2))
        return 0;
    vertex1->RemoveNeighbour(vertex2);
    vertex2->RemoveNeighbour(vertex1);
    return 1;
}
bool Graph::RemoveNeighbour(unsigned int position1, unsigned int position2)
{
    if (position1 >= _Graph.size() || position2 >= _Graph.size())
        return 0;
    RemoveNeighbour(_Graph.at(position1).get(), _Graph.at(position2).get());
    return 1;
}

Vertex *Graph::GetVertex(unsigned int position)
{
    if (position >= _Graph.size())
        return nullptr;
    return _Graph.at(position).get();
}

std::list<Vertex *> *Graph::GetNeighbours(Vertex *vertex)
{
    if (!IsVertexInGraph(vertex))
        return nullptr;
    return vertex->GetNeighbours();
}
std::list<Vertex *> *Graph::GetNeighbours(unsigned int position)
{
    if (position >= _Graph.size())
        return nullptr;
    return GetNeighbours(_Graph.at(position).get());
}

int Graph::GetEdgeWeight(Vertex *vertex1, Vertex *vertex2)
{
    if (!IsVertexInGraph(vertex1) || !IsVertexInGraph(vertex2))
        return INFINITY;
    return vertex1->GetEdgeWeight(vertex2);
}
int Graph::GetEdgeWeight(unsigned int position1, unsigned int position2)
{
    if (position1 >= _Graph.size() || position2 >= _Graph.size())
        return INFINITY;
    return GetEdgeWeight(_Graph.at(position1).get(), _Graph.at(position2).get());
}

int Graph::GetVertexValue(Vertex *vertex)
{
    if (!IsVertexInGraph(vertex))
        return 0;
    return vertex->GetValue();
}
int Graph::GetVertexValue(unsigned int position)
{
    if (position >= _Graph.size())
        return 0;
    return GetVertexValue(_Graph.at(position).get());
}

bool Graph::SetVertexValue(Vertex *vertex, int value)
{
    if (!IsVertexInGraph(vertex))
        return 0;
    vertex->SetValue(value);
    return 1;
}
bool Graph::SetVertexValue(unsigned int position, int value)
{
    if (position >= _Graph.size())
        return 0;
    SetVertexValue(_Graph.at(position).get(), value);
    return 1;
}

bool Graph::SetEdgeWeight(Vertex *vertex1, Vertex *vertex2, int weight)
{
    if (!IsVertexInGraph(vertex1) || !IsVertexInGraph(vertex2))
        return 0;
    vertex1->SetEdgeWeight(vertex2, weight);
    vertex2->SetEdgeWeight(vertex1, weight);
    return 1;
}
bool Graph::SetEdgeWeight(unsigned int position1, unsigned int position2, int weight)
{
    if (position1 >= _Graph.size() || position2 >= _Graph.size())
        return 0;
    SetEdgeWeight(_Graph.at(position1).get(), _Graph.at(position2).get(), weight);
    return 1;
}

// Vertex definition *********************************************************************
Vertex::Vertex(int value = 0)
{
    _Value = value;
}

std::list<Vertex *> *Vertex::GetNeighbours()
{
    return &_Neighbours;
}
bool Vertex::IsNeighbour(Vertex *neighbour)
{
    if (std::find(_Neighbours.begin(), _Neighbours.end(), neighbour) == _Neighbours.end())
        return 0;
    return 1;
}
bool Vertex::AddNeighbour(Vertex *neighbour, int weight)
{
    if (IsNeighbour(neighbour))
        return 0;
    _Neighbours.push_back(neighbour);
    _Weights[neighbour] = weight;
    return 1;
}
bool Vertex::RemoveNeighbour(Vertex *neighbour)
{
    if (!IsNeighbour(neighbour))
        return 0;
    _Neighbours.remove(neighbour);
    _Weights.erase(neighbour);
    return 1;
}

int Vertex::GetEdgeWeight(Vertex *neighbour)
{
    if (!IsNeighbour(neighbour))
        return INFINITY;
    return _Weights.at(neighbour);
}
bool Vertex::SetEdgeWeight(Vertex *neighbour, int weight)
{
    if (!IsNeighbour(neighbour))
        return 0;
    _Weights[neighbour] = weight;
    return 1;
}

int Vertex::GetValue()
{
    return _Value;
}
void Vertex::SetValue(int value)
{
    _Value = value;
}