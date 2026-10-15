#include "robot.h"

Robot::Robot(){
    _Position[0] = 1;
    _Position[1] = 1;

    _StartPoint[0] = 1;
    _StartPoint[1] = 1;

    _VictimCount = 0;

    for (uint8_t i = 0; i < 3; i++)
        _Map.push_back(std::vector<uint8_t>(3));
}

bool Robot::ExtendMap(uint8_t direction){
    switch (direction){
        case UP:
            _Map.insert(_Map.begin(), std::vector<uint8_t>(_Map.at(0).size()));
            _Position [1]++;
            _StartPoint[1]++;
            break;
        
        case RIGHT:
            for (uint8_t i = 0; i < _Map.size(); i++)
                _Map.at(i).push_back(ROOM_ID);
                _Position [0]++;
                _StartPoint[0]++;
            break;
        
        case DOWN:
            _Map.push_back(std::vector<uint8_t>(_Map.at(0).size()));
            _Position [1]--;
            _StartPoint[1]--;
            break;
        
        case LEFT:
            for (uint8_t i = 0; i < _Map.size(); i++)
                _Map.at(i).insert(_Map.at(i).begin(), WALL_ID);
            _Position [0]--;
            _StartPoint[0]--;
            break;
    }
}

bool Robot::Move(uint8_t direction){
    switch(direction){
        case UP:
            if (_Map.at(_Position[1] - 1).at(_Position[0]) == WALL_ID)
                return 0;
            _Position[1]--;
            break;
        case RIGHT:
            if (_Map.at(_Position[1]).at(_Position[0] + 1) == WALL_ID)
                return 0;
            _Position[0]++;
            break;
        case DOWN:
            if (_Map.at(_Position[1] + 1).at(_Position[0]) == WALL_ID)
                return 0;
            _Position[1]++;
            break;
        case LEFT:
            if (_Map.at(_Position[1]).at(_Position[0] - 1) == WALL_ID)
                return 0;
            _Position[0]--;
            break;
    }
    return 1;
}

bool Robot::GoTo(uint8_t x, uint8_t y){
    if(_CurrentPath.empty())
        return 1;

    Move(_CurrentPath.front());
    _CurrentPath.erase(_CurrentPath.begin());
    return 0;
}

bool Robot::GoTo(uint8_t* destination){
    return GoTo(destination[0], destination[1]);
}