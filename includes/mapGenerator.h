#include "cfg.h"
#include <random>

uint8_t** createMap(uint8_t width, uint8_t height);
uint8_t** createMap(uint8_t* size);

uint8_t** generateMap(uint8_t width, uint8_t height);
uint8_t** generateMap(uint8_t* size);

uint8_t** generateVerticalEdges(uint8_t** map, uint8_t width, uint8_t height);

uint8_t** generateHorizontalEdges(uint8_t** map, uint8_t width, uint8_t height);

bool testWallGeneration(uint8_t** map, uint8_t width, uint8_t height, uint8_t x, uint8_t y);
bool testWallGeneration(uint8_t** map, uint8_t* size, uint8_t x, uint8_t y);
bool testWallGeneration(uint8_t** map, uint8_t* size, uint8_t* position);
bool testWallGeneration(uint8_t** map, uint8_t width, uint8_t height, uint8_t* position);

bool testEdgeGeneration(uint8_t** map, uint8_t width, uint8_t height, uint8_t x, uint8_t y);
bool testEdgeGeneration(uint8_t** map, uint8_t* size, uint8_t x, uint8_t y);
bool testEdgeGeneration(uint8_t** map, uint8_t* size, uint8_t* position);
bool testEdgeGeneration(uint8_t** map, uint8_t width, uint8_t height, uint8_t* position);
