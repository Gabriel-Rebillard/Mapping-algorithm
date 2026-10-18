#include "../includes/map.h"
#include "../includes/mapReader.h"
#include "../includes/mapGenerator.h"
#include <iostream>

int main(int argc, char **argv)
{
    // Help infos 
    if (argv[1] == NULL || argv[1] == "-h" || argv[1] == "--help" || argv[1] == "help"){
        std::cout << "Usage : ./main <width> <height> [seed]" << std::endl;
        return 0;
    }

    // Seed selection
    if (argv[3] != NULL)
        srand(std::stoi(argv[3]));
    else
        srand((unsigned)time(NULL));

    ClearConsole();

    
    // Map generation
    int w = std::stoi(argv[1]) , h = std::stoi(argv[2]);
    std::unique_ptr<Map> map = generateMap(w, h);


    // Display map
    DisplayMap(map.get());


    // TESTS ****************************************************************************************
    // neighbours verification on case (2;2)
    auto n = map->GetNeighbours(2, 2);
    for (auto neighbour = n.get()->begin(); neighbour != n.get()->end(); neighbour++){
        uint8_t* pos = map->GetCasePosition(*neighbour);
        std::cout << "x : " << (int)pos[0] << " y : " << (int)pos[1] << std::endl;
        std::cout << (int)vToC(map->GetCase(2, 2))->GetEdgeWeight(*neighbour) << std::endl;
    }

    // Enxtending tests
    map->ExtendMap(UP);
    DisplayMap(map.get());

    map->ExtendMap(RIGHT);
    DisplayMap(map.get());

    map->ExtendMap(DOWN);
    DisplayMap(map.get());

    map->ExtendMap(LEFT);
    DisplayMap(map.get());

    map->ExtendMap(LEFT);
    DisplayMap(map.get());
    
    return 0;
}