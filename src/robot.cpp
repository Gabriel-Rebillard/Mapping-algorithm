#include "../includes/robot.h"

Robot::Robot()
: _Map(3, 3) {
    _Position[0] = 1;
    _Position[1] = 1;

    _StartPoint[0] = 1;
    _StartPoint[1] = 1;

    _VictimCount = 0;

    _Map.SetCaseType(_Position, ROBOT_BASE_ID);
}

bool Robot::ExtendMap(uint8_t direction){
    switch (direction){
        case UP:
            _Map.ExtendMap(UP);
            _Position [1]++;
            _StartPoint[1]++;
            break;
        
        case RIGHT:
            _Map.ExtendMap(RIGHT);
            break;
        
        case DOWN:
            _Map.ExtendMap(DOWN);
            break;
        
        case LEFT:
            _Map.ExtendMap(LEFT);
            _Position [0]++;
            _StartPoint[0]++;
            break;
    }
}

bool Robot::Move(uint8_t direction){
    switch(direction){
        case UP:
            if (_Map.GetCaseType(_Position[0], _Position[1] - 1) == WALL_ID )
                return 0;
            _Position[1]--;
            break;
        case RIGHT:
            if (_Map.GetCaseType(_Position[0] + 1, _Position[1]) == WALL_ID)
                return 0;
            _Position[0]++;
            break;
        case DOWN:
            if (_Map.GetCaseType(_Position[0], _Position[1] + 1) == WALL_ID)
                return 0;
            _Position[1]++;
            break;
        case LEFT:
            if (_Map.GetCaseType(_Position[0] - 1, _Position[1]) == WALL_ID)
                return 0;
            _Position[0]--;
            break;
    }
    
    _MoveCount++;
    return 1;
}

bool Robot::FollowPath(){
    if(_CurrentPath.empty())
        return 1;

    Move(_CurrentPath.front());
    _CurrentPath.erase(_CurrentPath.begin());
    return 0;
}
