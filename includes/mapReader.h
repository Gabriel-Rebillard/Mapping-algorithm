#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "cfg.h"
#include "map.h"


// bitMap Recuperation
uint8_t *GetSize(std::ifstream &file);

bool **CreateNullBitMap(uint8_t width, uint8_t height);

void DeleteBitMap(bool **mat, uint8_t height);

void ReadBitMap(std::ifstream& file , bool** map, uint8_t n, uint8_t m);


// Affiche la carte donnée sous forme de matrice d'uint8_t (type de case seulement)
void DisplayMap(uint8_t** map, uint8_t n, uint8_t m);
// converti une carte sous forme de graphe en matrice d'uint8_t
void DisplayMap(Map* map);

// Cherche la position de la base du robot dans la carte
uint8_t* findStart(Map* map, uint8_t robot=0);

// Efface le terminal
void ClearConsole();

// Move Cursor in console
void gotoxy(int x,int y);