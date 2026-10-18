#include "../includes/map.h"

// Case definition *********************************************************************
Case::Case(uint8_t type) : Vertex(type)
{
}

uint8_t Case::GetVisitedCount()
{
    return _VisitedCount;
}

// Map definition *********************************************************************
Map::Map(uint8_t width, uint8_t height) : _Graph(width * height)
{
    _Width = width;
    _Height = height;

    for (uint8_t i = 0; i < _Height; i++)
        for (uint8_t j = 0; j < _Width; j++)
            SetCaseType(j, i, ROOM_ID);
}
Map::~Map() {}

const Graph &Map::GetMap()
{
    return _Graph;
}

void Map::ExtendMap(uint8_t dir)
{
    switch (dir)
    {
    case UP:
        for (uint8_t i = 0; i < _Width; i++)
            _Graph.GetGraph()->insert(_Graph.GetGraph()->begin(), std::make_unique<Case>(UNKNOWN_ID));
        _Height++;
        break;
    case DOWN:
        for (uint8_t i = 0; i < _Width; i++)
            _Graph.GetGraph()->push_back(std::make_unique<Case>(UNKNOWN_ID));
        _Height++;
        break;

    case LEFT:
        for (uint8_t i = 0; i < _Height; i++)
            _Graph.GetGraph()->insert(_Graph.GetGraph()->begin() + i * (_Width + 1), std::make_unique<Case>(UNKNOWN_ID));
        _Width++;
        break;

    case RIGHT:
        for (uint8_t i = 0; i < _Height; i++)
            _Graph.GetGraph()->insert(_Graph.GetGraph()->begin() + i * (_Width + 1) + _Width, std::make_unique<Case>(UNKNOWN_ID));
        _Width++;
        break;
    }
}

uint8_t Map::GetValue(uint8_t x, uint8_t y)
{
    return _Graph.GetVertexValue(y * _Width + x);
}
uint8_t Map::GetValue(uint8_t *position)
{
    return GetValue(position[0], position[1]);
}

Case *Map::GetCase(uint8_t x, uint8_t y)
{
    return vToC(_Graph.GetVertex(y * _Width + x));
}
Case *Map::GetCase(uint8_t *position)
{
    return GetCase(position[0], position[1]);
}

bool Map::SetCaseType(uint8_t x, uint8_t y, uint8_t type)
{
    if (_Graph.GetVertex(y * _Width + x) == nullptr)
        return 0;
    _Graph.GetVertex(y * _Width + x)->SetValue(type);
    return 1;
}
bool Map::SetCaseType(uint8_t *position, uint8_t type)
{
    return SetCaseType(position[0], position[1], type);
}

bool Map::AddNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight)
{
    if (x1 == x2 && y1 == y2)
        return 0;
    return _Graph.AddNeighbour(y1 * _Width + x1, y2 * _Width + x2, weight);
}
bool Map::AddNeighbour(uint8_t *position1, uint8_t *position2, uint8_t weight)
{
    return AddNeighbour(position1[0], position1[1], position2[0], position2[1], weight);
}

bool Map::RemoveNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    return _Graph.RemoveNeighbour(y1 * _Width + x1, y2 * _Width + x2);
}
bool Map::RemoveNeighbour(uint8_t *position1, uint8_t *position2)
{
    return RemoveNeighbour(position1[0], position1[1], position2[0], position2[1]);
}

bool Map::ModifyDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight)
{
    return _Graph.SetEdgeWeight(y1 * _Width + x1, y2 * _Width + x2, weight);
}
bool Map::ModifyDistance(uint8_t *position1, uint8_t *position2, uint8_t weight)
{
    return ModifyDistance(position1[0], position1[1], position2[0], position2[1], weight);
}

std::unique_ptr<std::list<Case *>> Map::GetNeighbours(uint8_t x, uint8_t y)
{
    std::list<Vertex *> *neighbours = _Graph.GetNeighbours(y * _Width + x);
    std::unique_ptr<std::list<Case *>> cases = std::make_unique<std::list<Case *>>();

    for (auto it = neighbours->begin(); it != neighbours->end(); it++)
        cases->push_back(vToC(*it));

    return cases;
}
std::unique_ptr<std::list<Case *>> Map::GetNeighbours(uint8_t *position)
{
    return GetNeighbours(position[0], position[1]);
}

uint8_t Map::GetDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    return _Graph.GetEdgeWeight(y1 * _Width + x1, y2 * _Width + x2);
}
uint8_t Map::GetDistance(uint8_t *position1, uint8_t *position2)
{
    return GetDistance(position1[0], position1[1], position2[0], position2[1]);
}

uint8_t Map::GetCaseType(uint8_t x, uint8_t y)
{
    return _Graph.GetVertexValue(y * _Width + x);
}
uint8_t Map::GetCaseType(uint8_t *position)
{
    return GetCaseType(position[0], position[1]);
}

uint8_t Map::GetWidth()
{
    return _Width;
}
uint8_t Map::GetHeight()
{
    return _Height;
}

uint8_t Map::GetCasePositionX(Case *c)
{
    if (c == nullptr)
        return INFINITY;
    // if (!_Graph.IsVertexInGraph(c))
    // return INFINITY;
    return std::distance(_Graph.GetGraph()->begin(), std::find_if(_Graph.GetGraph()->begin(), _Graph.GetGraph()->end(), [c](const std::unique_ptr<Vertex> &ptr)
                                                                  { return ptr.get() == c; })) %
           (_Width);
}
uint8_t Map::GetCasePositionY(Case *c)
{
    if (c == nullptr)
        return INFINITY;
    if (!_Graph.IsVertexInGraph(c))
        return INFINITY;
    return std::distance(_Graph.GetGraph()->begin(), std::find_if(_Graph.GetGraph()->begin(), _Graph.GetGraph()->end(), [c](const std::unique_ptr<Vertex> &ptr)
                                                                  { return ptr.get() == c; })) /
           (_Width);
}
uint8_t *Map::GetCasePosition(Case *c)
{
    uint8_t *position = new uint8_t[2];
    position[0] = GetCasePositionX(c);
    position[1] = GetCasePositionY(c);
    return position;
}

std::unique_ptr<uint8_t[]> Map::GetMap_Matrixe()
{
    auto matrix = std::make_unique<uint8_t[]>(_Height * _Width);

    for (uint8_t i = 0; i < _Height; i++)
        for (uint8_t j = 0; j < _Width; j++)
            matrix.get()[i * _Width + j] = _Graph.GetVertexValue(i * _Width + j);
    return matrix;
}