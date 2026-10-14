#include "data.h"


std::ifstream OpenFile(std::string path)
{
    std::ifstream file(path);
    return file;
}

short* GetSize(std::ifstream& file)
{
    short* size = new short[2];
    std::string line;
    
    std::getline(file, line);
    if (line.size() > 2)
        std::cout << "Taille ligne mal remplie ?" << std::endl;

    size[0] = std::stoi(line);

    std::getline(file, line);
    if (line.size() > 2)
        std::cout << "Taille colonne mal remplie ?" << std::endl;

    size[1] = std::stoi(line);

    std::cout << "Taille : " << size[0] << " * " << size[1] << std::endl;

    return size;
}

short** CreateMat(short n, short m)
{
    short** mat = new short*[n];
    for (int i = 0; i < n; i++)
        mat[i] = new short[m];

    for (int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
                mat[i][j] = -1;

    return mat;
}

void GetMap(std::ifstream& file, short** map, short n, short m)
{
    std::string line;

    for (int i = 0; i < n; i++){
        if(!std::getline(file, line) || line.size() != m){
            std::cout << "Erreur sur la ligne " << i << std::endl;
            std::cout << "Fichier mal rempli" << std::endl;
            return;
        }

        std::cout << "Ligne " << i << " : " << line << std::endl;
        
        for(int j = 0; j < m; j++)
            map[i][j] = line[j] - '0';
    }
}

void PrintMap(short** map, short n, short m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        std::cout << (map[i][j] == WALL_ID ? WALL_SYMBOL : (map[i][j] == VICTIM_ID ? VICTIM_SYMBOL : ROOM_SYMBOL));
        std::cout << std::endl;
    }
}

void CloseFile(std::ifstream& file)
{
    file.close();
}

void DeleteMap(short** mat, short n)
{
    for (int i = 0; i < n; i++)
        delete[] mat[i];
    delete[] mat;
}