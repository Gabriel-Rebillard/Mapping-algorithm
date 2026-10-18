#include "../includes/mapReader.h"


std::ifstream OpenFile(std::string path)
{
    std::ifstream file(path);
    return file;
}

uint8_t* GetSize(std::ifstream& file)
{
    uint8_t* size = new uint8_t[2];
    std::string line;
    
    std::getline(file, line);
    if (line.size() > 2)
        std::cout << "Taille ligne mal remplie ?" << std::endl;

    size[0] = std::stoi(line);

    std::getline(file, line);
    if (line.size() > 2)
        std::cout << "Taille colonne mal remplie ?" << std::endl;

    size[1] = std::stoi(line);

    std::cout << "Taille : " << (short)size[0] << " * " << (short)size[1] << std::endl;

    return size;
}

uint8_t** CreateMat(uint8_t width, uint8_t height)
{
    uint8_t** mat = new uint8_t*[height];
    for (int i = 0; i < height; i++)
        mat[i] = new uint8_t[width];

    for (int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
                mat[i][j] = -1;

    return mat;
}

void GetMap(std::ifstream& file, uint8_t** map, uint8_t width, uint8_t height)
{
    std::string line;

    for (int i = 0; i < height; i++){
        if(!std::getline(file, line) || line.size() != width){
            std::cout << "Erreur sur la ligne " << i + 2 << std::endl;
            std::cout << "Fichier mal rempli" << std::endl;
            return;
        }

        std::cout << "Ligne " << i + 2 << " : " << line << std::endl;
        
        for(int j = 0; j < width; j++)
            map[i][j] = line[j] - '0';
    }
}

void DisplayMap(int** map, uint8_t width, uint8_t height)
{
    std::string symbol = ROOM_SYMBOL;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++){
            switch (map[i][j])
            {
                case WALL_ID:
                    symbol = WALL_SYMBOL;
                    break;
                
                case UNKNOWN_ID:
                    symbol = UNKNOWN_SYMBOL;
                    break;
                
                case FRONTIER_ID:
                    symbol = FRONTIER_SYMBOL;
                    break;

                case VICTIM_ID:
                    symbol = VICTIM_SYMBOL;
                    break;
                
                case ROBOT_ID:
                    symbol = ROBOT_SYMBOL;
                    break;

                case ROBOT_BASE_ID:
                    symbol = ROBOT_BASE_SYMBOL;
                    break;

                case PORTAL_ID:
                    symbol = PORTAL_SYMBOL;
                    break;

                default:
                    symbol = ROOM_SYMBOL;
                    break;
            }
            std::cout << symbol;
        }

        std::cout << std::endl;
    }
}
void DisplayMap(Map* map){
    uint8_t h = map->GetHeight();
    uint8_t w = map->GetWidth();

    int** mat = new int*[h];
    for (int i = 0; i < h; i++)
        mat[i] = new int[w];
    
    auto matrix = map->GetMap_Matrixe();
    for (uint8_t i = 0; i < h; i++)
        for (uint8_t j = 0; j < w; j++)
            mat[i][j] = matrix.get()[i * w + j];

    DisplayMap(mat, w, h);

    for (int i = 0; i < h; i++)
        delete[] mat[i];
    delete[] mat;
}

void CloseFile(std::ifstream& file)
{
    file.close();
}

void DeleteMap(uint8_t** mat, uint8_t height)
{
    for (int i = 0; i < height; i++)
        delete[] mat[i];
    delete[] mat;
}

void ClearConsole()
{
    system("clear");
}