#pragma once

#include "cfg.h"
#include "graph.h"

// Converting vertex to case
#define vToC(v) static_cast<Case *>(v)


// Case herits from vertex to be able to store it in a graph
class Case : public Vertex
{
private:
    uint8_t _VisitedCount = 0;

public:
    Case(uint8_t type = NaN);
    uint8_t GetVisitedCount();
};

class Map
{
private:
    Graph _Graph;
    uint8_t _Width;
    uint8_t _Height;

public:
    Map(uint8_t width, uint8_t height);
    ~Map();

    const Graph &GetMap();

    void ExtendMap(uint8_t dir);

    uint8_t GetValue(uint8_t x, uint8_t y);
    uint8_t GetValue(uint8_t *position);

    Case *GetCase(uint8_t x, uint8_t y);
    Case *GetCase(uint8_t *position);

    bool SetCaseType(uint8_t x, uint8_t y, uint8_t type);
    bool SetCaseType(uint8_t *position, uint8_t type);

    bool AddNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight);
    bool AddNeighbour(uint8_t *position1, uint8_t *position2, uint8_t weight);

    bool RemoveNeighbour(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    bool RemoveNeighbour(uint8_t *position1, uint8_t *position2);

    bool ModifyDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t weight);
    bool ModifyDistance(uint8_t *position1, uint8_t *position2, uint8_t weight);

    std::unique_ptr<std::list<Case *>> GetNeighbours(uint8_t x, uint8_t y);
    std::unique_ptr<std::list<Case *>> GetNeighbours(uint8_t *position);

    uint8_t GetDistance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    uint8_t GetDistance(uint8_t *position1, uint8_t *position2);

    uint8_t GetCaseType(uint8_t x, uint8_t y);
    uint8_t GetCaseType(uint8_t *position);

    uint8_t GetWidth();
    uint8_t GetHeight();

    uint8_t GetCasePositionX(Case *c);
    uint8_t GetCasePositionY(Case *c);
    uint8_t *GetCasePosition(Case *c);

    std::unique_ptr<uint8_t[]> GetMap_Matrixe();
};