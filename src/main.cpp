#include "../includes/data.h"

int main(int argc, char **argv)
{
    std::ifstream file = OpenFile(argv[1]);
    if (!file.is_open())
    {
        std::cout << "Fichier introuvable" << std::endl;
        return 1;
    }

    short *size = GetSize(file);

    short **map = CreateMat(size[0], size[1]);
    GetMap(file, map, size[0], size[1]);
    PrintMap(map, size[0], size[1]);
    CloseFile(file);
    return 0;
}