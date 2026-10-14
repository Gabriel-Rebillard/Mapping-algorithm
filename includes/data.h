#pragma once

#include <fstream>
#include <string>
#include <iostream>

std::ifstream OpenFile(std::string path);

short* GetSize(std::ifstream& file);

short** CreateMat(short n, short m);

void GetMap(std::ifstream& file , short** map, short n, short m);

void PrintMap(short** map, short n, short m);

void CloseFile(std::ifstream& file);

void DeleteMap(short** mat, short n);