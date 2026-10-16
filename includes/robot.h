#pragma once
#include <vector>
#include "cfg.h"


enum direction
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

class Robot
{   
    private:
        std::vector<std::vector<uint8_t>> _Map;
        std::vector<std::vector<uint8_t>> _VerticalEdges;
        std::vector<std::vector<uint8_t>> _HorizontalEdges;
        std::vector<uint8_t> _CurrentPath;

        uint8_t _Position[2];
        uint8_t _StartPoint[2];

        std::vector<uint16_t> _DetectionDistance;
        uint8_t _VictimCount = 0;
        uint16_t _MoveCount = 0;

        bool ExtendMap(uint8_t direction);
        void FillMap(uint8_t** map);

        bool Move(uint8_t direction);
        
        uint16_t FindPath(uint8_t* destination);
        uint16_t FindPath(uint8_t x, uint8_t y);
        
        uint16_t FindNearestFrontier();
        uint16_t FindNextFrontier();
        
    public:
        Robot();
        

        bool GoTo(uint8_t* destination);
        bool GoTo(uint8_t x, uint8_t y);

        void AnalyzeEnvironment(uint8_t** map);

        uint8_t* ComputeNextDestination();

        // Getters
        const std::vector<std::vector<uint8_t>>& getMap();
        const uint8_t* getPosition();
        const uint8_t* getStartPoint();
        const uint16_t getFreeSpace();
};