#include "../includes/robot.h"
#include <algorithm>
#include <cstdlib>

Robot::Robot(uint8_t id)
    : _Map(3, 3)
{
    _RobotID = id;

    _Position[0] = 1;
    _Position[1] = 1;

    _StartPoint[0] = 1;
    _StartPoint[1] = 1;

    _VictimCount = 0;

    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
            _Map.SetCaseType(i, j, UNKNOWN_ID);
    _Map.SetCaseType(_Position, ROBOT_BASE_ID + id);
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

void Robot::OptimizeFrontier()
{
    // uint8_t h = _Map.GetHeight() - 1;
    // uint8_t w = _Map.GetWidth() - 1;
    uint8_t adjUnknownCount;
    /*for (uint8_t i = 1; i < w; i++)
    {
        for (uint8_t j = 1; j < h; j++)
        {
            adjUnknownCount = 0;
            if (isFrontier(_Map.GetCaseType(i, j)))
            {
                if (_Map.GetCaseType(i - 1, j) == UNKNOWN_ID)
                    adjUnknownCount++;
                if (_Map.GetCaseType(i + 1, j) == UNKNOWN_ID)
                    adjUnknownCount++;
                if (_Map.GetCaseType(i, j - 1) == UNKNOWN_ID)
                    adjUnknownCount++;
                if (_Map.GetCaseType(i, j + 1) == UNKNOWN_ID)
                    adjUnknownCount++;

                if (adjUnknownCount == 0)
                    _Map.SetCaseType(i, j, removeFrontier(_Map.GetCaseType(i, j)));
            }
        }
    }*/

    auto mapGraph = _Map.GetMap().GetGraph();
    uint8_t w = _Map.GetWidth();
    uint8_t *pos = new uint8_t[2];

    auto frontierIte = mapGraph->begin();
    std::advance(frontierIte, w);

    auto endIte = mapGraph->begin();
    std::advance(endIte, w * (_Map.GetHeight() - 1));

    while (frontierIte < endIte)
    {
        frontierIte = std::find_if(frontierIte, endIte,
                                   [](const std::unique_ptr<Vertex> &v)
                                   { return isFrontier(v.get()->GetValue()); });

        if (frontierIte < endIte)
        {

            pos = _Map.GetCasePosition(static_cast<Case *>((*frontierIte).get()));
            if (pos[0] > 0 && pos[0] < w - 1)
            {

                adjUnknownCount = 0;
                if (_Map.GetCaseType(pos[0] - 1, pos[1]) == UNKNOWN_ID || (isFrontier(_Map.GetCaseType(pos[0] - 1, pos[1])) && _Map.GetDistance(pos[0] - 1, pos[1], pos[0], pos[1]) == INFINITY))
                    adjUnknownCount++;
                if (_Map.GetCaseType(pos[0] + 1, pos[1]) == UNKNOWN_ID || (isFrontier(_Map.GetCaseType(pos[0] + 1, pos[1])) && _Map.GetDistance(pos[0] + 1, pos[1], pos[0], pos[1]) == INFINITY))
                    adjUnknownCount++;
                if (_Map.GetCaseType(pos[0], pos[1] - 1) == UNKNOWN_ID || (isFrontier(_Map.GetCaseType(pos[0], pos[1] - 1)) && _Map.GetDistance(pos[0], pos[1] - 1, pos[0], pos[1]) == INFINITY))
                    adjUnknownCount++;
                if (_Map.GetCaseType(pos[0], pos[1] + 1) == UNKNOWN_ID || (isFrontier(_Map.GetCaseType(pos[0], pos[1] + 1)) && _Map.GetDistance(pos[0], pos[1] + 1, pos[0], pos[1]) == INFINITY))
                    adjUnknownCount++;

                if (adjUnknownCount == 0)
                    _Map.SetCaseType(pos[0], pos[1], removeFrontier(_Map.GetCaseType(pos[0], pos[1])));
            }
        }
        frontierIte++;
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
    if (isFrontier(_Map.GetCaseType(_Position)))
        _Map.SetCaseType(_Position, removeFrontier(_Map.GetCaseType(_Position)));

    if (_Map.GetCaseType(_Position) == ROOM_ID)
        _Map.GetCase(_Position)->IncrementVisits();;

    // Cases filling
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 3; j++)
        {
            if (_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1) != map->GetCaseType(start_point_x - _StartPoint[0] + _Position[0] + i - 1,
                                                                                                 start_point_y - _StartPoint[1] + _Position[1] + j - 1))
            {
                _Map.SetCaseType(_Position[0] + i - 1, _Position[1] + j - 1,
                                 map->GetCaseType(start_point_x - _StartPoint[0] + _Position[0] + i - 1,
                                                  start_point_y - _StartPoint[1] + _Position[1] + j - 1));

                if (_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1) == VICTIM_ID)
                {
                    _VictimCount++;
                    uint8_t *pos = new uint8_t[2];
                    pos[0] = _Position[0] + i - 1;
                    pos[1] = _Position[1] + j - 1;
                    _VictimPositions.push_back(pos);
                }
                if (_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1) != WALL_ID)
                    _Map.SetCaseType(_Position[0] + i - 1, _Position[1] + j - 1, addFrontier(_Map.GetCaseType(_Position[0] + i - 1, _Position[1] + j - 1)));
            }
        }

    // Horizontal Edge filling
    uint8_t xc, yc;
    for (uint8_t i = 0; i < 2; i++)
        for (uint8_t j = 0; j < 3; j++)
        {
            xc = _Position[0] + i - 1;
            yc = _Position[1] + j - 1;
            if (_Map.GetDistance(xc, yc, xc + 1, yc) == INFINITY)
                _Map.AddNeighbour(xc, yc, xc + 1, yc, map->GetDistance(start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc, start_point_x - _StartPoint[0] + xc + 1, start_point_y - _StartPoint[1] + yc));

            if (_Map.GetDistance(xc, yc, xc + 1, yc) != map->GetDistance(start_point_x - _StartPoint[0] + xc,
                                                                         start_point_y - _StartPoint[1] + yc,
                                                                         start_point_x - _StartPoint[0] + xc + 1,
                                                                         start_point_y - _StartPoint[1] + yc))
                _Map.ModifyDistance(xc, yc, xc + 1, yc, map->GetDistance(start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc, start_point_x - _StartPoint[0] + xc + 1, start_point_y - _StartPoint[1] + yc));
        }

    // Vertical Edge filling
    for (uint8_t i = 0; i < 3; i++)
        for (uint8_t j = 0; j < 2; j++)
        {
            xc = _Position[0] + i - 1;
            yc = _Position[1] + j - 1;
            if (_Map.GetDistance(xc, yc, xc, yc + 1) == INFINITY)
                _Map.AddNeighbour(xc, yc, xc, yc + 1, map->GetDistance(start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc, start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc + 1));

            if (_Map.GetDistance(xc, yc, xc, yc + 1) != map->GetDistance(start_point_x - _StartPoint[0] + xc,
                                                                         start_point_y - _StartPoint[1] + yc,
                                                                         start_point_x - _StartPoint[0] + xc,
                                                                         start_point_y - _StartPoint[1] + yc + 1))
                _Map.ModifyDistance(xc, yc, xc, yc + 1, map->GetDistance(start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc, start_point_x - _StartPoint[0] + xc, start_point_y - _StartPoint[1] + yc + 1));
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

int Robot::ClosedRoom(Map *map)
{
    uint8_t sides = 0;
    int enclosedNum = 0;
    for (int i = 0; i < map->GetHeight(); i++)
    {
        for (int j = 0; j < map->GetWidth(); j++)
        {
            sides = 0;
            if (map->GetCaseType(i,j) == UNKNOWN_ID)
            {
                if(map->GetEnclosedState(i,j) == false){
                    for (int x = i; x < map->GetHeight(); x++)
                    {
                        if (map->GetCaseType(x,j) == WALL_ID)
                        {
                            sides++;
                            break;
                        }
                    }
                    for (int x = i; x > 0; x--)
                    {
                        if (map->GetCaseType(x,j) == WALL_ID)
                        {
                            sides++;
                            break;
                        }
                    }
                    for (int y = j; y < map->GetWidth(); y++)
                    {
                        if (map->GetCaseType(i,y) == WALL_ID)
                        {
                            sides++;
                            break;
                        }
                    }
                    for (int y = j; y < map->GetWidth(); y++)
                    {
                        if (map->GetCaseType(i,y) == WALL_ID)
                        {
                            sides++;
                            break;
                        }
                    }
                    if (sides == 4)
                    {
                        map->FloodEnclosed(i,j);
                        enclosedNum++;
                    }
                }
            }
        }
    }
    return enclosedNum;
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
    OptimizeFrontier();
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

uint8_t Robot::GetId(){
    return _RobotID;
}
uint8_t Robot::GetVictimCount()
{
    return _VictimCount;
}

const std::vector<const uint8_t *> *Robot::GetVictimPositions()
{
    return &_VictimPositions;
}


// ============================================================================
// Exploration : choix de la direction qui découvre le plus de cases
// (ajouté par Erwan, ne modifie rien au code existant)
// ============================================================================

// Convertit une direction (UP, RIGHT, DOWN, LEFT) en décalage (dx, dy)
void Robot::DirectionVector(uint8_t direction, int &dx, int &dy)
{
    dx = 0;
    dy = 0;
    switch (direction)
    {
    case UP:
        dy = -1;
        break;
    case RIGHT:
        dx = 1;
        break;
    case DOWN:
        dy = 1;
        break;
    case LEFT:
        dx = -1;
        break;
    }
}

// Lit une case de _Map sans risque.
// Hors de la carte = case pas encore vue -> UNKNOWN_ID
uint8_t Robot::GetCell(int x, int y)
{
    if (x < 0 || y < 0 || x >= (int)_Map.GetWidth() || y >= (int)_Map.GetHeight())
        return UNKNOWN_ID;
    return _Map.GetCaseType((uint8_t)x, (uint8_t)y);
}

// Nombre de cases inconnues qu'un pas dans cette direction révélerait
// -1 si la case voisine est un mur
int Robot::CountUnknown(uint8_t direction)
{
    int dx, dy;
    DirectionVector(direction, dx, dy);
    int x = _Position[0];
    int y = _Position[1];

    if (GetCell(x + dx, y + dy) == WALL_ID)
        return -1;

    int n = 0;
    for (int k = -1; k <= 1; k++)
    {
        int cx = x + 2 * dx + (dx == 0 ? k : 0);
        int cy = y + 2 * dy + (dy == 0 ? k : 0);
        if (GetCell(cx, cy) == UNKNOWN_ID)
            n++;
    }
    return n;
}

// Renvoie la direction qui découvre le plus de cases, ou NO_DIRECTION
uint8_t Robot::Check()
{
    const uint8_t ordre[4] = {DOWN, RIGHT, UP, LEFT}; // sud, est, nord, ouest
    uint8_t meilleure = NO_DIRECTION;
    int max = 0;

    for (uint8_t d : ordre)
    {
        int n = CountUnknown(d);
        if (n == 3)
            return d;
        if (n > max)
        {
            max = n;
            meilleure = d;
        }
    }
    return meilleure;
}

// ============================================================================
// A* (ajout Erwan) : implementation
// ============================================================================
/*
    A* adapté au projet Mapping-algorithm.

    Basé sur "a-star" de Damian Barczyński : https://github.com/daancode/a-star

    Modifications par rapport à l'original :
      - lit les murs directement dans la carte du projet (Map) au lieu d'une liste
      - les cases MUR et INCONNUES sont infranchissables
      - 4 directions seulement (UP, RIGHT, DOWN, LEFT), coût de 1 par pas
      - renvoie la liste des directions à suivre, du départ vers l'arrivée
      - renvoie false si aucun chemin n'existe (l'original renvoyait un faux chemin)
      - à score égal, préfère la case la plus proche de l'arrivée
*/

AStar::Node::Node(Vec2i coordinates_, Node *parent_, uint8_t move_)
{
    parent = parent_;
    coordinates = coordinates_;
    move = move_;
    G = H = 0;
}

AStar::uint AStar::Node::getScore() const
{
    return G + H;
}

AStar::Generator::Generator(Map *map_)
{
    map = map_;
    // L'ordre n'a pas d'importance pour la longueur du chemin, seulement pour les égalités
    steps = {
        {UP, {0, -1}},
        {RIGHT, {1, 0}},
        {DOWN, {0, 1}},
        {LEFT, {-1, 0}},
    };
}

bool AStar::Generator::findPath(Vec2i source_, Vec2i target_, std::vector<uint8_t> &path_)
{
    path_.clear();

    // Déjà arrivé
    if (source_ == target_)
        return true;

    // Arrivée infranchissable : inutile de chercher
    if (detectCollision(target_))
        return false;

    Node *current = nullptr;
    NodeSet openSet, closedSet;
    openSet.reserve(100);
    closedSet.reserve(100);

    Node *start = new Node(source_);
    start->H = manhattan(source_, target_);
    openSet.push_back(start);

    bool found = false;

    while (!openSet.empty())
    {
        // 1. Prendre la case de la liste ouverte avec le plus petit f = G + H
        //    (à égalité, celle qui a le plus petit H : la plus proche de l'arrivée)
        auto current_it = openSet.begin();
        current = *current_it;
        for (auto it = openSet.begin(); it != openSet.end(); it++)
        {
            Node *node = *it;
            if (node->getScore() < current->getScore() ||
                (node->getScore() == current->getScore() && node->H < current->H))
            {
                current = node;
                current_it = it;
            }
        }

        // 2. Arrivée atteinte
        if (current->coordinates == target_)
        {
            found = true;
            break;
        }

        // 3. La case passe de la liste ouverte à la liste fermée
        closedSet.push_back(current);
        openSet.erase(current_it);

        // 4. Examiner les 4 voisins
        for (const Step &step : steps)
        {
            Vec2i newCoordinates = current->coordinates + step.delta;
            if (detectCollision(newCoordinates) || findNodeOnList(closedSet, newCoordinates))
                continue;

            uint totalCost = current->G + 1; // 1 pas = coût 1

            Node *successor = findNodeOnList(openSet, newCoordinates);
            if (successor == nullptr)
            {
                // Nouvelle case découverte
                successor = new Node(newCoordinates, current, step.dir);
                successor->G = totalCost;
                successor->H = manhattan(newCoordinates, target_);
                openSet.push_back(successor);
            }
            else if (totalCost < successor->G)
            {
                // Chemin plus court trouvé vers une case déjà connue
                successor->parent = current;
                successor->move = step.dir;
                successor->G = totalCost;
            }
        }
    }

    // 5. Reconstruire le chemin en remontant les parents, puis le remettre dans l'ordre
    if (found)
    {
        for (Node *n = current; n->parent != nullptr; n = n->parent)
            path_.push_back(n->move);
        std::reverse(path_.begin(), path_.end());
    }

    releaseNodes(openSet);
    releaseNodes(closedSet);

    return found;
}

AStar::Node *AStar::Generator::findNodeOnList(NodeSet &nodes_, Vec2i coordinates_)
{
    for (Node *node : nodes_)
        if (node->coordinates == coordinates_)
            return node;
    return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet &nodes_)
{
    for (Node *node : nodes_)
        delete node;
    nodes_.clear();
}

bool AStar::Generator::detectCollision(Vec2i coordinates_)
{
    // Hors de la carte
    if (coordinates_.x < 0 || coordinates_.y < 0 ||
        coordinates_.x >= (int)map->GetWidth() || coordinates_.y >= (int)map->GetHeight())
        return true;

    // On retire le bit "frontière" pour ne garder que le type de la case
    uint8_t type = removeFrontier(map->GetCaseType((uint8_t)coordinates_.x, (uint8_t)coordinates_.y));
    return type == WALL_ID || type == UNKNOWN_ID;
}

AStar::uint AStar::manhattan(Vec2i source_, Vec2i target_)
{
    return std::abs(source_.x - target_.x) + std::abs(source_.y - target_.y);
}

// ============================================================================
// A* : chemin le plus court vers une case (ajout Erwan)
// ============================================================================

// Cherche le chemin le plus court vers (x, y) dans la carte du robot
// Remplit _CurrentPath avec les directions a suivre
// Renvoie le nombre de pas, ou NO_PATH si la case est inaccessible
uint16_t Robot::FindPath(uint8_t x, uint8_t y)
{
    AStar::Generator generator(&_Map);
    std::vector<uint8_t> path;

    if (!generator.findPath({_Position[0], _Position[1]}, {x, y}, path))
        return NO_PATH;

    _CurrentPath = path;
    return path.size();
}

uint16_t Robot::FindPath(uint8_t *destination)
{
    return FindPath(destination[0], destination[1]);
}

bool Robot::GoTo(uint8_t x, uint8_t y)
{
    return FindPath(x, y) != NO_PATH;
}

bool Robot::HasPath()
{
    return !_CurrentPath.empty();
}
