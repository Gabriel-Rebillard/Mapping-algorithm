#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "cfg.h"
#include "map.h"

std::ifstream OpenFile(std::string path);

uint8_t* GetSize(std::ifstream& file);

uint8_t** CreateMat(uint8_t n, uint8_t m);

void GetMap(std::ifstream& file , uint8_t** map, uint8_t n, uint8_t m);

void DisplayMap(uint8_t** map, uint8_t n, uint8_t m);
void DisplayMap(Map* map);

uint8_t* findStart(Map* map, uint8_t robot=0);

void CloseFile(std::ifstream& file);

void DeleteMap(uint8_t** mat, uint8_t n);

void ClearConsole();