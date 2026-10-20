#include "../includes/robot.h"

Robot::Robot()
    : _Map(3, 3)
{
    _Position[0] = 1;
    _Position[1] = 1;

    _StartPoint[0] = 1;
    _StartPoint[1] = 1;

    _VictimCount = 0;

    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            _Map.SetCaseType(i, j, UNKNOWN_ID);
    _Map.SetCaseType(_Position, ROBOT_BASE_ID);
}

void Robot::ExtendMap(uint8_t direction)
{
    switch (direction)
    {
    case UP:
        _Map.ExtendMap(UP);
        _Position[1]++;
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
        _Position[0]++;
        _StartPoint[0]++;
        break;
    }
}

bool Robot::Move(uint8_t direction)
{
    switch (direction)
    {
    case UP:
        if (_Map.GetCaseType(_Position[0], _Position[1] - 1) == WALL_ID)
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

void Robot::FillMap(Map *map, uint8_t start_point_x, uint8_t start_point_y)
{
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            if (_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1) == UNKNOWN_ID)
            {
                _Map.SetCaseType(_Position[0] + i - 1, _Position[1] + j - 1,
                                 map->GetCaseType(start_point_x - _StartPoint[0] + _Position[0] + i - 1,
                                                  start_point_y - _StartPoint[1] + _Position[1] + i - 1));

                if (_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1) == VICTIM_ID)
                    _VictimCount++;
            }
}

bool Robot::FollowPath()
{
    if (_CurrentPath.empty())
        return 1;

    Move(_CurrentPath.front());
    _CurrentPath.erase(_CurrentPath.begin());
    return 0;
}

void Robot::AnalyzeEnvironment(Map *map, uint8_t start_point_x, uint8_t start_point_y)
{
    if (_Position[0] == 0)
        ExtendMap(LEFT);
    else if (_Position[0] == _Map.GetWidth() - 1)
        ExtendMap(RIGHT);
    else if (_Position[1] == 0)
        ExtendMap(UP);
    else if (_Position[1] == _Map.GetHeight() - 1)
        ExtendMap(DOWN);

    FillMap(map, start_point_x, start_point_y);
}

void Robot::AnalyzeEnvironment(Map *map, uint8_t *start_point)
{
    AnalyzeEnvironment(map, start_point[0], start_point[1]);
}

Map *Robot::GetMap()
{
    return &_Map;
}

uint8_t *Robot::GetPosition()
{
    return _Position;
}

uint8_t *Robot::GetStartPoint()
{
    return _StartPoint;
}