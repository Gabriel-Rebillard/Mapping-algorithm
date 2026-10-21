#pragma once

#include "cfg.h"
#include <random>
#include "map.h"

// Get the 'n'th bit of 'var', used for testing walls generation
#define bit(var, n) (var >> n) & 1

// Map generation
std::unique_ptr<Map> generateMap(uint8_t width, uint8_t height); 
std::unique_ptr<Map> generateMap(uint8_t* size);

// Generators
void generateBorderWalls(Map* map); // Genere les murs extérieurs                                  
void generateWalls(Map* map);       // Genere les murs internes et pilliers                        
void generateVictims(Map* map);     // Genere les victimes                                         
void generateRobots(Map* map);      // Genere les robots                                           
void generatePortals(Map* map);     // Genere les portails (pas encore utilisé)                    

                                    // Genere les passages avec distance entre les cases accesibles
void generateVerticalEdges(Map* map, bool random_weights=false, uint8_t min_random_weight=1, uint8_t max_random_weight=1);
void generateHorizontalEdges(Map* map, bool random_weights=false, uint8_t min_random_weight=1, uint8_t max_random_weight=1);

// Verify generation conditions
bool testWallGeneration(Map* map, uint8_t x, uint8_t y);                            // Verifie si la generation d'un mur est autorisée
bool testWallGeneration(Map* map, uint8_t* position);

bool testEdgeGeneration(Map* map, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);  // Verifie si la generation d'un passage est autorisée
bool testEdgeGeneration(Map* map, uint8_t* position1, uint8_t* position2);
