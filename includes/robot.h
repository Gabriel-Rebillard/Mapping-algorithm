#pragma once
#include "map.h"
#include "cfg.h"

class Robot
{
private:
    Map _Map;
    std::vector<uint8_t> _CurrentPath;

    uint8_t _Position[2];
    uint8_t _StartPoint[2];

    std::vector<uint16_t> _DetectionDistance;
    uint8_t _VictimCount = 0;
    std::vector<uint8_t[2]> _VictimPositions;
    uint16_t _MoveCount = 0;

    void ExtendMap(uint8_t direction);
    
    uint16_t FindPath(uint8_t *destination);
    uint16_t FindPath(uint8_t x, uint8_t y);
    
    uint16_t FindNearestFrontier();
    uint16_t FindNextFrontier();
    
    public:
    Robot();
    
    void FillMap(Map *map, uint8_t start_point_x, uint8_t start_point_y);
    bool Move(uint8_t direction);
    bool FollowPath();

    void AnalyzeEnvironment(Map *map, uint8_t start_point_x, uint8_t start_point_y);
    void AnalyzeEnvironment(Map *map, uint8_t *start_point);

    uint8_t *ComputeNextDestination();

    // Getters
    Map *GetMap();
    uint8_t *GetPosition();
    uint8_t *GetStartPoint();
    uint16_t GetFreeSpace();
};