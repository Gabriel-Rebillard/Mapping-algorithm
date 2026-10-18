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
        

        bool FollowPath();

        void AnalyzeEnvironment(uint8_t** map);

        uint8_t* ComputeNextDestination();

        // Getters
        const Map* GetMap();
        const uint8_t* GetPosition();
        const uint8_t* GetStartPoint();
        const uint16_t GetFreeSpace();
};