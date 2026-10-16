#include "mapGenerator.h"

uint8_t** createMap(uint8_t width, uint8_t height){
    uint8_t** map = new uint8_t*[height];
    for (uint8_t i = 0; i < height; i++){
        map[i] = new uint8_t[width];
        for (uint8_t j = 0; j < width; j++)
            map[i][j] = (i == 0 || i == height - 1 || j == 0 || j == width - 1) ? WALL_ID : ROOM_ID;
    }
    return map;
}
uint8_t** createMap(uint8_t* size){
    return createMap(size[0], size[1]);
}
uint8_t** generateMap(uint8_t width, uint8_t height){
    uint8_t** map = createMap(width, height);
    return map;
}
uint8_t** generateMap(uint8_t* size){
    return generateMap(size[0], size[1]);
}



uint8_t** generateVerticalEdges(uint8_t** map, uint8_t width, uint8_t height){
    width--;
    height--;
    uint8_t** verticalEdges = new uint8_t*[height];

    for (uint8_t i = 0; i < height; i++){
        verticalEdges[i] = new uint8_t[width];

        for (uint8_t j = 0; j < width; j++)
            verticalEdges[i][j] = (i == 0 || i == height - 1 || j == 0 || j == width - 1) ? 0 : (random() % GENERATOR_MAX_EDGE_WEIGHT) + 1;
    }
    return verticalEdges;
}
uint8_t** generateHorizontalEdges(uint8_t** map, uint8_t width, uint8_t height){
    width--;
    height--;
    uint8_t** horizontalEdges = new uint8_t*[height];

    for (uint8_t i = 0; i < height; i++){
        horizontalEdges[i] = new uint8_t[width];

        for (uint8_t j = 0; j < width; j++)
            horizontalEdges[i][j] = (i == 0 || i == height - 1 || j == 0 || j == width - 1) ? 0 : (random() % GENERATOR_MAX_EDGE_WEIGHT) + 1;
    }
    return horizontalEdges;
}

bool testWallGeneration(uint8_t** map, uint8_t width, uint8_t height, uint8_t x, uint8_t y){
    uint8_t adjacentWalls = 0;
    if (map[y-1][x] == WALL_ID) adjacentWalls++;
    if (map[y+1][x] == WALL_ID) adjacentWalls++;
    if (map[y][x-1] == WALL_ID) adjacentWalls++;
    if (map[y][x+1] == WALL_ID) adjacentWalls++;
    return adjacentWalls > GENERATOR_MAX_ADJACENT_WALLS;
}