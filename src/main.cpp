#include "../includes/mapReader.h"
#include "../includes/mapGenerator.h"
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{
    int robotCoord[] = {4, 4};
    srand((unsigned)time(NULL));
    

    std::ifstream file = OpenFile(argv[1]);
    if (!file.is_open())
    {
        std::cout << "Fichier introuvable" << std::endl;
        return 1;
    }

    uint8_t *size = GetSize(file);

    uint8_t **map = CreateMat(size[0], size[1]);
    GetMap(file, map, size[0], size[1]);

    uint8_t** map2 = generateMap(size[0], size[1]);
    uint8_t** verticalEdges = generateVerticalEdges(map2, size[0], size[1]);
    uint8_t** horizontalEdges = generateHorizontalEdges(map2, size[0], size[1]);

    // Test affichage map
    DisplayMap(map, size[0], size[1]);
    std::cout << std::endl;
    DisplayMap(map2, size[0], size[1]);
    std::cout << std::endl;
    DisplayMap(verticalEdges, size[0]-1, size[1]-1);
    std::cout << std::endl;
    DisplayMap(horizontalEdges, size[0]-1, size[1]-1);
    // Test affichage move random robot
    /*
    short move;
    bool possible = 0;
    short temp = map[robotCoord[0]][robotCoord[1]];
    for (int i = 0; i < 20; i++)
    {
        ClearConsole();
        DisplayMap(map, size[0], size[1]);
        while (possible == 0){
            move = random() % 4;
            if (move == 0 && robotCoord[0] > 0 && (map[robotCoord[0] - 1][robotCoord[1]] != WALL_ID && map[robotCoord[0] - 1][robotCoord[1]] != UNKNOWN_ID && map[robotCoord[0] - 1][robotCoord[1]] != FRONTIER_ID))
                possible = 1;
            else if (move == 1 && robotCoord[0] < size[0] - 1 && (map[robotCoord[0] + 1][robotCoord[1]] != WALL_ID && map[robotCoord[0] + 1][robotCoord[1]] != UNKNOWN_ID && map[robotCoord[0] + 1][robotCoord[1]] != FRONTIER_ID))
                possible = 1;
            else if (move == 2 && robotCoord[1] > 0 && (map[robotCoord[0]][robotCoord[1] - 1] != WALL_ID && map[robotCoord[0]][robotCoord[1] - 1] != UNKNOWN_ID && map[robotCoord[0]][robotCoord[1] - 1] != FRONTIER_ID))
                possible = 1;
            else if (move == 3 && robotCoord[1] < size[1] - 1 && (map[robotCoord[0]][robotCoord[1] + 1] != WALL_ID && map[robotCoord[0]][robotCoord[1] + 1] != UNKNOWN_ID && map[robotCoord[0]][robotCoord[1] + 1] != FRONTIER_ID))
                possible = 1;
        }

        map[robotCoord[0]][robotCoord[1]] = temp;

        switch (move)
        {
            case 0:
                robotCoord[0]--;
                break;
            case 1:
                robotCoord[0]++;
                break;
            case 2:
                robotCoord[1]--;
                break;
            case 3:
                robotCoord[1]++;
                break;
        }
        possible = 0;
        
        temp = map[robotCoord[0]][robotCoord[1]];
        map[robotCoord[0]][robotCoord[1]] = ROBOT_ID;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }*/


    CloseFile(file);
    return 0;
}