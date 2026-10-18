#include "../includes/mapGenerator.h"

std::unique_ptr<Map> generateMap(uint8_t width, uint8_t height)
{
    std::unique_ptr<Map> map = std::make_unique<Map>(width, height);

    // Walls generation
    generateBorderWalls(map.get());
    generateWalls(map.get());

    // Vicitms generation
    generateVictims(map.get());

    // Robots generation
    generateRobots(map.get());

    // Neighbours generation
    generateVerticalEdges(map.get(), true, 2, 5);
    generateHorizontalEdges(map.get());

    generatePortals(map.get());

    return map;
}
std::unique_ptr<Map> generateMap(uint8_t *size)
{
    return generateMap(size[0], size[1]);
}

void generateBorderWalls(Map *map)
{
    uint8_t h = map->GetHeight();
    uint8_t w = map->GetWidth();
    for (uint8_t i = 0; i < h; i++)
    {
        map->SetCaseType(0, i, WALL_ID);
        map->SetCaseType(w - 1, i, WALL_ID);
    }
    for (uint8_t i = 0; i < w; i++)
    {
        map->SetCaseType(i, 0, WALL_ID);
        map->SetCaseType(i, h - 1, WALL_ID);
    }
}
void generateWalls(Map *map)
{
    // Generate walls next to other walls by spiral generation
    uint8_t h = map->GetHeight() - 2;
    uint8_t w = map->GetWidth() - 2;

    float probability = GENERATOR_WALL_PROBABILITY;

    uint8_t xBegin = 1, xEnd = w, yBegin = 1, yEnd = h;
    while (xBegin < xEnd && yBegin < yEnd)
    {
        // First row
        for (uint8_t i = xBegin; i < xEnd; i++)
            if (rand() % (int)(1 / probability) == 0 && testWallGeneration(map, i, yBegin))
                map->SetCaseType(i, yBegin, WALL_ID);
        yBegin++;

        // Last column
        for (uint8_t i = yBegin; i < yEnd; i++)
            if (rand() % (int)(1 / probability) == 0 && testWallGeneration(map, xEnd, i))
                map->SetCaseType(xEnd, i, WALL_ID);
        xEnd--;

        // Last row
        for (uint8_t i = xEnd; i > xBegin; i--)
            if (rand() % (int)(1 / probability) == 0 && testWallGeneration(map, i, yEnd))
                map->SetCaseType(i, yEnd, WALL_ID);
        yEnd--;

        // First column
        for (uint8_t i = yEnd; i > yBegin; i--)
            if (rand() % (int)(1 / probability) == 0 && testWallGeneration(map, xBegin, i))
                map->SetCaseType(xBegin, i, WALL_ID);
        xBegin++;

        probability *= GENERATOR_WALL_PROBABILITY_EVOLUTION_RULE(w, h);
        if (probability > 1)
            probability = 1;
    }
}
void generateVictims(Map *map)
{
    uint8_t x, y;
    for (uint8_t i = 0; i < GENERATOR_VICTIM_NUMBER; i++)
    {
        do
        {
            x = rand() % (map->GetWidth() - 2) + 1;
            y = rand() % (map->GetHeight() - 2) + 1;
        } while (map->GetCaseType(x, y) == WALL_ID || map->GetCaseType(x, y) == VICTIM_ID);
        map->SetCaseType(x, y, VICTIM_ID);
    }
}
void generateRobots(Map *map)
{
    uint8_t x, y;
    for (uint8_t i = 0; i < GENERATOR_ROBOT_NUMBER; i++)
    {
        do
        {
            x = rand() % (map->GetWidth() - 2) + 1;
            y = rand() % (map->GetHeight() - 2) + 1;
        } while (map->GetCaseType(x, y) == WALL_ID || map->GetCaseType(x, y) == VICTIM_ID || map->GetCaseType(x, y) == ROBOT_BASE_ID);
        map->SetCaseType(x, y, ROBOT_BASE_ID);
    }
}
void generatePortals(Map *map)
{
    uint8_t x1, y1, x2, y2;
    for (uint8_t i = 0; i < GENERATOR_PORTAL_NUMBER; i++)
    {
        for (uint8_t j = 0; j < 2; j++)
        {
            x2 = x1;
            y2 = y1;

            do
            {
                x1 = rand() % (map->GetWidth() - 2) + 1;
                y1 = rand() % (map->GetHeight() - 2) + 1;
            } while (map->GetCaseType(x1, y1) == WALL_ID || map->GetCaseType(x1, y1) == VICTIM_ID || map->GetCaseType(x1, y1) == ROBOT_BASE_ID);
            map->SetCaseType(x1, y1, PORTAL_ID);
        }

        map->AddNeighbour(x1, y1, x2, y2, 1);
    }
}

void generateVerticalEdges(Map *map, bool random_weights, uint8_t min_random_weight, uint8_t max_random_weight)
{
    uint8_t h = map->GetHeight() - 3;
    uint8_t w = map->GetWidth() - 2;
    for (uint8_t i = 1; i < w; i++)
        for (uint8_t j = 1; j < h; j++)
        {
            if (testEdgeGeneration(map, i, j, i, j + 1))
            {
                if (random_weights)
                    map->AddNeighbour(i, j, i, j + 1, rand() % (max_random_weight - min_random_weight + 1) + min_random_weight);
                else
                    map->AddNeighbour(i, j, i, j + 1, 1);
            }
        }
}
void generateHorizontalEdges(Map *map, bool random_weights, uint8_t min_random_weight, uint8_t max_random_weight)
{
    uint8_t h = map->GetHeight() - 2;
    uint8_t w = map->GetWidth() - 3;
    for (uint8_t i = 1; i < w; i++)
        for (uint8_t j = 1; j < h; j++)
        {
            if (testEdgeGeneration(map, i, j, i + 1, j))
            {
                if (random_weights)
                    map->AddNeighbour(i, j, i + 1, j, rand() % (max_random_weight - min_random_weight + 1) + min_random_weight);
                else
                    map->AddNeighbour(i, j, i + 1, j, 1);
            }
        }
}

bool testWallGeneration(Map *map, uint8_t x, uint8_t y)
{
    if (x == 0 || y == 0 || x == map->GetWidth() - 1 || y == map->GetHeight() - 1)
        return 0;

    uint8_t adjacentWalls = 0;

    // straight adjacent
    // UP       bit 0
    if (map->GetCaseType(x, y - 1) == WALL_ID)
        adjacentWalls |= 1;

    // DOWN      bit 1
    if (map->GetCaseType(x, y + 1) == WALL_ID)
        adjacentWalls |= 2;

    // LEFT    bit 2
    if (map->GetCaseType(x - 1, y) == WALL_ID)
        adjacentWalls |= 4;

    // RIGHT     bit 3
    if (map->GetCaseType(x + 1, y) == WALL_ID)
        adjacentWalls |= 8;

    // diagonal adjacent
    // UP LEFT      bit 4
    if (map->GetCaseType(x - 1, y - 1) == WALL_ID)
        adjacentWalls += 16;

    // DOWN LEFT    bit 5
    if (map->GetCaseType(x - 1, y + 1) == WALL_ID)
        adjacentWalls += 32;

    // UP RIGHT     bit 6
    if (map->GetCaseType(x + 1, y - 1) == WALL_ID)
        adjacentWalls += 64;

    // DOWN RIGHT   bit 7
    if (map->GetCaseType(x + 1, y + 1) == WALL_ID)
        adjacentWalls += 128;

    if (adjacentWalls == 0)
        return rand() % (int)(1 / GENERATOR_PILLAR_PROBABILITY) == 0;

    if ((bit(adjacentWalls, 0) + bit(adjacentWalls, 1) + bit(adjacentWalls, 2) + bit(adjacentWalls, 3)) <= GENERATOR_MAX_ADJACENT_WALLS)
    {
        if (bit(adjacentWalls, 4) && !(bit(adjacentWalls, 2) || bit(adjacentWalls, 0))) // UP LEFT AND (LEFT OR UP)
            return 0;
        if (bit(adjacentWalls, 5) && !(bit(adjacentWalls, 2) || bit(adjacentWalls, 1))) // DOWN LEFT AND (LEFT OR DOWN)
            return 0;
        if (bit(adjacentWalls, 6) && !(bit(adjacentWalls, 0) || bit(adjacentWalls, 3))) // UP RIGHT AND (RIGHT OR UP)
            return 0;
        if (bit(adjacentWalls, 7) && !(bit(adjacentWalls, 1) || bit(adjacentWalls, 3))) // DOWN RIGHT AND (RIGHT OR DOWN)
            return 0;
        return 1;
    }

    return 0;
}
bool testWallGeneration(Map *map, uint8_t *position)
{
    return testWallGeneration(map, position[0], position[1]);
}

bool testEdgeGeneration(Map *map, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if (x1 == x2 && y1 == y2)
        return 0;
    if (map->GetCaseType(x1, y1) == WALL_ID || map->GetCaseType(x2, y2) == WALL_ID)
        return 0;
    return 1;
}
bool testEdgeGeneration(Map *map, uint8_t *position1, uint8_t *position2)
{
    return testEdgeGeneration(map, position1[0], position1[1], position2[0], position2[1]);
}