#pragma once

#include "cfg.h"
#include <random>
#include "map.h"

// Get the 'n'th bit of 'var'
#define bit(var, n) (var >> n) & 1

std::unique_ptr<Map> generateMap(uint8_t width, uint8_t height);
std::unique_ptr<Map> generateMap(uint8_t* size);

// Generators
void generateBorderWalls(Map* map);
void generateWalls(Map* map);
void generateVictims(Map* map);
void generateRobots(Map* map);
void generatePortals(Map* map);

void generateVerticalEdges(Map* map, bool random_weights=false, uint8_t min_random_weight=1, uint8_t max_random_weight=1);
void generateHorizontalEdges(Map* map, bool random_weights=false, uint8_t min_random_weight=1, uint8_t max_random_weight=1);

// Verify generation conditions
bool testWallGeneration(Map* map, uint8_t x, uint8_t y);
bool testWallGeneration(Map* map, uint8_t* position);

bool testEdgeGeneration(Map* map, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
bool testEdgeGeneration(Map* map, uint8_t* position1, uint8_t* position2);
